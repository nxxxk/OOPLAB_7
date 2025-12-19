#pragma once
#include "npc.hpp"

struct Bear : public NPC {
    Bear(int x, int y);
    Bear(std::istream& is);

    bool is_bear() const override;

    bool accept(IVisitor& visitor) override;

    void print() const override;
    void save(std::ostream& os) const override;

    int get_move_distance() const override {
        return 5;
    }
    int get_kill_distance() const override {
        return 10;
    }
};
