//
// Created by minimal on 12/17/20.
//

#include "DictEntryTriple.h"


k2::DictEntryTriple::DictEntryTriple(hdt::DictionaryEntry *subject, hdt::DictionaryEntry *predicate,
                                 hdt::DictionaryEntry *object) {
    this->subject=subject;
    this->predicate=predicate;
    this->object=object;
}

hdt::DictionaryEntry * k2::DictEntryTriple::getObject() const {
    return this->object;
}

hdt::DictionaryEntry * k2::DictEntryTriple::getPredicate() const {
    return this->predicate;
}

hdt::DictionaryEntry * k2::DictEntryTriple::getSubject() const {
    return this->subject;
}

void k2::DictEntryTriple::clear() {
    this->subject= nullptr;
    this->predicate=nullptr;
    this->object=nullptr;
}
