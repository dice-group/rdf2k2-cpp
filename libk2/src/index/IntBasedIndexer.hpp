#ifndef  RDF2K2_CPP_INTBASEDINDEXER_H
#define RDF2K2_CPP_INTBASEDINDEXER_H

#include <filesystem>
#include <fstream>
#include <memory>
#include <PlainDictionary.hpp>
#include <FourSectionDictionary.hpp>
#include "../serializer/ThreadedKD2TreeSerializer.hpp"

using namespace std;

class IntBasedIndexer {
    public:
    IntBasedIndexer(shared_ptr<hdt::PlainDictionary> pDictionary, shared_ptr<hdt::FourSectionDictionary> pDict);
    void load();
    shared_ptr<long[]> indexTriples(char* rdfFile, shared_ptr<ThreadedKD2TreeSerializer> serializer, hdt::RDFNotation);

    private:
        long noOfPredicates;
        shared_ptr<hdt::FourSectionDictionary> dictionary;
        shared_ptr<hdt::PlainDictionary> dict;
};

#endif //RDF2K2_CPP_INTBASEDINDEXER_H