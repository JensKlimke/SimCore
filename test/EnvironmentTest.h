//
// Created by Jens Klimke on 2019-04-20.
//

#ifndef SIMCORE_ENVIRONMENTTEST_H
#define SIMCORE_ENVIRONMENTTEST_H

#include <gtest/gtest.h>
#include <components/traffic/Environment.h>

class EnvironmentTest : public ::testing::Test, public Environment {


protected:

    ::sim::Loop sim{};
    Agent *agent;

public:

    void SetUp() override {

        // load map
        this->registerMap("tracks/CircleR100.xodr");

        // add virtual horizon as component
        sim.addComponent(this);

        // add agents
        agent = this->createAgent(1, {"1", "-2"});
        agent->setMapPosition("1", -2, 200.0, 0.5);

    }


    void TearDown() override {

        // clear all
        this->clear();

    }

};

TEST_F(EnvironmentTest, MapLoaded) {

    EXPECT_EQ(1, this->getMapID());

}

TEST_F(EnvironmentTest, RegisterAgents) {

    // check id
    EXPECT_EQ(1, agent->getID());

    // get position
    auto pos = agent->getPosition();
    EXPECT_NEAR(-43.6954, pos.x,     1e-4);
    EXPECT_NEAR( 95.4762, pos.y,     1e-4);
    EXPECT_NEAR(  0.0,    pos.z,     1e-4);
    EXPECT_NEAR(  3.5708, pos.phi,   1e-4);
    EXPECT_NEAR(  0.0095, pos.kappa, 1e-4);

}


TEST_F(EnvironmentTest, SetAgent) {

    Agent::Position pos;
    pos.x     = -43.6954;
    pos.y     =  95.4762;
    pos.phi   =   3.5708;

    // set position
    agent->setPosition(pos, 1.0);

    // get position
    pos = agent->getPosition();
    EXPECT_NEAR(-43.6954, pos.x,     1e-4);
    EXPECT_NEAR( 95.4762, pos.y,     1e-4);
    EXPECT_NEAR(  0.0,    pos.z,     1e-4);
    EXPECT_NEAR(  3.5708, pos.phi,   1e-4);


    pos = Agent::Position{};
    pos.x     = -48.4126;
    pos.y     =  93.1730;
    pos.phi   =   4.0;

    // set position
    agent->setPosition(pos, 10.0);

    // get position
    pos = agent->getPosition();
    EXPECT_NEAR(-48.4126, pos.x,     1e-4);
    EXPECT_NEAR( 93.1730, pos.y,     1e-4);
    EXPECT_NEAR(  0.0,    pos.z,     1e-4);
    EXPECT_NEAR(  4.0,    pos.phi,   1e-4);

}


TEST_F(EnvironmentTest, MoveAgent) {

    // move agent
    agent->move(5.0, 0.5);

    // get position
    auto pos = agent->getPosition();
    EXPECT_NEAR(-48.4126, pos.x,     1e-4);
    EXPECT_NEAR( 93.1730, pos.y,     1e-4);
    EXPECT_NEAR(  0.0,    pos.z,     1e-4);
    EXPECT_NEAR(  3.6207, pos.phi,   1e-4);

}


#endif //SIMCORE_ENVIRONMENTTEST_H
