#pragma once
#include "npc.hpp"

struct Orc : public NPC {
    Orc(int x, int y);
    Orc(std::istream& is);

    bool is_orc() const override;

    bool accept(IVisitor& visitor) override;

    void print() const override;
    void save(std::ostream& os) const override;

    int get_move_distance() const override {
        return 20;
    }
    int get_kill_distance() const override {
        return 10;
    }
};
