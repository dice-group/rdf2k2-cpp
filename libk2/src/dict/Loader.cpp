//
// Created by minimal on 12/14/20.
//

#include "Loader.h"
#include <memory>

using namespace hdt;

Loader::Loader( hdt::ModifiableDictionary *dict) {
    this->dict = dict;
    this->count=0;
}

void Loader::processTriple(const TripleString &triple, unsigned long long pos) {
    //TODO BNodes
    dict->insert(triple.getSubject(), TripleComponentRole::SUBJECT);
    dict->insert(triple.getPredicate(), TripleComponentRole::PREDICATE);
    dict->insert(triple.getObject(), TripleComponentRole::OBJECT);

    count++;
    if(count%100000==0){
        cout << "\rLoaded " << count << " triples.";
    }
}

long Loader::getCount() {
    return this->count;
}