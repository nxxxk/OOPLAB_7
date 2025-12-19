#include <chrono>
#include <iostream>
#include <thread>

#include "world.hpp"

int main() {
    World world;
    world.start();

    std::this_thread::sleep_for(std::chrono::seconds(World::GAME_DURATION_SEC + 2));

    std::cout << "\nПрограмма завершена.\n";
    return 0;
}
