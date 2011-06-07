/*
 * StringUtils.cpp
 *
 *  Created on: Jun 1, 2011
 *      Author: wrightjb
 */

#include <Utils/StringUtils.h>

using namespace CartWheel;
using namespace CartWheel::Util;

StringUtils::StringUtils() {}

StringUtils::~StringUtils() {}

std::vector<std::string> & StringUtils::split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> StringUtils::split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    return split(s, delim, elems);
}

