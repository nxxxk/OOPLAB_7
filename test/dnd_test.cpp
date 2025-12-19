// tests.cpp
#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include <thread>

#include "factory.hpp"
#include "world.hpp"
#include "orc.hpp"
#include "bear.hpp"
#include "squirrel.hpp"
#include "BattleVisitor.hpp"
#include "observers.hpp"

TEST(NPCFactoryTest, CreateNPCs) {
    auto orc = create(OrcType, 10, 20);
    ASSERT_NE(orc, nullptr);
    EXPECT_TRUE(orc->is_orc());
    EXPECT_FALSE(orc->is_bear());
    EXPECT_EQ(orc->x, 10);
    EXPECT_EQ(orc->y, 20);
    
    auto bear = create(BearType, 30, 40);
    ASSERT_NE(bear, nullptr);
    EXPECT_TRUE(bear->is_bear());
    
    auto squirrel = create(SquirrelType, 50, 60);
    ASSERT_NE(squirrel, nullptr);
    EXPECT_TRUE(squirrel->is_squirrel());
}

TEST(NPCLoadTest, LoadFromStream) {
    std::istringstream iss("1 100 200\n2 300 400\n3 500 600");
    
    auto orc = create(iss);
    ASSERT_NE(orc, nullptr);
    EXPECT_EQ(orc->type, OrcType);
    EXPECT_EQ(orc->x, 100);
    EXPECT_EQ(orc->y, 200);
    
    auto bear = create(iss);
    ASSERT_NE(bear, nullptr);
    EXPECT_EQ(bear->type, BearType);
    
    auto squirrel = create(iss);
    ASSERT_NE(squirrel, nullptr);
    EXPECT_EQ(squirrel->type, SquirrelType);
}

TEST(NPCDistanceTest, IsCloseCalculation) {
    auto npc1 = create(OrcType, 0, 0);
    auto npc2 = create(BearType, 5, 5);

    EXPECT_TRUE(npc1->is_close(npc2, 10));
    EXPECT_FALSE(npc1->is_close(npc2, 9));

    npc2->kill();
    EXPECT_FALSE(npc1->is_close(npc2, 100));
}

TEST(NPCMovementTest, RandomMovementWithinBounds) {
    auto npc = create(OrcType, 50, 50);
    const int MAP_WIDTH = 100;
    const int MAP_HEIGHT = 100;

    for (int i = 0; i < 100; ++i) {
        int old_x = npc->x;
        int old_y = npc->y;
        
        npc->move_random(MAP_WIDTH, MAP_HEIGHT);

        EXPECT_GE(npc->x, 0);
        EXPECT_LT(npc->x, MAP_WIDTH);
        EXPECT_GE(npc->y, 0);
        EXPECT_LT(npc->y, MAP_HEIGHT);

        EXPECT_LE(std::abs(npc->x - old_x), 20);
        EXPECT_LE(std::abs(npc->y - old_y), 20);
    }
}

TEST(BattleVisitorTest, OrcBattleLogic) {
    auto orc = create(OrcType, 0, 0);
    auto enemyOrc = create(OrcType, 1, 1);
    auto enemyBear = create(BearType, 2, 2);
    auto enemySquirrel = create(SquirrelType, 3, 3);
    
    OrcVisitor visitor(orc);

    bool result1 = visitor.visit(*dynamic_cast<Orc*>(enemyOrc.get()));

    bool result2 = visitor.visit(*dynamic_cast<Bear*>(enemyBear.get()));
    EXPECT_TRUE(result2);

    bool result3 = visitor.visit(*dynamic_cast<Squirrel*>(enemySquirrel.get()));
    EXPECT_TRUE(result3);
}

TEST(BattleVisitorTest, SquirrelBattleLogic) {
    auto squirrel = create(SquirrelType, 0, 0);
    auto enemyOrc = create(OrcType, 1, 1);
    auto enemyBear = create(BearType, 2, 2);
    auto enemySquirrel = create(SquirrelType, 3, 3);
    
    SquirrelVisitor visitor(squirrel);

    bool result1 = visitor.visit(*dynamic_cast<Orc*>(enemyOrc.get()));
    EXPECT_FALSE(result1);
    
    bool result2 = visitor.visit(*dynamic_cast<Bear*>(enemyBear.get()));
    EXPECT_FALSE(result2);
    
    bool result3 = visitor.visit(*dynamic_cast<Squirrel*>(enemySquirrel.get()));
    EXPECT_FALSE(result3);
}

