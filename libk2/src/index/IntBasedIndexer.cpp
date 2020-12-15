#include "IntBasedIndexer.hpp"
#include <memory>
#include <RDFParser.hpp>
#include "RDFCallbackIndex.h"
#include <FourSectionDictionary.hpp>
#include "HDTVocabulary.hpp"

using namespace std;



IntBasedIndexer::IntBasedIndexer(shared_ptr<hdt::PlainDictionary> pDictionary, shared_ptr<hdt::FourSectionDictionary> pDict) {
    this->dict= pDictionary;
    this->dictionary = pDict;
}

shared_ptr<long[]> IntBasedIndexer::indexTriples(char* rdfFile, ThreadedKD2TreeSerializer *serializer, hdt::RDFNotation notation){
    //int ret[this->dict.get()->getNpredicates()];
    shared_ptr<long[]> ret = make_shared<long[]>(this->dict.get()->getNpredicates());

    for(int i=0;i<dict->getNpredicates(); i++){
        ret[i]=0;
    }
    load();

    hdt::RDFParserCallback *parser = hdt::RDFParserCallback::getParserCallback(notation);
    RDFCallbackIndex callback = RDFCallbackIndex(dictionary, serializer, ret);
    parser->doParse(rdfFile, "<base>", notation, false, &callback);
    cout << "\rIndexed " << callback.getCount() << " triples in total. " << endl;

    return ret;
}

void IntBasedIndexer::load() {
    dictionary.get()->import(dict.get());
    dict.reset();
}

