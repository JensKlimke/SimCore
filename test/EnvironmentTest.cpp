//
// Created by Jens Klimke on 2019-04-20.
//

#include <gtest/gtest.h>
#include <components/traffic/Environment.h>
#include <components/traffic/Agent.h>
#include <core/Loop.h>
#include <core/IComponent.h>
#include <cmath>

#ifndef EPS_DISTANCE
#define EPS_DISTANCE 1e-6
#endif


struct IDM : public ::sim::IComponent {


    Agent *ag = nullptr;

    double dv = 0.0;
    double v  = 0.0;
    double s  = 0.0;
    double a  = 0.0;


    void initialize(double initTime) override {

        initializeTimer(initTime);

    }

    bool step(double simTime) override {

        // parameters
        double ac  = 0.73;
        double bc  = 1.67;
        double v0  = 100.0 / 3.6;
        double T   = 1.5;
        double s0  = 2.0;

        auto s_star = s0 + v * T + (v * dv / (2.0 * sqrt(ac * bc)));
        a = ac * (1.0 - pow(v / v0, 4) - pow(s_star / s, 2));

        return true;

    }

    void terminate(double simTime) override {}


};



class EnvironmentTest : public ::testing::Test, public Environment {


protected:

    ::sim::Loop sim{};
    Agent *agent = nullptr;

public:

    void SetUp() override {

        // get map path: SIMMAP_SRC has to be provided by cmake
        std::stringstream ss;
        ss << SIMMAP_SRC << "/tests/tracks/CircleR100.xodr";

        // load map
        this->registerMap(ss.str());

        // add virtual horizon as component
        sim.addComponent(this);

        // add agents
        agent = this->createAgent(1, {"1", "-2"});
        agent->setMapPosition("R1-LS2-R2", 42.9203673205, 0.5);

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

    // get map position
    auto mpos = agent->getMapPosition();
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
    agent->setPosition(pos, 10.0);

    // get map position
    auto mpos = agent->getMapPosition();
    EXPECT_EQ("R1-LS2-R2", std::string(mpos.edgeID));
    EXPECT_NEAR(47.9203673205, mpos.longPos, 1e-4);
    EXPECT_NEAR(0.5, mpos.latPos, 1e-4);

}


TEST_F(EnvironmentTest, TrafficSimulation) {

    unsigned int n = 1;

    // init agents
    std::vector<IDM> agents(n);
    for(unsigned int i = 0; i < n; ++i) {

        // add agent
        agents[i].ag = this->createAgent(1, {"1", "-2"});

        // place agent
        double s = (double) i * 10 / (2 * M_PI * 100.0);
        if(s < M_PI * 50.0)
            agent->setMapPosition("R1-LS1-R1", s, 0.0);
        else if(s < M_PI * 100.0)
            agent->setMapPosition("R1-LS2-R1", s - M_PI * 50.0, 0.0);
        else
            agent->setMapPosition("R2-LS1-L1", s - M_PI * 100.0, 0.0);



    }



}