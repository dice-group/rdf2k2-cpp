//
// Created by minimal on 12/20/20.
//

#ifndef RDF2K2_CPP_RDFSIMPLEPARSER_H
#define RDF2K2_CPP_RDFSIMPLEPARSER_H

#include <RDFParser.hpp>
namespace k2 {
    class RDFSimpleParser : public hdt::RDFParserCallback {
        virtual void doParse(const char *fileName, const char *baseUri, hdt::RDFNotation notation, bool ignoreErrors,
                             hdt::RDFCallback *callback) override;

    };
}

#endif //RDF2K2_CPP_RDFSIMPLEPARSER_H
