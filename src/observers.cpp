#include "observers.hpp"

std::mutex g_cout_mutex;

// ConsoleObserver — синглтон
std::shared_ptr<IFightObserver> ConsoleObserver::get() {
    static ConsoleObserver instance;
    return std::shared_ptr<IFightObserver>(&instance, [](IFightObserver*) {});
}

void ConsoleObserver::on_fight(const std::shared_ptr<NPC>& attacker,
                               const std::shared_ptr<NPC>& defender, bool win) {
    //
}

// FileObserver — синглтон
FileObserver::FileObserver() {
    logfile.open("fight_log.txt", std::ios::app);
}

FileObserver::~FileObserver() {
    if (logfile.is_open()) {
        logfile.close();
    }
}

std::shared_ptr<IFightObserver> FileObserver::get() {
    static FileObserver instance;
    return std::shared_ptr<IFightObserver>(&instance, [](IFightObserver*) {});
}

void FileObserver::on_fight(const std::shared_ptr<NPC>& attacker,
                            const std::shared_ptr<NPC>& defender, bool win) {
    std::lock_guard<std::mutex> lock(file_mutex);
    if (!logfile.is_open())
        return;

    const char* attacker_type = "Unknown";
    const char* defender_type = "Unknown";

    if (attacker) {
        switch (attacker->type) {
        case NpcType::OrcType:
            attacker_type = "Orc";
            break;
        case NpcType::BearType:
            attacker_type = "Bear";
            break;
        case NpcType::SquirrelType:
            attacker_type = "Squirrel";
            break;
        default:
            attacker_type = "Unknown";
        }
    }

    if (defender) {
        switch (defender->type) {
        case NpcType::OrcType:
            defender_type = "Orc";
            break;
        case NpcType::BearType:
            defender_type = "Bear";
            break;
        case NpcType::SquirrelType:
            defender_type = "Squirrel";
            break;
        default:
            defender_type = "Unknown";
        }
    }

    logfile << "Fight: " << attacker_type << " (" << attacker->x << "," << attacker->y << ") ";
    logfile << (win ? "wins against " : "loses to ");
    logfile << defender_type << " (" << defender->x << "," << defender->y << ")\n";
    logfile.flush();
}
