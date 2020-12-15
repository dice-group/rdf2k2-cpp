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

#include "../dict/Loader.h"

using namespace std;

RDFCompressor::RDFCompressor(bool threaded) {
            this->threaded = threaded;
            this->dict = make_shared<hdt::PlainDictionary>();
            this->dictionary = make_shared<hdt::FourSectionDictionary>();

}


void RDFCompressor::compressRDF(char *in, char *out){

            hdt::RDFNotation notation = guessNotation(in);
            chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();
            long triples = readFile(in, notation);
            cout << "read " << triples << " triples" << endl;
            chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now();            
            chrono::duration<double, milli> time_span = end - start;
            cout << "Reading took " << time_span.count() << "ms" << endl; 
            long noOfPredicates = 0;


            IntBasedIndexer index = IntBasedIndexer(dict, dictionary);

            ThreadedKD2TreeSerializer *serializer = new ThreadedKD2TreeSerializer(threaded,dict->getNpredicates(), triples);
            shared_ptr<long[]> sizeList = index.indexTriples(in, serializer, notation);

            hdt::ControlInformation ci = hdt::ControlInformation();
            ci.setType(hdt::ControlInformationType::DICTIONARY);
            char dictOut[strlen(out)+5];
            strcpy(dictOut, out);
            strcat(dictOut, ".dict");
            ofstream outfile;
            outfile.open(dictOut, ios::out | ios::trunc );
            dictionary->save(outfile, ci);
            outfile.close();
            serializer->initSpace(sizeList);
            serializer->flush();

            serializer->serialize(out);
        }

long RDFCompressor::readFile(const char *in, hdt::RDFNotation notation){
            long triples = 0;
            //TODO guess notation

            hdt::RDFParserCallback *parser = hdt::RDFParserCallback::getParserCallback(notation);
            Loader callback(dict);
            dict.get()->startProcessing();
            parser->doParse(in, "<base>", notation, false, &callback);
            triples = callback.getCount();
            cout << "\rLoaded " << triples << " triples in total." << endl;
            dict->stopProcessing();
            return triples;
        }

        hdt::RDFNotation RDFCompressor::guessNotation(const char *in){
            const char* ptr = strrchr(in, '.')+1;

            if(strcmp(ptr, "nt")==0){
                return hdt::RDFNotation::NTRIPLES;
            }
            if(strcmp(ptr, "ttl")==0){
                return hdt::RDFNotation::TURTLE;
            }
            if(strcmp(ptr, "n3")==0){
                return hdt::RDFNotation::N3;
            }
            if(strcmp(ptr, "json")==0){
                return hdt::RDFNotation::JSON;
            }
            if(strcmp(ptr, "rdf")==0 || strcmp(ptr, "xml")==0){
                return hdt::RDFNotation::XML;
            }
        }