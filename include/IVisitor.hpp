#pragma once

#include <memory>

#include "npc.hpp"

struct IVisitor {
    std::shared_ptr<NPC> attacker;

    IVisitor(std::shared_ptr<NPC> attacker_) : attacker(attacker_) {
    }

    virtual bool visit(Orc& defender) = 0;
    virtual bool visit(Bear& defender) = 0;
    virtual bool visit(Squirrel& defender) = 0;

    virtual ~IVisitor() = default;
};
