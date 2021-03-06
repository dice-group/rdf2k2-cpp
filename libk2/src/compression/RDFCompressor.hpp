#ifndef RDF2K2_CPP_RDFCOMPRESSOR_H
#define RDF2K2_CPP_RDFCOMPRESSOR_H

#include <Dictionary.hpp>
#include <PlainDictionary.hpp>
#include <FourSectionDictionary.hpp>
#include <RDFParser.hpp>
#include "../util/DictEntryTriple.h"
#include "../dict/PlainDictionaryPlus.h"

namespace k2 {
    class RDFCompressor {
    public:
        RDFCompressor(bool threaded);

        void compressRDF(char *in, char *out);

    private:
        bool threaded;
        PlainDictionaryPlus *dict;
        hdt::Dictionary *dictionary;

        void readFile(const char *in, hdt::RDFNotation, hdt::RDFParserCallback *parser,
                      std::shared_ptr<std::vector<DictEntryTriple *>> &tripleEntries);

        hdt::RDFNotation guessNotation(const char *in) const;
    };
}
#endif //RDF2K2_CPP_RDFCOMPRESSOR_H