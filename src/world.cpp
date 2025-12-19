#include "world.hpp"

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>

#include "factory.hpp"

World::World() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> type_dist(1, 3);
    std::uniform_int_distribution<> coord_dist(0, MAP_WIDTH - 1);

    for (int i = 0; i < 50; ++i) {
        NpcType type = static_cast<NpcType>(type_dist(gen));
        int x = coord_dist(gen);
        int y = coord_dist(gen);
        auto npc = create(type, x, y);
        if (npc)
            add(npc);
    }
}

World::~World() {
    stop();
}

void World::add(const std::shared_ptr<NPC>& npc) {
    std::unique_lock<std::shared_mutex> lock(npcs_mutex);
    npcs.insert(npc);
}

void World::remove(const std::shared_ptr<NPC>& npc) {
    std::unique_lock<std::shared_mutex> lock(npcs_mutex);
    npcs.erase(npc);
}

void World::start() {
    stop_requested = false;

    system("clear");

    movement_thread = std::thread(&World::movement_worker, this);
    battle_thread = std::thread(&World::battle_worker, this);
    display_thread = std::thread(&World::display_worker, this);
}

void World::stop() {
    stop_requested = true;
    queue_cv.notify_all();

    if (movement_thread.joinable())
        movement_thread.join();
    if (battle_thread.joinable())
        battle_thread.join();
    if (display_thread.joinable())
        display_thread.join();
}

int World::roll_dice() const
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dice(1, 6);
    return dice(gen);
}

void World::movement_worker() {
    while (!stop_requested) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        std::shared_lock<std::shared_mutex> lock(npcs_mutex);
        for (const auto& npc : npcs) {
            if (npc->is_alive()) {
                npc->move_random(MAP_WIDTH, MAP_HEIGHT);
            }
        }
        lock.unlock();

        check_and_queue_battles();
    }
}

void World::check_and_queue_battles() {
    std::vector<std::pair<std::shared_ptr<NPC>, std::shared_ptr<NPC>>> battles;

    {
        std::shared_lock<std::shared_mutex> lock(npcs_mutex);
        for (auto it1 = npcs.begin(); it1 != npcs.end(); ++it1) {
            if (!(*it1)->is_alive())
                continue;

            for (auto it2 = std::next(it1); it2 != npcs.end(); ++it2) {
                if (!(*it2)->is_alive())
                    continue;

                int kill_dist = std::max((*it1)->get_kill_distance(), (*it2)->get_kill_distance());
                if ((*it1)->is_close(*it2, kill_dist)) {
                    battles.emplace_back(*it1, *it2);
                }
            }
        }
    }

    if (!battles.empty()) {
        std::unique_lock<std::mutex> qlock(queue_mutex);
        for (auto& b : battles) {
            battle_queue.push(b);
        }
        qlock.unlock();
        queue_cv.notify_one();
    }
}

void World::battle_worker() {
    while (!stop_requested) {
        std::unique_lock<std::mutex> lock(queue_mutex);
        queue_cv.wait(lock, [this] { return !battle_queue.empty() || stop_requested; });

        if (stop_requested)
            break;

        auto battle = battle_queue.front();
        battle_queue.pop();
        lock.unlock();

        auto attacker = battle.first;
        auto defender = battle.second;

        if (!attacker || !defender || !attacker->is_alive() || !defender->is_alive())
            continue;

        try {
            int attack_power = roll_dice();
            int defense_power = roll_dice();

            if (attack_power > defense_power) {
                auto visitor = create_visitors(attacker);
                if (!visitor)
                    continue;

                bool attacker_wins = defender->accept(*visitor);
                if (attacker_wins) {
                    defender->kill();
                    remove(defender);
                }
            }
            
        } catch (const std::exception& e) {
            std::lock_guard<std::mutex> cout_lock(cout_mutex);
            std::cerr << "Ошибка битвы: " << e.what() << std::endl;
        }
    }
}

void World::display_worker() {
    auto start = std::chrono::steady_clock::now();
    auto last_update = start;

    while (!stop_requested) {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();

        auto time_since_update =
            std::chrono::duration_cast<std::chrono::milliseconds>(now - last_update);
        if (time_since_update.count() >= 1000) {
            last_update = now;
            print_map();
        }

        if (elapsed >= GAME_DURATION_SEC) {
            stop_requested = true;
            queue_cv.notify_all();
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    print_survivors();
}

void World::print_map() const {
    std::lock_guard<std::mutex> lock(cout_mutex);

    system("clear");

    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            char c = '.';
            std::shared_lock<std::shared_mutex> npc_lock(npcs_mutex);
            for (const auto& npc : npcs) {
                if (npc->is_alive() && npc->x == x && npc->y == y) {
                    if (npc->type == NpcType::OrcType)
                        c = 'O';
                    else if (npc->type == NpcType::BearType)
                        c = 'B';
                    else if (npc->type == NpcType::SquirrelType)
                        c = 'S';
                    break;
                }
            }
            std::cout << c;
        }
        std::cout << '\n';
    }

    std::cout << std::flush;
}

void World::print_survivors() const {
    std::lock_guard<std::mutex> lock(cout_mutex);

    std::cout << "\033[" << (MAP_HEIGHT + 2) << ";1H";

    std::cout << "\n=== ИГРА ОКОНЧЕНА ===\n";
    std::cout << "Выжившие:\n";
    size_t count = 0;
    std::shared_lock<std::shared_mutex> npc_lock(npcs_mutex);
    for (const auto& npc : npcs) {
        if (npc->is_alive()) {
            ++count;
            npc->print();
            std::cout << '\n';
        }
    }
    std::cout << "Всего выживших: " << count << "\n\n";
    std::cout << std::flush;
}

size_t World::alive_count() const {
    std::shared_lock<std::shared_mutex> lock(npcs_mutex);
    return std::count_if(npcs.begin(), npcs.end(), [](const auto& npc) { return npc->is_alive(); });
}
