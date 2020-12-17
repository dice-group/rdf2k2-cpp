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

using namespace hdt;

Loader::Loader(PlainDictionaryPlus *dict, std::shared_ptr<vector<DictEntryTriple *>> &triplesA) {
    this->dict = dict;
    this->count=0;
    this->triples=triplesA;
}

void Loader::processTriple(const TripleString &triple, unsigned long long pos) {
    //TODO get DictEntry directly -> change hdt code
    //TODO check if the IDs later are the actual one or if we need to calculate the correct id. very likely
    DictionaryEntry *se = dict->getEntry(triple.getSubject(), TripleComponentRole::SUBJECT);
    DictionaryEntry *pe = dict->getEntry(triple.getPredicate(), TripleComponentRole::PREDICATE);
    DictionaryEntry *oe = dict->getEntry(triple.getObject(), TripleComponentRole::OBJECT);
    DictEntryTriple *tripleEntry = new DictEntryTriple(se, pe, oe);
    this->triples->push_back(tripleEntry);
    count++;
    if(count%100000==0){
        cout << "\rLoaded " << count << " triples.";
    }
}

long Loader::getCount() {
    return this->count;
}
