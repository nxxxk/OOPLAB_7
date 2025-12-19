#include "bear.hpp"

#include "BattleVisitor.hpp"

Bear::Bear(int x, int y) : NPC(NpcType::BearType, x, y) {
}
Bear::Bear(std::istream& is) : NPC(NpcType::BearType, 0, 0) {
    int tmp_x, tmp_y;
    is >> tmp_x >> tmp_y;
    x = tmp_x;
    y = tmp_y;
}

bool Bear::is_bear() const {
    return true;
}

bool Bear::accept(IVisitor& visitor) {
    return visitor.visit(*this);
}

void Bear::print() const {
    std::cout << "Bear  (" << x << "," << y << ")";
}
void Bear::save(std::ostream& os) const {
    os << int(type) << ' ' << x << ' ' << y << '\n';
}
