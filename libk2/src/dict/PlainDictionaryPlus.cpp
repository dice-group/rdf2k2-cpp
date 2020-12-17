//
// Created by minimal on 12/17/20.
//

#include "PlainDictionaryPlus.h"
using namespace hdt;

hdt::DictionaryEntry * PlainDictionaryPlus::getEntry(const std::string & str, hdt::TripleComponentRole pos){
    if(str=="") return nullptr;

    if(pos==PREDICATE) {
        DictEntryIt it = hashPredicate.find(str.c_str());
        if(it!=hashPredicate.end()) {
            //cout << "  existing predicate: " << str << endl;
            return it->second;
        } else {
            DictionaryEntry *entry = new DictionaryEntry;
            entry->str = new char [str.length()+1];
            strcpy(entry->str, str.c_str());
            entry->id = predicates.size()+1;
            sizeStrings += str.length();
            //cout << " Add new predicate: " << str.c_str() << endl;

            hashPredicate[entry->str] = entry;
            predicates.push_back(entry);
            return entry;
        }
    }

    DictEntryIt subjectIt = hashSubject.find(str.c_str());
    DictEntryIt objectIt = hashObject.find(str.c_str());

    bool foundSubject = subjectIt!=hashSubject.end();
    bool foundObject = objectIt!=hashObject.end();
    //cout << "A: " << foundSubject << " B: " << foundSubject << endl;

    if(pos==SUBJECT) {
        if( !foundSubject && !foundObject) {
            // Did not exist, create new.
            DictionaryEntry *entry = new DictionaryEntry;
            entry->str = new char [str.length()+1];
            strcpy(entry->str, str.c_str());
            sizeStrings += str.length();

            //cout << " Add new subject: " << str << endl;
            hashSubject[entry->str] = entry;
            return entry;
        } else if(foundSubject) {
            // Already exists in subjects.
            //cout << "   existing subject: " << str << endl;
            return subjectIt->second;
        } else if(foundObject) {
            // Already exists in objects.
            //cout << "   existing subject as object: " << str << endl;
            DictionaryEntry *entry = objectIt->second;
            hashSubject[objectIt->second->str] = entry;
            return entry;
        }
    } else if(pos==OBJECT) {
        if(!foundSubject && !foundObject) {
            // Did not exist, create new.
            DictionaryEntry *entry = new DictionaryEntry;
            entry->str = new char [str.length()+1];
            strcpy(entry->str, str.c_str());
            sizeStrings += str.length();

            //cout << " Add new object: " << str << endl;
            hashObject[entry->str] = entry;
            return entry;
        } else if(foundObject) {
            // Already exists in objects.
            //cout << "     existing object: " << str << endl;
            return objectIt->second;
        } else if(foundSubject) {
            // Already exists in subjects.
            //cout << "     existing object as subject: " << str << endl;
            DictionaryEntry * entry = subjectIt->second;
            hashObject[subjectIt->second->str] = entry ;
            return entry;
        }
    }

    // FIXME: Return inserted index?
    return nullptr;
}
