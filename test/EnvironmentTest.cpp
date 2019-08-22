//
// Created by Jens Klimke on 2019-04-20.
//

#include <traffic/Environment.h>
#include <traffic/Agent.h>
#include <core/Loop.h>
#include <core/IComponent.h>
#include <cmath>
#include <gtest/gtest.h>

#ifndef EPS_DISTANCE
#define EPS_DISTANCE 1e-6
#endif



class EnvironmentTest : public ::testing::Test, public Environment {


protected:

    ::sim::Loop sim{};
    Agent agent{};
    std::string file;

public:

    void SetUp() override {

        // get map path: SIMMAP_SRC has to be provided by cmake
        std::stringstream ss{};
        ss << SIMMAP_SRC << "/tests/tracks/CircleR100.xodr";
        file = ss.str();

        // load map
        this->registerMap(file);

        // add agents
        this->createAgent(&agent, 1, {"1", "-2"});
        agent.setMapPosition("R1-LS2-R2", 42.9203673205, 0.5);

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
    EXPECT_EQ(1, agent.getID());

    // get position
    auto pos = agent.getPosition();
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
    agent.setPosition(pos, 1.0);

    // get position
    pos = agent.getPosition();
    EXPECT_NEAR(-43.6954, pos.x,     1e-4);
    EXPECT_NEAR( 95.4762, pos.y,     1e-4);
    EXPECT_NEAR(  0.0,    pos.z,     1e-4);
    EXPECT_NEAR(  3.5708, pos.phi,   1e-4);


    pos = Agent::Position{};
    pos.x     = -48.4126;
    pos.y     =  93.1730;
    pos.phi   =   4.0;

    // set position
    agent.setPosition(pos, 10.0);

    // get position
    pos = agent.getPosition();
    EXPECT_NEAR(-48.4126, pos.x,     1e-4);
    EXPECT_NEAR( 93.1730, pos.y,     1e-4);
    EXPECT_NEAR(  0.0,    pos.z,     1e-4);
    EXPECT_NEAR(  4.0,    pos.phi,   1e-4);

}


TEST_F(EnvironmentTest, MoveAgent) {

    // move agent
    agent.move(5.0, 0.5);

    // get position
    auto pos = agent.getPosition();
    EXPECT_NEAR(-48.4126, pos.x,     1e-4);
    EXPECT_NEAR( 93.1730, pos.y,     1e-4);
    EXPECT_NEAR(  0.0,    pos.z,     1e-4);
    EXPECT_NEAR(  3.6207, pos.phi,   1e-4);

    // get map position
    auto mpos = agent.getMapPosition();
    EXPECT_EQ("R1-LS2-R2", std::string(mpos.edgeID));
    EXPECT_NEAR(47.9203673205, mpos.longPos, EPS_DISTANCE);
    EXPECT_NEAR(0.5, mpos.latPos, EPS_DISTANCE);

}


TEST_F(EnvironmentTest, MatchAgent) {

    // set positions
    Agent::Position pos{};
    pos.x = -48.4126;
    pos.y = 93.1730;
    pos.z = 0.0;
    pos.phi = 3.6207;

    // set position
    agent.setMapPosition("R1-LS2-R2", 47.0, 0.0);
    agent.setPosition(pos, 10.0);

    // get map position
    auto mpos = agent.getMapPosition();
    EXPECT_EQ("R1-LS2-R2", std::string(mpos.edgeID));
    EXPECT_NEAR(47.9203673205, mpos.longPos, 1e-4);
    EXPECT_NEAR(0.5, mpos.latPos, 1e-4);

}


TEST_F(EnvironmentTest, Horizon) {

    // set position
    agent.setMapPosition("R1-LS1-R1", 100.0, 0.0);

    // set position
    std::vector<double> s = {-1.0, 0.0, 1.0, 10.0, 100.0};
    auto hor = agent.getHorizon(s);

    EXPECT_DOUBLE_EQ(s[0], hor[0].s);

    EXPECT_DOUBLE_EQ(s[1], hor[1].s);
    EXPECT_DOUBLE_EQ(0.0, hor[1].pos.x);
    EXPECT_DOUBLE_EQ(0.0, hor[1].pos.y);

    EXPECT_DOUBLE_EQ(s[2], hor[2].s);

    EXPECT_DOUBLE_EQ(s[3], hor[3].s);
    EXPECT_NEAR(10.17, hor[3].pos.x, 1e-3);
    EXPECT_NEAR(0.509, hor[3].pos.y, 1e-3);

    EXPECT_DOUBLE_EQ(s[4], hor[4].s);
    EXPECT_NEAR(85.725, hor[4].pos.x, 1e-3);
    EXPECT_NEAR(46.832, hor[4].pos.y, 1e-3);

}
