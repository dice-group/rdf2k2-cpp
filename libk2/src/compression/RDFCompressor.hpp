#ifndef RDF2K2_CPP_RDFCOMPRESSOR_H
#define RDF2K2_CPP_RDFCOMPRESSOR_H

#include <Dictionary.hpp>
#include <PlainDictionary.hpp>
#include <FourSectionDictionary.hpp>
#include <RDFParser.hpp>

class RDFCompressor {
    public: 
        RDFCompressor(bool threaded);
        void compressRDF(char *in, char *out);

    private:
        bool threaded;
        hdt::PlainDictionary *dict;
        hdt::Dictionary *dictionary;
        long readFile(const char *in, hdt::RDFNotation, hdt::RDFParserCallback *parser);

        hdt::RDFNotation guessNotation(const char *in);
};

#endif //RDF2K2_CPP_RDFCOMPRESSOR_H