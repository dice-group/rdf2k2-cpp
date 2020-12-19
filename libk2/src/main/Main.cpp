#include <iostream>
#include <string>
#include <string.h>
#include <ctime>
#include <chrono>
#include <filesystem>
#include <fstream>

#include "../compression/RDFCompressor.hpp"
#include "../compression/RDFDecompressor.h"


void printHelp(){
    std::cout << "rdf2k2 [-c|-d] options [-kd2|-tkd2] in out" << std::endl;
    std::cout << "\t-c\tcompress RDF File" << std::endl;
    std::cout << "\t-d\tdecompress GRP File" << std::endl;
    std::cout << std::endl;
    std::cout << "\t-kd2\t(de)serialize using KD2 TREE" << std::endl;
    std::cout << "\t-tkd2\t(de)serialize using Threaded KD2 Tree" << std::endl;
    std::cout << std::endl;
    std::cout << "\tDecompress Options" << std::endl;
    std::cout << "\t-out: N-TRIPLE, TURTLE, RDF/XML - will save output in the format specified" << std::endl;
}

long getFileSize(char *in){
    return std::filesystem::file_size(in);     
}

void compress(char *in, char *out, bool threaded){
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    std::cout << "Starting compression for file " << in << std::endl;
    RDFCompressor *compressor = new RDFCompressor(threaded);
    compressor->compressRDF(in, out);
    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, milli> time_span = end - start;
    long totalFileSize = getFileSize(out);
    char dictOut[sizeof(out)+5];
    strcpy(dictOut,out);
    strcat(dictOut, ".dict");
    totalFileSize += getFileSize(dictOut);
    double ratio = totalFileSize* 1.0/ getFileSize(in);
    std::cout << "Finished compression to " << out << "[.dict] took " << time_span.count() << "ms " << " with a " << ratio << " ratio" << std::endl;
}

void decompress(char *in, char *out, bool threaded, string format){
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    cout << "Starting decompression for file " << in << std::endl;
    RDFDecompressor *decompressor = new RDFDecompressor();
    decompressor->writeRDF(in, out);
    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, milli> time_span = end - start;
    std::cout << "Finished decompression to " << out << " took " << time_span.count() << "ms " << std::endl;
}

int main(int argc, char *argv[]){
    if(argc<5){
        printHelp();
        return 0;
    }
    bool threaded = false;

    if(strcmp(argv[1], "-c") == 0){
        bool threaded = false;
        if(strcmp(argv[2], "-tkd2") == 0){
                        

            threaded=true;
        }
        else if(strcmp(argv[2], "-kd2") == 0){
            threaded=false;
        }
        else{
            printHelp();
            return 0;
        }
        compress(argv[3], argv[4], threaded);
    }
    else if(strcmp(argv[1], "-d") == 0){
        int x = 2;
        std::string format = "N-TRIPLE";

        if(strcmp(argv[x], "-out") == 0){
            x+=2;
            format = argv[x+1];
        }
        if(strcmp(argv[x], "-tkd2") == 0){
            threaded=true;
        }
        else if(strcmp(argv[x], "-tdk2") == 0){
            threaded=false;
        }
        else{
            printHelp();
            return 0;
        }
        decompress(argv[x+1], argv[x+2], threaded, format);
    }
    else{
        printHelp();
    }

    return 0;
}