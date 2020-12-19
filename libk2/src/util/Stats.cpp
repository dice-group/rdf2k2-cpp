#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>
#include <sys/sysinfo.h>
#include "Stats.h"

int parseLine(char* line){
    // This assumes that a digit will be found and the line ends in " Kb".
    int i = strlen(line);
    const char* p = line;
    while (*p <'0' || *p > '9') p++;
    line[i-3] = '\0';
    i = atoi(p);
    return i;
}

int getValue(){ //Note: this value is in KB!
    FILE* file = fopen("/proc/self/status", "r");
    int result = -1;
    char line[128];

    while (fgets(line, 128, file) != NULL){
        if (strncmp(line, "VmSize:", 7) == 0){
            result = parseLine(line);
            break;
        }
    }
    fclose(file);
    return result;
}


long getTotalMemory(){
    struct sysinfo memInfo;

    sysinfo (&memInfo);
    long long totalVirtualMem = memInfo.totalram;
    return totalVirtualMem;
}

long getVirtMemoryUsed(){
    struct sysinfo memInfo;

    sysinfo (&memInfo);
    long long virtualMemUsed = memInfo.totalram - memInfo.freeram;

    return virtualMemUsed;
}

void printMem(const std::string& prefix){
    long currentUsedVirtMemory = getVirtMemoryUsed();
    long totalVirtMemory = getTotalMemory();
    int procMem = getValue();
    long availableVirtMemory = (totalVirtMemory-currentUsedVirtMemory)/1024 + procMem;
    std::cout << prefix << "Memory Usage: " << procMem << " kb / " << availableVirtMemory << " kb [" << (totalVirtMemory/1024) << " kb total]" << std::endl;
}