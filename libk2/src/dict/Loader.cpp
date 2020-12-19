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


k2::Loader::Loader(PlainDictionaryPlus *dict, std::shared_ptr<vector<DictEntryTriple *>> &triplesA) {
    this->dict = dict;
    this->count=0;
    this->triples=triplesA;
}

void k2::Loader::processTriple(const hdt::TripleString &triple, unsigned long long pos) {
    hdt::DictionaryEntry *se = dict->getEntry(triple.getSubject(), hdt::TripleComponentRole::SUBJECT);
    hdt::DictionaryEntry *pe = dict->getEntry(triple.getPredicate(), hdt::TripleComponentRole::PREDICATE);
    hdt::DictionaryEntry *oe = dict->getEntry(triple.getObject(), hdt::TripleComponentRole::OBJECT);
    DictEntryTriple *tripleEntry = new DictEntryTriple(se, pe, oe);
    this->triples->push_back(tripleEntry);
    count++;
    if(count%1000000==0){
        cout << "\rLoaded " << count << " triples.";
    }
}

long k2::Loader::getCount() {
    return this->count;
}
