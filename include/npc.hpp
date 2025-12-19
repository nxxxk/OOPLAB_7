#pragma once

#include <atomic>
#include <iostream>
#include <memory>
#include <mutex>
#include <vector>

struct Orc;
struct Bear;
struct Squirrel;
struct NPC;
struct IVisitor;

enum NpcType { Unknown = 0, OrcType = 1, BearType = 2, SquirrelType = 3 };

struct IFightObserver {
    virtual void on_fight(const std::shared_ptr<NPC>& attacker,
                          const std::shared_ptr<NPC>& defender, bool win) = 0;
    virtual ~IFightObserver() = default;
};

struct NPC : public std::enable_shared_from_this<NPC> {
    NpcType type{NpcType::Unknown};
    std::atomic<int> x, y;
    std::atomic<bool> alive{true};

    virtual bool is_orc() const;
    virtual bool is_bear() const;
    virtual bool is_squirrel() const;

    std::vector<std::shared_ptr<IFightObserver>> observers;
    mutable std::mutex observer_mutex;

    NPC(NpcType t, int _x, int _y) : type(t), x(_x), y(_y) {
    }
    virtual ~NPC() = default;

    void subscribe(const std::shared_ptr<IFightObserver>& obs);
    void fight_notify(const std::shared_ptr<NPC>& defender, bool win);

    bool is_close(const std::shared_ptr<NPC>& other, int distance) const;

    // Двойная диспетчеризация
    virtual bool accept(IVisitor& visitor) = 0;

    virtual int get_move_distance() const = 0;
    virtual int get_kill_distance() const = 0;

    virtual void print() const = 0;
    virtual void save(std::ostream& os) const = 0;

    void move_random(int max_x, int max_y);
    void kill() {
        alive = false;
    }
    bool is_alive() const {
        return alive.load();
    }
};
