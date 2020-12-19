//
// Created by minimal on 12/14/20.
//

#ifndef RDF2K2_CPP_RDFCALLBACKINDEX_H
#define RDF2K2_CPP_RDFCALLBACKINDEX_H

#include <RDFParser.hpp>
#include <FourSectionDictionary.hpp>
#include <memory>
#include "../serializer/ThreadedKD2TreeSerializer.hpp"
#include <PlainDictionary.hpp>

class RDFCallbackIndex : public hdt::RDFCallback {
public:

    void processTriple(const hdt::TripleString &triple, unsigned long long pos) override;
    RDFCallbackIndex(hdt::PlainDictionary *dict, ThreadedKD2TreeSerializer *serializer, std::vector<long> *sizeList);
    long getCount();
private:
    hdt::PlainDictionary * dict;
    ThreadedKD2TreeSerializer *serializer;
    std::vector<long> *sizeList;
    long count;
};


#endif //RDF2K2_CPP_RDFCALLBACKINDEX_H
