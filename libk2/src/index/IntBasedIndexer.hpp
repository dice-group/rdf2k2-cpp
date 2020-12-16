#ifndef  RDF2K2_CPP_INTBASEDINDEXER_H
#define RDF2K2_CPP_INTBASEDINDEXER_H

#include <filesystem>
#include <fstream>
#include <memory>
#include <PlainDictionary.hpp>
#include <FourSectionDictionary.hpp>
#include "../serializer/ThreadedKD2TreeSerializer.hpp"
#include <RDFParser.hpp>
using namespace std;

class IntBasedIndexer {
    public:
    IntBasedIndexer(hdt::PlainDictionary *pDictionary, hdt::Dictionary * pDict);
    void load();
    vector<long> *indexTriples(char* rdfFile, ThreadedKD2TreeSerializer  *serializer, hdt::RDFNotation, hdt::RDFParserCallback *parser);

    private:
        long noOfPredicates;
        hdt::Dictionary * dictionary;
        hdt::PlainDictionary * dict;
};

#endif //RDF2K2_CPP_INTBASEDINDEXER_H