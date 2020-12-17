//
// Created by minimal on 12/17/20.
//

#ifndef RDF2K2_CPP_DICTENTRYTRIPLE_H
#define RDF2K2_CPP_DICTENTRYTRIPLE_H

#include <Dictionary.hpp>
#include <PlainDictionary.hpp>

class DictEntryTriple {

public:
    DictEntryTriple(hdt::DictionaryEntry *subject, hdt::DictionaryEntry *predicate, hdt::DictionaryEntry *object);
    hdt::DictionaryEntry * getSubject()const ;
    hdt::DictionaryEntry * getPredicate()const ;
    hdt::DictionaryEntry * getObject()const ;

    void clear();

private:
    hdt::DictionaryEntry *subject;
    hdt::DictionaryEntry *predicate;
    hdt::DictionaryEntry *object;

};


#endif //RDF2K2_CPP_DICTENTRYTRIPLE_H
