#pragma once

#include "BattleVisitor.hpp"
#include "npc.hpp"

std::shared_ptr<NPC> create(NpcType type, int x, int y);
std::shared_ptr<NPC> create(std::istream& is);
std::unique_ptr<IVisitor> create_visitors(std::shared_ptr<NPC> attacker);
