//
// Created by Jens Klimke on 2019-04-20.
//

#ifndef SIMCORE_VIRTUALHORIZONTEST_H
#define SIMCORE_VIRTUALHORIZONTEST_H


#include <gtest/gtest.h>
#include <components/environment/VirtualHorizon.h>

class VirtualHorizonTest : public ::testing::Test, public VirtualHorizon {

public:

    void SetUp() override {

        // load map
        this->registerMap("./track/CircleR100.xodr");

    }

};

TEST_F(VirtualHorizonTest, MapLoaded) {

    EXPECT_EQ(1, this->getMapId());

}


#endif //SIMCORE_VIRTUALHORIZONTEST_H
