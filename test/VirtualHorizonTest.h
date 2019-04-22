//
// Created by Jens Klimke on 2019-04-20.
//

#ifndef SIMCORE_VIRTUALHORIZONTEST_H
#define SIMCORE_VIRTUALHORIZONTEST_H

#include <gtest/gtest.h>
#include <components/environment/VirtualHorizon.h>

class VirtualHorizonTest : public ::testing::Test, public VirtualHorizon {


protected:

    ::sim::Loop sim{};

public:

    void SetUp() override {

        // load map
        this->registerMap("tracks/CircleR100.xodr");

        // add virtual horizon as component
        sim.addComponent(this);

        // add agents
        this->registerSimAgent(1, {"1", "-2"});

    }


    void TearDown() override {

        // clear all
        this->clear();

    }

};

TEST_F(VirtualHorizonTest, MapLoaded) {

    EXPECT_EQ(1, this->getMapId());

}

TEST_F(VirtualHorizonTest, RegisterAgents) {



}


#endif //SIMCORE_VIRTUALHORIZONTEST_H
