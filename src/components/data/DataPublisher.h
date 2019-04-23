//
// Created by Jens Klimke on 2019-04-22.
//

#ifndef SIMCORE_DATAPUBLISHER_H
#define SIMCORE_DATAPUBLISHER_H

#include <core/functions.h>
#include <core/ISynchronized.h>
#include <core/IComponent.h>
#include <curl/curl.h>
#include <sstream>
#include <iostream>


class DataPublisher : public ::sim::ISynchronized, public ::sim::data::DataManager {


    std::string _url = "http://localhost:9001?pipe=sim::data";


public:

    void initialize(double initTime) override {

        // initialize curl
        curl_global_init(CURL_GLOBAL_ALL);

        // send command to cleanup
        send("&reset=true", "");

    }


    bool step(double simTime) override {


        if(::sim::ISynchronized::step(simTime)) {

            // create string stream and stream data content into it
            std::stringstream os;
            os << *this;

            // send content
            return send("", os.str());

        }

        return true;

    }


    void terminate(double simTime) override {

        // cleanup globally
        curl_global_cleanup();

    }


protected:


    bool send(const std::string &get, const std::string &post) {

        // curl variables
        CURL *curl;
        CURLcode res;

        // success flag
        bool success;

        // init curl
        curl = curl_easy_init();
        success = curl != nullptr;

        // main process
        if (success) {

            // shut up, but log output
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, logResponse);

            // set url
            auto url = _url + get;
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

            // specify post data
            if(!post.empty())
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post.c_str());

            // preform post
            res = curl_easy_perform(curl);

            // check result
            if (res != CURLE_OK)
                throw std::runtime_error(curl_easy_strerror(res));

        }

        // clean up
        curl_easy_cleanup(curl);

        // return true
        return success;

    }


    static size_t logResponse(void *buffer, size_t size, size_t nmemb, void *userp) {

        return size * nmemb;

    }


};


#endif //SIMCORE_DATAPUBLISHER_H
