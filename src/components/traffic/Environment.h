//
// Created by Jens Klimke on 2019-04-20.
//

#ifndef SIMCORE_ENVIRONMENT_H
#define SIMCORE_ENVIRONMENT_H

#include <core/Model.h>
#include <SimMap/lib_wrapper/odrfw.h>
#include <memory>
#include <vector>
#include <map>
#include "Agent.h"


class Environment : public ::sim::IComponent {


    ::simmap::id_type_t _map_id = 0;

    std::vector<std::unique_ptr<Agent>> _agents;
    std::map<unsigned int, Agent*> _index;


public:


    void initialize(double) override;



    bool step(double simTime) override;



    void terminate(double) override;


    /**
     * Register agent
     * @param id ID of the agent
     * @param track Track of the agent
     */
    Agent * createAgent(unsigned int id, const std::vector<std::string> &track = {});


    /**
     * Returns the agent with the given ID
     * @param id ID of the agent
     * @return The agent
     */
    Agent *getAgent(unsigned int id);


    /**
     * Registers a given map
     * @param map Map file to be registered
     */
    void registerMap(const std::string& map);



    /**
     * Returns the map id
     * @return
     */
    unsigned int getMapID() const;



    /**
     * Clears all maps and agents from the map framework
     */
    void clear() const;


};


#endif //SIMCORE_ENVIRONMENT_H
