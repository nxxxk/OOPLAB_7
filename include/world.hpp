#pragma once

#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <set>
#include <shared_mutex>
#include <thread>

#include "npc.hpp"

struct World {
    using set_t = std::set<std::shared_ptr<NPC>>;

    static constexpr int MAP_WIDTH = 100;
    static constexpr int MAP_HEIGHT = 100;
    static constexpr int GAME_DURATION_SEC = 30;

    World();
    ~World();

    void add(const std::shared_ptr<NPC>& npc);
    void remove(const std::shared_ptr<NPC>& npc);

    void start();
    void stop();

    void print_map() const;
    void print_survivors() const;
    size_t alive_count() const;

  private:
    set_t npcs;
    mutable std::shared_mutex npcs_mutex;

    std::queue<std::pair<std::shared_ptr<NPC>, std::shared_ptr<NPC>>> battle_queue;
    std::mutex queue_mutex;
    std::condition_variable queue_cv;

    std::atomic<bool> stop_requested{false};

    std::thread movement_thread;
    std::thread battle_thread;
    std::thread display_thread;

    mutable std::mutex cout_mutex;

    void movement_worker();
    void battle_worker();
    void display_worker();
    int roll_dice() const;

    void check_and_queue_battles();
};
