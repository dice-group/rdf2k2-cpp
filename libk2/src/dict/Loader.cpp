//
// Created by minimal on 12/14/20.
//

#include "Loader.h"
#include "../util/DictEntryTriple.h"
#include <memory>
#include <string.h>
#include <vector>
#include <memory>
#include "PlainDictionaryPlus.h"
#include <chrono>

k2::Loader::Loader(PlainDictionaryPlus *dict, std::shared_ptr<vector<DictEntryTriple *>> &triplesA) {
    this->dict = dict;
    this->count=0;
    this->triples=triplesA;
}

void k2::Loader::processTriple(const hdt::TripleString &triple, unsigned long long pos) {
    std::chrono::high_resolution_clock::time_point p1 = std::chrono::high_resolution_clock::now();

    hdt::DictionaryEntry *se = dict->getEntry(triple.getSubject(), hdt::TripleComponentRole::SUBJECT);
    hdt::DictionaryEntry *pe = dict->getEntry(triple.getPredicate(), hdt::TripleComponentRole::PREDICATE);
    hdt::DictionaryEntry *oe = dict->getEntry(triple.getObject(), hdt::TripleComponentRole::OBJECT);
    DictEntryTriple *tripleEntry = new DictEntryTriple(se, pe, oe);
    std::chrono::high_resolution_clock::time_point p2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, milli> time_span = p2 - p1;
    this->timeMSindex+=time_span.count();
    this->triples->push_back(tripleEntry);
    count++;
    p1 = std::chrono::high_resolution_clock::now();
    time_span = p1 - p2;
    this->timeMSpush+=time_span.count();

    if(count%1000000==0){
        cout << "\rLoaded " << count << " triples.";
    }
}

size_t k2::Loader::getCount() {
    return this->count;
}
