//
// Created by minimal on 12/16/20.
//

#ifndef RDF2K2_CPP_STATS_H
#define RDF2K2_CPP_STATS_H

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <sys/sysinfo.h>


int parseLine(char* line);

int getValue();


long getTotalMemory();

long getVirtMemoryUsed();
void printMem(const std::string &prefix="");

#endif //RDF2K2_CPP_STATS_H
