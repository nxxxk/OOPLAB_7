#include "squirrel.hpp"

#include "BattleVisitor.hpp"

Squirrel::Squirrel(int x, int y) : NPC(NpcType::SquirrelType, x, y) {
}
Squirrel::Squirrel(std::istream& is) : NPC(NpcType::SquirrelType, 0, 0) {
    int tmp_x, tmp_y;
    is >> tmp_x >> tmp_y;
    x = tmp_x;
    y = tmp_y;
}

bool Squirrel::is_squirrel() const {
    return true;
}

bool Squirrel::accept(IVisitor& visitor) {
    return visitor.visit(*this);
}

void Squirrel::print() const {
    std::cout << "Squirrel (" << x << "," << y << ")";
}
void Squirrel::save(std::ostream& os) const {
    os << int(type) << ' ' << x << ' ' << y << '\n';
}
