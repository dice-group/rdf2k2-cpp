//
// Created by minimal on 12/16/20.
//

#ifndef RDF2K2_CPP_STATS_H
#define RDF2K2_CPP_STATS_H

#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include <iostream>
#include <sys/sysinfo.h>

using namespace std;


int parseLine(char* line);

int getValue();


long getTotalMemory();

long getVirtMemoryUsed();
void printMem(char * prefix="");

#endif //RDF2K2_CPP_STATS_H
