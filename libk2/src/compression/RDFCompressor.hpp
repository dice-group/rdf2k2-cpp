#ifndef RDF2K2_CPP_RDFCOMPRESSOR_H
#define RDF2K2_CPP_RDFCOMPRESSOR_H

#include <Dictionary.hpp>
#include <PlainDictionary.hpp>
#include <FourSectionDictionary.hpp>


class RDFCompressor {
    public: 
        RDFCompressor(bool threaded);
        void compressRDF(char *in, char *out);

    private:
        bool threaded;
        shared_ptr<hdt::PlainDictionary> dict;
        shared_ptr<hdt::FourSectionDictionary> dictionary;
        long readFile(const char *in, hdt::RDFNotation);

        hdt::RDFNotation guessNotation(const char *in);
};

#endif //RDF2K2_CPP_RDFCOMPRESSOR_H