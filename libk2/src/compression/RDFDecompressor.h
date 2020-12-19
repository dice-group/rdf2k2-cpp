//
// Created by minimal on 12/18/20.
//

#ifndef RDF2K2_CPP_RDFDECOMPRESSOR_H
#define RDF2K2_CPP_RDFDECOMPRESSOR_H

#include <vector>
#include "../util/LabledMatrix.h"
#include <Dictionary.hpp>
#include <FourSectionDictionary.hpp>

namespace k2 {
    class RDFDecompressor {
    public:
        void readK2(char *in, std::vector<LabledMatrix> &matrices);

        void readDict(char *dictIn, hdt::FourSectionDictionary &dict);

        void writeRDF(char *in, char *out);

    private:
        char *readFile(char *in);

        void writeNTRIPLES(char *out, std::vector<LabledMatrix> &matrices, hdt::FourSectionDictionary &dict);
    };
}

#endif //RDF2K2_CPP_RDFDECOMPRESSOR_H
