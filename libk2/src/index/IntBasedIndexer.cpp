#include "IntBasedIndexer.hpp"
#include <memory>
#include <RDFParser.hpp>
#include "RDFCallbackIndex.h"
#include <FourSectionDictionary.hpp>
#include "HDTVocabulary.hpp"

using namespace std;



IntBasedIndexer::IntBasedIndexer(hdt::PlainDictionary *pDictionary, hdt::Dictionary *pDict) {
    this->dict= pDictionary;
    this->dictionary = pDict;
}

vector<long> *IntBasedIndexer::indexTriples(shared_ptr<vector<DictEntryTriple>> &triples, char* rdfFile, ThreadedKD2TreeSerializer *serializer, hdt::RDFNotation notation,hdt::RDFParserCallback *parser){
    //int ret[this->dict.get()->getNpredicates()];
    vector<long> *sizeList = new vector<long>(this->dict->getNpredicates());

    size_t count=0;
    for(const DictEntryTriple &entry: *triples){
        size_t pID = entry.getPredicate()->id;

        serializer->add(entry.getSubject()->id-1, pID-1, entry.getObject()->id-1);
        long add = (*sizeList)[pID-1];
        (*sizeList)[pID-1] = add+1;

        count++;
        if(count%100000==0){
            cout << "\rIndexed " << count << " triples.";
        }
    }


    /*
    for(unsigned int i=0;i<dict->getNpredicates(); i++){
        (*ret)[i]=0;
    }
    cout << dict->subjects[0]->id << endl;
    RDFCallbackIndex callback = RDFCallbackIndex(dict, serializer, ret);
    parser->doParse(rdfFile, "<base>", notation, false, &callback);
    cout << "\rIndexed " << callback.getCount() << " triples in total. " << endl;
     */
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