TEST(ObserverTest, SubscriptionAndNotification) {
    auto npc = create(OrcType, 0, 0);

    class MockObserver : public IFightObserver {
    public:
        int notificationCount = 0;
        void on_fight(const std::shared_ptr<NPC>& attacker,
                     const std::shared_ptr<NPC>& defender, bool win) override {
            notificationCount++;
        }
    };
    
    auto observer = std::make_shared<MockObserver>();
    npc->subscribe(observer);

    auto defender = create(BearType, 1, 1);
    npc->fight_notify(defender, true);
    
    EXPECT_EQ(observer->notificationCount, 1);
}

TEST(WorldTest, AddAndRemoveNPC) {
    World world;

    EXPECT_EQ(world.alive_count(), 50);

    auto npc = create(OrcType, 10, 20);
    world.add(npc);
    EXPECT_EQ(world.alive_count(), 51);

    world.remove(npc);
    EXPECT_EQ(world.alive_count(), 50);
}

TEST(NPCSerializationTest, SaveAndLoad) {

    auto originalOrc = std::make_shared<Orc>(100, 200);
    auto originalBear = std::make_shared<Bear>(300, 400);
    auto originalSquirrel = std::make_shared<Squirrel>(500, 600);
    
    std::ostringstream oss;
    originalOrc->save(oss);
    originalBear->save(oss);
    originalSquirrel->save(oss);

    std::istringstream iss(oss.str());
    
    auto loadedOrc = create(iss);
    ASSERT_NE(loadedOrc, nullptr);
    EXPECT_EQ(loadedOrc->type, OrcType);
    EXPECT_EQ(loadedOrc->x, 100);
    EXPECT_EQ(loadedOrc->y, 200);
    
    auto loadedBear = create(iss);
    ASSERT_NE(loadedBear, nullptr);
    EXPECT_EQ(loadedBear->type, BearType);
    
    auto loadedSquirrel = create(iss);
    ASSERT_NE(loadedSquirrel, nullptr);
    EXPECT_EQ(loadedSquirrel->type, SquirrelType);
}

TEST(VisitorFactoryTest, CreateVisitorsForDifferentNPCs) {
    auto orc = create(OrcType, 0, 0);
    auto bear = create(BearType, 1, 1);
    auto squirrel = create(SquirrelType, 2, 2);
    
    auto orcVisitor = create_visitors(orc);
    ASSERT_NE(orcVisitor, nullptr);
    EXPECT_NE(dynamic_cast<OrcVisitor*>(orcVisitor.get()), nullptr);
    
    auto bearVisitor = create_visitors(bear);
    ASSERT_NE(bearVisitor, nullptr);
    EXPECT_NE(dynamic_cast<BearVisitor*>(bearVisitor.get()), nullptr);
    
    auto squirrelVisitor = create_visitors(squirrel);
    ASSERT_NE(squirrelVisitor, nullptr);
    EXPECT_NE(dynamic_cast<SquirrelVisitor*>(squirrelVisitor.get()), nullptr);

    EXPECT_THROW(create_visitors(nullptr), std::invalid_argument);
}

TEST(NPCPropertiesTest, TypeSpecificProperties) {
    auto orc = create(OrcType, 0, 0);
    auto bear = create(BearType, 0, 0);
    auto squirrel = create(SquirrelType, 0, 0);

    EXPECT_EQ(orc->get_move_distance(), 20);
    EXPECT_EQ(bear->get_move_distance(), 5);
    EXPECT_EQ(squirrel->get_move_distance(), 5);

    EXPECT_EQ(orc->get_kill_distance(), 10);
    EXPECT_EQ(bear->get_kill_distance(), 10);
    EXPECT_EQ(squirrel->get_kill_distance(), 5);

    EXPECT_TRUE(orc->is_orc());
    EXPECT_FALSE(orc->is_bear());
    EXPECT_FALSE(orc->is_squirrel());
}
