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

namespace k2 {

    class IntBasedIndexer {
    public:
        IntBasedIndexer(hdt::PlainDictionary *pDictionary, hdt::Dictionary *pDict);

        void load();

        vector<size_t> *indexTriples(std::shared_ptr<std::vector<k2::DictEntryTriple *>> &triples,
                                   ThreadedKD2TreeSerializer *serializer);

    private:
//        long noOfPredicates;
        hdt::Dictionary *dictionary;
        hdt::PlainDictionary *dict;
    };
}
#endif //RDF2K2_CPP_INTBASEDINDEXER_H