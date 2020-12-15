#include "IntBasedIndexer.hpp"
#include <memory>
#include <RDFParser.hpp>
#include "RDFCallbackIndex.h"
#include <FourSectionDictionary.hpp>
#include "HDTVocabulary.hpp"
#include "RDFParserSerd.hpp"

using namespace std;



IntBasedIndexer::IntBasedIndexer(hdt::PlainDictionary *pDictionary, hdt::Dictionary *pDict) {
    this->dict= pDictionary;
    this->dictionary = pDict;
}

vector<long> IntBasedIndexer::indexTriples(char* rdfFile, ThreadedKD2TreeSerializer *serializer, hdt::RDFNotation notation,hdt::RDFParserCallback *parser){
    //int ret[this->dict.get()->getNpredicates()];

    load();

    vector<long> ret = vector<long>(this->dictionary->getNpredicates());
    for(int i=0;i<dictionary->getNpredicates(); i++){
        ret[i]=0;
    }
    RDFCallbackIndex callback = RDFCallbackIndex(dictionary, serializer, &ret);
    parser->doParse(rdfFile, "<base>", notation, false, &callback);
    cout << "\rIndexed " << callback.getCount() << " triples in total. " << endl;

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

