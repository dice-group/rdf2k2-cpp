#include "IntBasedIndexer.hpp"
#include <memory>
#include <RDFParser.hpp>
#include <FourSectionDictionary.hpp>
#include <HDTVocabulary.hpp>


k2::IntBasedIndexer::IntBasedIndexer(hdt::PlainDictionary *pDictionary, hdt::Dictionary *pDict) {
    this->dict= pDictionary;
    this->dictionary = pDict;
}

std::vector<size_t> *k2::IntBasedIndexer::indexTriples(std::shared_ptr<std::vector<k2::DictEntryTriple *>> &triples, ThreadedKD2TreeSerializer *serializer, hdt::RDFNotation notation,hdt::RDFParserCallback *parser){
    std::vector<size_t> *sizeList = new std::vector<size_t>(this->dict->getNpredicates());

    size_t count=0;
    for(const k2::DictEntryTriple *entry: *triples){
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

void k2::IntBasedIndexer::load() {
    if(dictionary->getType()!=hdt::HDTVocabulary::DICTIONARY_TYPE_PLAIN) {
        dictionary->import(dict);
        delete dict;
    }
    else{
        dictionary=dict;
    }
}

