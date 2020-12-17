//
// Created by minimal on 12/17/20.
//

#include "DictEntryTriple.h"


DictEntryTriple::DictEntryTriple(hdt::DictionaryEntry *subject, hdt::DictionaryEntry *predicate,
                                 hdt::DictionaryEntry *object) {
    this->subject=subject;
    this->predicate=predicate;
    this->object=object;
}

hdt::DictionaryEntry * DictEntryTriple::getObject() const {
    return this->object;
}

hdt::DictionaryEntry * DictEntryTriple::getPredicate() const {
    return this->predicate;
}

hdt::DictionaryEntry * DictEntryTriple::getSubject() const {
    return this->subject;
}

void DictEntryTriple::clear() {
    this->subject= nullptr;
    this->predicate=nullptr;
    this->object=nullptr;
}
