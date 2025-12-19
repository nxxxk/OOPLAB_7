#include "factory.hpp"

#include <stdexcept>

#include "bear.hpp"
#include "observers.hpp"
#include "orc.hpp"
#include "squirrel.hpp"

std::shared_ptr<NPC> create(NpcType type, int x, int y) {
    std::shared_ptr<NPC> result;

    switch (type) {
    case OrcType:
        result = std::make_shared<Orc>(x, y);
        break;
    case BearType:
        result = std::make_shared<Bear>(x, y);
        break;
    case SquirrelType:
        result = std::make_shared<Squirrel>(x, y);
        break;
    default:
        break;
    }

    if (result) {
        result->subscribe(ConsoleObserver::get());
        result->subscribe(FileObserver::get());
    }

    return result;
}

std::shared_ptr<NPC> create(std::istream& is) {
    std::shared_ptr<NPC> result;
    int type = 0;

    if (is >> type) {
        int x, y;

        if (!(is >> x >> y)) {
            std::cerr << "Invalid NPC entry in file\n";
            return nullptr;
        }

        switch (type) {
        case OrcType:
            result = std::make_shared<Orc>(x, y);
            break;
        case BearType:
            result = std::make_shared<Bear>(x, y);
            break;
        case SquirrelType:
            result = std::make_shared<Squirrel>(x, y);
            break;
        default:
            std::cerr << "Unknown NPC type: " << type << "\n";
            return nullptr;
        }
    }

    if (result) {
        result->subscribe(ConsoleObserver::get());
        result->subscribe(FileObserver::get());
    }

    return result;
}

std::unique_ptr<IVisitor> create_visitors(std::shared_ptr<NPC> attacker) {
    if (!attacker)
        throw std::invalid_argument("Attacker can't be null");

    switch (attacker->type) {
    case OrcType:
        return std::make_unique<OrcVisitor>(attacker);
    case BearType:
        return std::make_unique<BearVisitor>(attacker);
    case SquirrelType:
        return std::make_unique<SquirrelVisitor>(attacker);
    default:
        throw std::invalid_argument("Unknow NPC type");
    }
}
