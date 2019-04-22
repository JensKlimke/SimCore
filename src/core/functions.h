//
// Created by Jens Klimke on 2019-03-27.
//

#ifndef SIMCORE_FUNCTIONS_H
#define SIMCORE_FUNCTIONS_H

#include <string>

namespace sim {
namespace fnc {

    /**
     * String formatter
     * @tparam Args
     * @param format Format string
     * @param args Inputs
     * @return Formatted string
     */
    template<typename ... Args>
    std::string string_format(const std::string &format, Args ... args) {
        auto size = snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
        char *buf(new char[size]);
        snprintf(buf, size, format.c_str(), args ...);
        return std::string(buf, buf + size - 1); // We don't want the '\0' inside
    }


}} // namespace ::sim::fnc

#endif //SIMCORE_FUNCTIONS_H
