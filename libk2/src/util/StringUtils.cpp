//
// Created by minimal on 12/19/20.
//

#include "StringUtils.h"
#include <regex>
#include <string>

 void k2::StringUtils::replace(std::string &str, const std::string &find, const std::string &replace) {
    size_t start_pos = 0;
    while((start_pos = str.find(find, start_pos)) != std::string::npos) {
        str.replace(start_pos, 1, replace);
        start_pos += 2;
    }
}

std::string k2::StringUtils::replaceAll(std::string &str, const std::string &regexStr, const std::string &replace) {
    std::regex regex(regexStr);
    return std::regex_replace(str, regex, replace);
}