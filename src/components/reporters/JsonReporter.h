//
// Created by Jens Klimke on 2019-03-16.
//

#ifndef SIMCORE_JSONREPORTER_H
#define SIMCORE_JSONREPORTER_H

#include <map>
#include <fstream>
#include <string>
#include <core/IComponent.h>
#include <core/Exceptions.h>


class JsonReporter : public sim::IComponent {

    std::ostream *_outstream = nullptr;
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
     * Sets the stream in which the data shall be written
     * @param os Outstream
     */
    void setOutstream(std::ostream &os) {

        _outstream = &os;

    }


protected:


    bool step(double simTime) override {

        // save time and open object brackets
        (*_outstream) << (_hasContent ? ",\n" : "[\n") << "\t" << R"({"time":)" << simTime << R"(,"data":{)";

        // write data
        unsigned int i = 0;
        for(auto &p : _values)
            (*_outstream) << (i++ == 0 ? "" : ",") << "\"" << p.first << "\":" << *p.second;

        // close object brackets
        (*_outstream) << "}}";

        // save that data was already written
        _hasContent = true;

        // success
        return true;

    }


    void initialize(double initTime) override {

        _hasContent = false;

    }


    void terminate(double simTime) override {

        // close brackets
        if(_hasContent)
            (*_outstream) << "\n]" << std::endl;

    }


};


#endif //SIMCORE_JSONREPORTER_H
