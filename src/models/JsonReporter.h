//
// Created by Jens Klimke on 2019-03-16.
//

#ifndef SIMULATION_FRAMEWORK_JSONREPORTER_H
#define SIMULATION_FRAMEWORK_JSONREPORTER_H

#include <map>
#include <fstream>
#include <string>
#include <core/IModel.h>
#include <core/Exceptions.h>


class JsonReporter : public sim::IModel {

    std::string _filename = "";
    std::ofstream _file;
    std::map<std::string, const double*> _values{};

    bool _hasContent = false;




public:

    /**
     * Constructor
     */
    JsonReporter() = default;


    /**
     * Adds an double value to be added to the
     * @param val Pointer to the value
     * @param key Key to be used in json
     */
    void addValue(const std::string &key, const double *val) {
        _values[key] = val;
    }


    /**
     * Sets the filename of the json file
     * @param filename Filename to be set
     */
    void setFilename(const std::string& filename) {

        _filename = filename;

    }


protected:


    bool step(double simTime) override {

        // save time and open object brackets
        _file << (_hasContent ? ",\n" : "[\n") << "\t" << R"({"time":)" << simTime << R"(,"data":{)";

        // write data
        unsigned int i = 0;
        for(auto &p : _values)
            _file << (i++ == 0 ? "" : ",") << "\"" << p.first << "\":" << *p.second;

        // close object brackets
        _file << "}}";

        // save that data was already written
        _hasContent = true;

        // success
        return true;

    }


    void initialize(double initTime) override {

        // open file
        _file.open(_filename, std::ofstream::out | std::ofstream::trunc);
        _hasContent = false;

    }


    void terminate(double simTime) override {

        // close brackets
        if(_hasContent)
            _file << "\n]" << std::endl;

        // close file
        _file.close();

    }


};


#endif //SIMULATION_FRAMEWORK_JSONREPORTER_H
