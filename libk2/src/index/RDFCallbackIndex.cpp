//
// Created by minimal on 12/14/20.
//

#include "RDFCallbackIndex.h"
#include "FourSectionDictionary.hpp"
#include <memory>
#include "../serializer/ThreadedKD2TreeSerializer.hpp"
#include <iostream>
#include <PlainDictionary.hpp>


k2::RDFCallbackIndex::RDFCallbackIndex(hdt::PlainDictionary * dict, ThreadedKD2TreeSerializer *serializer, std::vector<long> *sizeList) {
    this->dict=dict;
    this->serializer = serializer;
    this->sizeList = sizeList;
    this->count =0;

}

long k2::RDFCallbackIndex::getCount() {return count;}

void k2::RDFCallbackIndex::processTriple(const hdt::TripleString &triple, unsigned long long pos) {

    long sID = dict->stringToId(triple.getSubject(), hdt::TripleComponentRole::SUBJECT);
    long pID = dict->stringToId(triple.getPredicate(), hdt::TripleComponentRole::PREDICATE);
    long oID = dict->stringToId(triple.getObject(), hdt::TripleComponentRole::OBJECT);
    serializer->add(sID-1, pID-1, oID-1);
    long add = (*sizeList)[pID-1];
    (*sizeList)[pID-1] = add+1;

    count++;
    if(count%100000==0){
        std::cout << "\rIndexed " << count << " triples.";
    }
}
