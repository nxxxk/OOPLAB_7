#pragma once

#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>

#include "npc.hpp"

extern std::mutex g_cout_mutex;

struct ConsoleObserver : public IFightObserver {
    static std::shared_ptr<IFightObserver> get();
    void on_fight(const std::shared_ptr<NPC>& attacker, const std::shared_ptr<NPC>& defender,
                  bool win) override;

  private:
    ConsoleObserver() = default;
};

struct FileObserver : public IFightObserver {
    static std::shared_ptr<IFightObserver> get();
    void on_fight(const std::shared_ptr<NPC>& attacker, const std::shared_ptr<NPC>& defender,
                  bool win) override;

  private:
    FileObserver();
    ~FileObserver();

    std::ofstream logfile;
    std::mutex file_mutex;
};
