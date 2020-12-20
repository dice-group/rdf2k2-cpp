//
// Created by minimal on 12/19/20.
//

#ifndef RDF2K2_CPP_STRINGUTILS_H
#define RDF2K2_CPP_STRINGUTILS_H

#include <string>
#include <regex>
namespace k2 {
    class StringUtils {
    public:
        static void replace(std::string &str, const std::string &find, const std::string &replace);
        static std::string replaceAll(std::string &str, const std::string &regexStr, const std::string &replace);

    };

}
#endif //RDF2K2_CPP_STRINGUTILS_H
