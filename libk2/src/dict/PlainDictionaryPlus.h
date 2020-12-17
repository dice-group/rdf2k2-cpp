//
// Created by minimal on 12/17/20.
//

#ifndef RDF2K2_CPP_PLAINDICTIONARYPLUS_H
#define RDF2K2_CPP_PLAINDICTIONARYPLUS_H

#include <PlainDictionary.hpp>

class PlainDictionaryPlus : public hdt::PlainDictionary{
public:
    hdt::DictionaryEntry * getEntry(const std::string & str, hdt::TripleComponentRole pos);
};


#endif //RDF2K2_CPP_PLAINDICTIONARYPLUS_H
