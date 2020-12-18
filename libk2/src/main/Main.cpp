#include <iostream>
#include <string>
#include <string.h>
#include <ctime>
#include <chrono>
#include <filesystem>
#include <fstream>

#include "../compression/RDFCompressor.hpp"
#include "../compression/RDFDecompressor.h"


using namespace std;

void printHelp(){
    cout << "rdf2k2 [-c|-d] options [-kd2|-tkd2] in out" << endl;
	cout << "\t-c\tcompress RDF File" << endl;
	cout << "\t-d\tdecompress GRP File" << endl;
	cout << endl;
	cout << "\t-kd2\t(de)serialize using KD2 TREE" << endl;
	cout << "\t-tkd2\t(de)serialize using Threaded KD2 Tree" << endl;
	cout << endl;
	cout << "\tDecompress Options" << endl;
	cout << "\t-out: N-TRIPLE, TURTLE, RDF/XML - will save output in the format specified" << endl;
}

long getFileSize(char *in){
    return std::filesystem::file_size(in);     
}

void compress(char *in, char *out, bool threaded){
    chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();            
    cout << "Starting compression for file " << in << endl;
    RDFCompressor *compressor = new RDFCompressor(threaded);
    compressor->compressRDF(in, out);
    chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now();     
    chrono::duration<double, milli> time_span = end - start;
    long totalFileSize = getFileSize(out);
    char dictOut[sizeof(out)+5];
    strcpy(dictOut,out);
    strcat(dictOut, ".dict");
    totalFileSize += getFileSize(dictOut);
    double ratio = totalFileSize* 1.0/ getFileSize(in);
    cout << "Finished compression to " << out << "[.dict] took " << time_span.count() << "ms " << " with a " << ratio << " ratio" << endl;
}

void decompress(char *in, char *out, bool threaded, string format){
    chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();
    cout << "Starting decompression for file " << in << endl;
    RDFDecompressor *decompressor = new RDFDecompressor();
    decompressor->writeRDF(in, out);
    chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> time_span = end - start;
    cout << "Finished compression to " << out << "[.dict] took " << time_span.count() << "ms " << endl;
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
        string format = "N-TRIPLE";

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