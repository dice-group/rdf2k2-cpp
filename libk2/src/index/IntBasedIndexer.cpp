#include "IntBasedIndexer.hpp"
#include <memory>
#include <RDFParser.hpp>
#include "RDFCallbackIndex.h"
#include <FourSectionDictionary.hpp>
#include <HDTVocabulary.hpp>


IntBasedIndexer::IntBasedIndexer(hdt::PlainDictionary *pDictionary, hdt::Dictionary *pDict) {
    this->dict= pDictionary;
    this->dictionary = pDict;
}

std::vector<long> *IntBasedIndexer::indexTriples(std::shared_ptr<std::vector<DictEntryTriple *>> &triples, char* rdfFile, ThreadedKD2TreeSerializer *serializer, hdt::RDFNotation notation,hdt::RDFParserCallback *parser){
    std::vector<long> *sizeList = new std::vector<long>(this->dict->getNpredicates());

    size_t count=0;
    for(const DictEntryTriple *entry: *triples){
        size_t pID = entry->getPredicate()->id;

        serializer->add(entry->getSubject()->id-1, pID-1, entry->getObject()->id-1);
        long add = (*sizeList)[pID-1];
        (*sizeList)[pID-1] = add+1;

        count++;
        if(count%100000==0){
            std::cout << "\rIndexed " << count << " triples.";
        }
    }

    std::cout << "\rIndexed " << count<< " triples in total. " << std::endl;

    load();

    return sizeList;
}

void IntBasedIndexer::load() {
    if(dictionary->getType()!=hdt::HDTVocabulary::DICTIONARY_TYPE_PLAIN) {
        dictionary->import(dict);
        delete dict;
    }
    else{
        dictionary=dict;
    }
}

