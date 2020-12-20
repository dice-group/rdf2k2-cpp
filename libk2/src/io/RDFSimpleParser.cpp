//
// Created by minimal on 12/20/20.
//
#ifdef BOOST_LIBRARY

#include <boost/filesystem/fstream.hpp>
#include <string>
#include <boost/algorithm/string.hpp>
#include "RDFSimpleParser.h"
void k2::RDFSimpleParser::doParse(const char *fileName, const char *baseUri, hdt::RDFNotation notation, bool ignoreErrors, hdt::RDFCallback *callback){
    boost::filesystem::ifstream is(fileName);
    std::string line;
    size_t count=0;
    while (std::getline(is, line)) {
        hdt::TripleString triple{};
        triple.read(line);
        callback->processTriple(triple, count++);
    }
    is.close();
}
#endif