//
// Created by Jens Klimke on 2019-03-16.
//

#ifndef SIMCORE_JSONFILEREPORTER_H
#define SIMCORE_JSONFILEREPORTER_H

#include "JsonReporter.h"
#include <fstream>


class JsonFileReporter : public JsonReporter {


    std::fstream _fstream;
    std::string _filename;

public:

    JsonFileReporter() = default;

    ~JsonFileReporter() override = default;


    void setFilename(const std::string &name) {

        _filename = name;

    }


protected:



    void initialize(double initTime) override {

        _fstream.open(_filename, std::ios::out);
        setOutstream(_fstream);

        JsonReporter::initialize(initTime);

    }


    void terminate(double simTime) override {

        JsonReporter::terminate(simTime);

        _fstream.close();

    }


};


#endif //SIMCORE_JSONFILEREPORTER_H
