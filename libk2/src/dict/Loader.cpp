//
// Created by minimal on 12/14/20.
//

#include "Loader.h"
#include "../util/DictEntryTriple.h"
#include <memory>
#include <string.h>
#include <vector>
#include <memory>

using namespace hdt;

Loader::Loader(hdt::PlainDictionary *dict, std::shared_ptr<vector<DictEntryTriple>> &triplesA) {
    this->dict = dict;
    this->count=0;
    this->triples=triplesA;
}

void Loader::processTriple(const TripleString &triple, unsigned long long pos) {
    dict->insert(triple.getSubject(), TripleComponentRole::SUBJECT);
    dict->insert(triple.getPredicate(), TripleComponentRole::PREDICATE);
    dict->insert(triple.getObject(), TripleComponentRole::OBJECT);
    dict->hashSubject[triple.getSubject().c_str()];
    DictEntryTriple tripleEntry{dict->hashSubject[triple.getSubject().c_str()],
                                dict->hashPredicate[triple.getPredicate().c_str()],
                                dict->hashObject[triple.getObject().c_str()]};
    this->triples->push_back(tripleEntry);
    count++;
    if(count%100000==0){
        cout << "\rLoaded " << count << " triples.";
    }
}

long Loader::getCount() {
    return this->count;
}
