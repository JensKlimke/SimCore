//
// Created by Jens Klimke on 2019-04-20.
//

#ifndef SIMCORE_ENVIRONMENT_H
#define SIMCORE_ENVIRONMENT_H

#include <core/Model.h>
#include <SimMap/lib.h>
#include <memory>
#include <vector>
#include <map>
#include "Agent.h"


class Environment {


    ::simmap::id_type_t _map_id = 0;

    std::vector<Agent*> _agents;
    std::map<unsigned int, Agent*> _index;


public:


    /**
     * Default constructor
     */
    Environment() = default;


    /**
     * Destructor (clears the map)
     */
    ~Environment();


    /**
     * Register agent
     * @param id ID of the agent
     * @param track Track of the agent
     */
    void createAgent(Agent *agent, unsigned int id, const std::vector<std::string> &track);


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
