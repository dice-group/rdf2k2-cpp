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
#include "../io/RDFSimpleParser.h"

k2::RDFCompressor::RDFCompressor(bool threaded) {
    this->threaded = threaded;
    this->dict = new PlainDictionaryPlus();
    this->dictionary = new hdt::FourSectionDictionary();
}


void k2::RDFCompressor::compressRDF(char *in, char *out) {
    printMem("Start: ");
    hdt::RDFNotation notation = guessNotation(in);
    auto *parser = hdt::RDFParserCallback::getParserCallback(notation);
    //hdt::RDFParserCallback *parser = new RDFSimpleParser();
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();


    std::shared_ptr<vector<DictEntryTriple *>> triples = std::make_shared<vector<DictEntryTriple *>>();
    readFile(in, notation, parser, triples);
    size_t noOfTriples = triples->size();

    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, milli> time_span = end - start;
    std::cout << "Reading took " << time_span.count() << "ms" << std::endl;
    printMem();


    start = std::chrono::high_resolution_clock::now();
    IntBasedIndexer index = IntBasedIndexer(dict, dictionary);

    ThreadedKD2TreeSerializer *serializer = new ThreadedKD2TreeSerializer(threaded, dict->getNpredicates(), noOfTriples);
    std::vector<size_t> *sizeList = index.indexTriples(triples, serializer);
    for(size_t x=0 ; x< triples->size(); x++){
        (*triples)[x]->clear();
        delete (*triples)[x];
    }
    triples->clear();
    triples.reset();
    end = std::chrono::high_resolution_clock::now();
    time_span = end - start;
    std::cout << "Indexing took " << time_span.count() << "ms" << std::endl;
    printMem();

    hdt::ControlInformation ci = hdt::ControlInformation();
    //ci.setType(hdt::ControlInformationType::DICTIONARY);
    char dictOut[strlen(out) + 5];
    strcpy(dictOut, out);
    strcat(dictOut, ".dict");
    std::ofstream outfile;
    outfile.open(dictOut, ios::out | ios::trunc);
    printMem("Before Dict save: ");
    dictionary->save(outfile, ci);

    delete dictionary;
    outfile.close();
    printMem("After Dict save: ");
    serializer->initSpace(sizeList);
    sizeList->clear();
    delete sizeList;
    start = std::chrono::high_resolution_clock::now();

    serializer->flush();
    printMem("Serializer Flush: ");
    serializer->serializeMtx(out);
    end = std::chrono::high_resolution_clock::now();
    delete serializer;
    time_span = end - start;
    std::cout << "serialization took " << time_span.count() << "ms" << std::endl;
    printMem("End:");
}

void k2::RDFCompressor::readFile(const char *in, hdt::RDFNotation notation, hdt::RDFParserCallback *parser, std::shared_ptr<std::vector<DictEntryTriple *>> &tripleEntries) {
    size_t triples = 0;
    k2::Loader callback(dict, tripleEntries);
    std::chrono::high_resolution_clock::time_point p1 = std::chrono::high_resolution_clock::now();
    dict->startProcessing();
    parser->doParse(in, "<http://base.com>", notation, true, &callback);
    triples = callback.getCount();
    std::cout << "\rLoaded " << triples << " triples in total." << std::endl;
    std::chrono::high_resolution_clock::time_point p2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, milli> time_span = p2 - p1;
    std::cout << "reading and temp indexing took " << time_span.count() << "ms [indexing: " << callback.timeMSindex << "ms, push: " <<callback.timeMSpush << "ms]" << std::endl;
    dict->stopProcessing();
    p1 = std::chrono::high_resolution_clock::now();
    time_span = p1 - p2;
    std::cout << "sorting index took " << time_span.count() << "ms" << std::endl;

    //return triples;
}

hdt::RDFNotation k2::RDFCompressor::guessNotation(const char *in) const{
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