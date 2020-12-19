//
// Created by minimal on 12/19/20.
//

#include "RDFTools.h"
#include "StringUtils.h"

std::string k2::RDFTools::getTerm(std::string term){
    if(term.starts_with("\"")){
        auto last =term.find_last_of("\"")-1;
        std::string test = term.substr(1, last);
        if(test.find("\"")!=test.npos){
            test;
        }
        k2::StringUtils::replace(test, "\\", "\\\\");
        k2::StringUtils::replace(test, "\"", "\\\"");
        k2::StringUtils::replace(test, "\n", "\\n");
        std::string newTerm = "\"" + test + term.substr(last+1);
        return newTerm;
    }
    else if(term.starts_with("_")){
        return term;
    }
    else{
        return "<" + term + ">";
    }
}