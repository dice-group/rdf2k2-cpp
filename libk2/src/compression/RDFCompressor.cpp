#include <iostream>
#include <ctime>
#include <chrono>
#include <memory>

#include "RDFCompressor.hpp"
#include "../index/IntBasedIndexer.hpp"
#include <Dictionary.hpp>
#include <RDFParser.hpp>
#include <PlainDictionary.hpp>
#include <FourSectionDictionary.hpp>
#include <string.h>

#include "../util/Stats.h"
#include "../dict/Loader.h"
#include "../dict/PlainDictionaryPlus.h"

using namespace std;

RDFCompressor::RDFCompressor(bool threaded) {
    this->threaded = threaded;
    this->dict = new PlainDictionaryPlus();
    this->dictionary = new hdt::FourSectionDictionary();
}


void RDFCompressor::compressRDF(char *in, char *out) {
    printMem("Start: ");
    hdt::RDFNotation notation = guessNotation(in);
    hdt::RDFParserCallback *parser = hdt::RDFParserCallback::getParserCallback(notation);

    chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();


    shared_ptr<vector<DictEntryTriple *>> triples = make_shared<vector<DictEntryTriple *>>();
    readFile(in, notation, parser, triples);
    size_t noOfTriples = triples->size();

    chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> time_span = end - start;
    cout << "Reading took " << time_span.count() << "ms" << endl;
    printMem();


    start = chrono::high_resolution_clock::now();
    IntBasedIndexer index = IntBasedIndexer(dict, dictionary);

    ThreadedKD2TreeSerializer *serializer = new ThreadedKD2TreeSerializer(threaded, dict->getNpredicates(), noOfTriples);
    vector<long> *sizeList = index.indexTriples(triples, in, serializer, notation, parser);
    for(size_t x=0 ; x< triples->size(); x++){
        (*triples)[x]->clear();
        delete (*triples)[x];
    }
    triples->clear();
    triples.reset();
    end = chrono::high_resolution_clock::now();
    time_span = end - start;
    cout << "Indexing took " << time_span.count() << "ms" << endl;
    printMem();

    hdt::ControlInformation ci = hdt::ControlInformation();
    //ci.setType(hdt::ControlInformationType::DICTIONARY);
    char dictOut[strlen(out) + 5];
    strcpy(dictOut, out);
    strcat(dictOut, ".dict");
    ofstream outfile;
    outfile.open(dictOut, ios::out | ios::trunc);
    printMem("Before Dict save: ");
    dictionary->save(outfile, ci);

    delete dictionary;
    outfile.close();
    printMem("After Dict save: ");
    serializer->initSpace(sizeList);
    sizeList->clear();
    delete sizeList;
    start = chrono::high_resolution_clock::now();

    serializer->flush();
    printMem("Serializer Flush: ");
    serializer->serializeMtx(out);
    end = chrono::high_resolution_clock::now();
    delete serializer;
    time_span = end - start;
    cout << "serialization took " << time_span.count() << "ms" << endl;
    printMem("End:");
}

void RDFCompressor::readFile(const char *in, hdt::RDFNotation notation, hdt::RDFParserCallback *parser, shared_ptr<vector<DictEntryTriple *>> &tripleEntries) {
    long triples = 0;
    Loader callback(dict, tripleEntries);
    dict->startProcessing();
    parser->doParse(in, "<http://base.com>", notation, true, &callback);
    triples = callback.getCount();
    cout << "\rLoaded " << triples << " triples in total." << endl;
    dict->stopProcessing();
    //return triples;
}

hdt::RDFNotation RDFCompressor::guessNotation(const char *in) {
    const char *ptr = strrchr(in, '.') + 1;

    if (strcmp(ptr, "nt") == 0) {
        return hdt::RDFNotation::NTRIPLES;
    }
    if (strcmp(ptr, "ttl") == 0) {
        return hdt::RDFNotation::TURTLE;
    }
    if (strcmp(ptr, "n3") == 0) {
        return hdt::RDFNotation::N3;
    }
    if (strcmp(ptr, "json") == 0) {
        return hdt::RDFNotation::JSON;
    }
    if (strcmp(ptr, "rdf") == 0 || strcmp(ptr, "xml") == 0) {
        return hdt::RDFNotation::XML;
    }
    throw std::logic_error{"Notation could not be guessed"};
}