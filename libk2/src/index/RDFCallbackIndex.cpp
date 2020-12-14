//
// Created by minimal on 12/14/20.
//

#include "RDFCallbackIndex.h"
#include "FourSectionDictionary.hpp"
#include <memory>
#include "../serializer/ThreadedKD2TreeSerializer.hpp"

using namespace hdt;


RDFCallbackIndex::RDFCallbackIndex(shared_ptr<FourSectionDictionary> dict, shared_ptr<ThreadedKD2TreeSerializer> serializer, shared_ptr<long[]> sizeList) {
    this->dict=dict;
    this->serializer = serializer;
    this->sizeList = sizeList;
    this->count =0;

}

long RDFCallbackIndex::getCount() {return count;}

void RDFCallbackIndex::processTriple(const TripleString &triple, unsigned long long pos) {
    //TODO blank nodes
    long sID = dict->stringToId(triple.getSubject(), TripleComponentRole::SUBJECT);
    long pID = dict->stringToId(triple.getPredicate(), TripleComponentRole::PREDICATE);
    long oID = dict->stringToId(triple.getObject(), TripleComponentRole::OBJECT);
    serializer->add(sID-1, pID-1, oID-1);
    sizeList[pID-1] = sizeList[pID-1]+1;

    count++;
    if(count%100000==0){
        cout << "\rIndexed " << count << " triples.";
    }
}
