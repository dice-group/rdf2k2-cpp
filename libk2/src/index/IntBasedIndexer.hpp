#ifndef  RDF2K2_CPP_INTBASEDINDEXER_H
#define RDF2K2_CPP_INTBASEDINDEXER_H

#include <filesystem>
#include <fstream>
#include <memory>
#include <PlainDictionary.hpp>
#include <FourSectionDictionary.hpp>
#include "../serializer/ThreadedKD2TreeSerializer.hpp"
#include "../util/DictEntryTriple.h"
#include <RDFParser.hpp>

class IntBasedIndexer {
    public:
    IntBasedIndexer(hdt::PlainDictionary *pDictionary, hdt::Dictionary * pDict);
    void load();
    vector<long> *indexTriples(std::shared_ptr<std::vector<DictEntryTriple *>> &triples, char* rdfFile, ThreadedKD2TreeSerializer  *serializer, hdt::RDFNotation, hdt::RDFParserCallback *parser);

    private:
//        long noOfPredicates;
        hdt::Dictionary * dictionary;
        hdt::PlainDictionary * dict;
};

#endif //RDF2K2_CPP_INTBASEDINDEXER_H