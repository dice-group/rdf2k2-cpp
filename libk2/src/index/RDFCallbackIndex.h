//
// Created by minimal on 12/14/20.
//

#ifndef RDF2K2_CPP_RDFCALLBACKINDEX_H
#define RDF2K2_CPP_RDFCALLBACKINDEX_H

#include <RDFParser.hpp>
#include <FourSectionDictionary.hpp>
#include <memory>
#include "../serializer/ThreadedKD2TreeSerializer.hpp"

class RDFCallbackIndex : public hdt::RDFCallback {
public:

    void processTriple(const hdt::TripleString &triple, unsigned long long pos) override;
    RDFCallbackIndex(shared_ptr<hdt::FourSectionDictionary> dict, shared_ptr<ThreadedKD2TreeSerializer> serializer, shared_ptr<long[]> sizeList);
    long getCount();
private:
    shared_ptr<hdt::FourSectionDictionary> dict;
    shared_ptr<ThreadedKD2TreeSerializer> serializer;
    shared_ptr<long[]> sizeList;
    long count;
};


#endif //RDF2K2_CPP_RDFCALLBACKINDEX_H
