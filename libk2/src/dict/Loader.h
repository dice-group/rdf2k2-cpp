//
// Created by minimal on 12/14/20.
//

#ifndef RDF2K2_CPP_LOADER_H
#define RDF2K2_CPP_LOADER_H
#include <RDFParser.hpp>
#include <Dictionary.hpp>
#include <memory>


class Loader : public hdt::RDFCallback {

public:
    Loader(shared_ptr<hdt::ModifiableDictionary> dict1);
    void processTriple(const hdt::TripleString &triple, unsigned long long pos) override;
    shared_ptr<hdt::ModifiableDictionary> dict;
    long count;
    long getCount();
};


#endif //RDF2K2_CPP_LOADER_H
