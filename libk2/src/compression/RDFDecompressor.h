//
// Created by minimal on 12/18/20.
//

#ifndef RDF2K2_CPP_RDFDECOMPRESSOR_H
#define RDF2K2_CPP_RDFDECOMPRESSOR_H

#include <vector>
#include "../util/LabledMatrix.h"
#include <Dictionary.hpp>

class RDFDecompressor {
public:
    void readK2(char *in, std::vector<LabledMatrix>& matrices);
    hdt::Dictionary& readDict(char *dictIn);
    void writeRDF(char *in, char* out);
private:
    char *readFile(char *in);

};


#endif //RDF2K2_CPP_RDFDECOMPRESSOR_H
