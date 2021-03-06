//
// Created by minimal on 12/14/20.
//

#ifndef RDF2K2_CPP_LOADER_H
#define RDF2K2_CPP_LOADER_H
#include <RDFParser.hpp>
#include <Dictionary.hpp>
#include <memory>
#include <PlainDictionary.hpp>
#include "../util/DictEntryTriple.h"
#include <vector>
#include <memory>
#include "PlainDictionaryPlus.h"

namespace k2 {
    class Loader : public hdt::RDFCallback {

    public:
        Loader(PlainDictionaryPlus *dict, shared_ptr<vector<DictEntryTriple *>> &triplesA);

        void processTriple(const hdt::TripleString &triple, unsigned long long pos) override;

        PlainDictionaryPlus *dict;
        size_t count;

        size_t getCount();

        std::shared_ptr<vector<DictEntryTriple *>> triples;
        size_t timeMSindex=0;
        size_t timeMSpush=0;

    };

}
#endif //RDF2K2_CPP_LOADER_H
