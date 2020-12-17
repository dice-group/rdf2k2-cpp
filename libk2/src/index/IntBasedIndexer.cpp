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

vector<long> *IntBasedIndexer::indexTriples(char* rdfFile, ThreadedKD2TreeSerializer *serializer, hdt::RDFNotation notation,hdt::RDFParserCallback *parser){
    //int ret[this->dict.get()->getNpredicates()];


    vector<long> *ret = new vector<long>(this->dict->getNpredicates());
    for(unsigned int i=0;i<dict->getNpredicates(); i++){
        (*ret)[i]=0;
    }
    RDFCallbackIndex callback = RDFCallbackIndex(dict, serializer, ret);
    parser->doParse(rdfFile, "<base>", notation, false, &callback);
    cout << "\rIndexed " << callback.getCount() << " triples in total. " << endl;
    load();

    return ret;
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

