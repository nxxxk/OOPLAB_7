#include "npc.hpp"

#include <chrono>
#include <random>

void NPC::subscribe(const std::shared_ptr<IFightObserver>& obs) {
    std::lock_guard<std::mutex> l(observer_mutex);
    observers.push_back(obs);
}

bool NPC::is_orc() const {
    return false;
}
bool NPC::is_bear() const {
    return false;
}
bool NPC::is_squirrel() const {
    return false;
}

void NPC::fight_notify(const std::shared_ptr<NPC>& defender, bool win) {
    std::lock_guard<std::mutex> l(observer_mutex);
    for (auto& o : observers)
        o->on_fight(shared_from_this(), defender, win);
}

bool NPC::is_close(const std::shared_ptr<NPC>& other, int distance) const {
    if (!other->is_alive())
        return false;
    return std::abs(x - other->x) + std::abs(y - other->y) <= distance;
}

void NPC::move_random(int max_x, int max_y) {
    if (!alive)
        return;
    thread_local std::mt19937 gen{std::random_device{}()};
    std::uniform_int_distribution<> d(-get_move_distance(), get_move_distance());

    int nx = std::clamp(x.load() + d(gen), 0, max_x - 1);
    int ny = std::clamp(y.load() + d(gen), 0, max_y - 1);
    x = nx;
    y = ny;
}
