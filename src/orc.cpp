#include "orc.hpp"

#include "BattleVisitor.hpp"

Orc::Orc(int x, int y) : NPC(NpcType::OrcType, x, y) {
}
Orc::Orc(std::istream& is) : NPC(NpcType::OrcType, 0, 0) {
    int tmp_x, tmp_y;
    is >> tmp_x >> tmp_y;
    x = tmp_x;
    y = tmp_y;
}

bool Orc::is_orc() const {
    return true;
}

bool Orc::accept(IVisitor& visitor) {
    return visitor.visit(*this);
}

void Orc::print() const {
    std::cout << "Orc   (" << x << "," << y << ")";
}
void Orc::save(std::ostream& os) const {
    os << int(type) << ' ' << x << ' ' << y << '\n';
}
