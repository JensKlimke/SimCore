//
// Created by Jens Klimke on 2019-03-27.
//

#ifndef SIMULATION_FRAMEWORK_AGENTSLOGGER_H
#define SIMULATION_FRAMEWORK_AGENTSLOGGER_H


#include <core/functions.h>
#include <core/ISynchronized.h>
#include <models/agent/AgentModel.h>
#include <fstream>
#include <string>
#include <vector>


class AgentsLogger : public sim::ISynchronized {


public:

    struct Agent {
        double *x;
        double *y;
        double *psi;
        double *length;
        double *width;
        double *driverPosX;
        double *driverPosY;
    };



private:

    std::string _filename = "";
    std::vector<Agent> _agents{};

    struct {
        double x0;
        double y0;
        double x1;
        double y1;
    }
    _size{};


public:

    AgentsLogger() = default;

    ~AgentsLogger() override = default;



    bool step(double simTime) override {

        // ignore invalid steps
        if(ISynchronized::step(simTime))
            return true;

        // create file
        std::ofstream file;
        file.open(_filename, std::ofstream::out | std::ofstream::trunc);

        // save time and open object brackets
        file << sim::string_format(R"({"time":%f, "size": {"x0": %f, "y0": %f, "x1": %f, "y1": %f}, "agents": [)",
                simTime, _size.x0, _size.y0, _size.x1, _size.y1);

        // write data
        unsigned int i = 0;
        for(auto &a : _agents) {

            file << sim::string_format(
                    R"({"id": %d, "name": "%d", "position": {"x": %f, "y": %f, "psi": %f}, "size": {"length": %f, "width": %f, "x": %f, "y": %f}})",
                    i, i, *a.x, *a.y, *a.psi, *a.length, *a.width,
                    a.driverPosX == nullptr ? 0.0 : *a.driverPosX, a.driverPosY == nullptr ? 0.0 : *a.driverPosY);

            i++;

        }

        // close object brackets
        file << "]}";

        // close file
        file.close();


        return true;

    }


    void terminate(double simTime) override {

    }


    /**
     * Sets the bounds of the reported area
     * @param x0 Lower x value
     * @param y0 Lower y value
     * @param x1 Upper x value
     * @param y1 Upper y value
     */
    void setSize(double x0, double y0, double x1, double y1) {

        _size = {x0, y0, x1, y1};

    }


    /**
     * Sets the filename of the json file
     * @param filename Filename to be set
     */
    void setFilename(const std::string& filename) {

        _filename = filename;

    }


    /**
     * Adds an agent to the reporter
     * @param agent Agent to be added
     */
    void addAgent(Agent agent) {

        _agents.push_back(agent);

    }


};


#endif //SIMULATION_FRAMEWORK_AGENTSLOGGER_H
