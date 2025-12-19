#include "BattleVisitor.hpp"

#include <cstdlib>

#include "bear.hpp"
#include "orc.hpp"
#include "squirrel.hpp"

bool OrcVisitor::visit(Orc& defender) {
    bool win = rand() % 2;
    std::shared_ptr<NPC> npc_defender = defender.shared_from_this();
    attacker->fight_notify(npc_defender, win);
    return win;
}

bool OrcVisitor::visit(Bear& defender) {
    bool win = true;
    std::shared_ptr<NPC> npc_defender = defender.shared_from_this();
    attacker->fight_notify(npc_defender, win);
    return win;
}

bool OrcVisitor::visit(Squirrel& defender) {
    bool win = true;
    std::shared_ptr<NPC> npc_defender = defender.shared_from_this();
    attacker->fight_notify(npc_defender, win);
    return win;
}

bool BearVisitor::visit(Orc& defender) {
    bool win = false;
    std::shared_ptr<NPC> npc_defender = defender.shared_from_this();
    attacker->fight_notify(npc_defender, win);
    return win;
}

bool BearVisitor::visit(Bear& defender) {
    bool win = rand() % 2;
    std::shared_ptr<NPC> npc_defender = defender.shared_from_this();
    attacker->fight_notify(npc_defender, win);
    return win;
}

bool BearVisitor::visit(Squirrel& defender) {
    bool win = true;
    std::shared_ptr<NPC> npc_defender = defender.shared_from_this();
    attacker->fight_notify(npc_defender, win);
    return win;
}

bool SquirrelVisitor::visit(Squirrel& defender) {
    bool win = false;
    std::shared_ptr<NPC> npc_defender = defender.shared_from_this();
    attacker->fight_notify(npc_defender, win);
    return win;
}

bool SquirrelVisitor::visit(Orc& defender) {
    bool win = false;
    std::shared_ptr<NPC> npc_defender = defender.shared_from_this();
    attacker->fight_notify(npc_defender, win);
    return win;
}

bool SquirrelVisitor::visit(Bear& defender) {
    bool win = false;
    std::shared_ptr<NPC> npc_defender = defender.shared_from_this();
    attacker->fight_notify(npc_defender, win);
    return win;
}
