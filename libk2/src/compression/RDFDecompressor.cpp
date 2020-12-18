//
// Created by minimal on 12/18/20.
//

#include "RDFDecompressor.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <FourSectionDictionary.hpp>

#include "../util/Path.h"

//read the whole file in mem, not sure if its faster though.
char *RDFDecompressor::readFile(char *in) {
    char * memblock;
    std::streampos size;
    std::ifstream file (in, std::ios::in|std::ios::binary|std::ios::ate);
    if (file.is_open())
    {
        size = file.tellg();
        memblock = new char [size];
        file.seekg (0, std::ios::beg);
        file.read (memblock, size);
        file.close();
    }
    return memblock;
}

void RDFDecompressor::writeRDF(char *in, char* out){
    std::vector<LabledMatrix> matrices{};
    readK2(in, matrices);
    hdt::FourSectionDictionary dict{};
    char dictIn[strlen(out) + 5];
    strcpy(dictIn, in);
    strcat(dictIn, ".dict");
    readDict(dictIn, dict);
    //TODO for now just use ntriples, but later on we want to use SERD
    writeNTRIPLES(out, matrices, dict);
}

void RDFDecompressor::writeNTRIPLES(char *out, std::vector<LabledMatrix> &matrices, hdt::FourSectionDictionary &dict){
    std::ofstream output;
    output.open(out, ios::out | ios::trunc );

    for(LabledMatrix &matrix: matrices) {

        std::string predicate = dict.idToString(matrix.getLabel()+1, hdt::TripleComponentRole::PREDICATE);
        for(const Point &p : matrix.getPoints()) {
            std::string subject = dict.idToString(p.getRow()+1, hdt::TripleComponentRole::SUBJECT);
            std::string object = dict.idToString(p.getCol()+1, hdt::TripleComponentRole::OBJECT);

            output << "<" << subject << "> <" << predicate << "> " << object << "." << endl;
        }
    }
    output.close();
}


void  RDFDecompressor::readDict(char *dictIn, hdt::FourSectionDictionary &dict){
    //std::ifstream file{dictIn, std::ios::in|std::ios::binary|std::ios::ate};
    //std::istream& s   = file;
    hdt::ControlInformation ci =hdt::ControlInformation();
    ci.setFormat(dict.getType());

    unsigned char * memblock;
    std::streampos size;
    std::ifstream file (dictIn, std::ios::in|std::ios::binary|std::ios::ate);
    if (file.is_open())
    {
        size = file.tellg();
        memblock = new unsigned char [size];
        file.seekg (0, std::ios::beg);
        file.read ((char *)memblock, size);
        file.close();
    }
    dict.load(memblock, memblock+size);
    file.close();
}


void RDFDecompressor::readK2(char *in, std::vector<LabledMatrix>& matrices){
    std::ifstream file (in, std::ios::in|std::ios::binary|std::ios::ate);
    std::streampos size;

    if (file.is_open()) {
        //read long, read h
        size = file.tellg();
        file.seekg(0, std::ios::beg);

        bool matrixEnd=false;
        char pLabel[sizeof(long)];
        size_t count=0;
        while (count<size){
            matrixEnd=false;
            file.read((char *) &pLabel, sizeof(pLabel));
            count+=sizeof(pLabel);
            long label = *((long *)pLabel);
            LabledMatrix matrix{label};
            //read n bytes
            char h[1];
            file.read((char *) h, 1);
            count++;
            u_int32_t hSize = (u_int32_t) h[0];

            u_int32_t k = hSize;
            //here begins the while loop
            Path p{hSize};
            u_int32_t j = 0;
            int x=0;
            if(label==184){
                x;
            }
            do {
                if (p.hasLast()) {

                    p.addLast(j);
                    j++;
                }
                char b[1];
                for (int i = j; i < k; i += 2) {
                    file.read((char *) b, sizeof(u_char));
                    count++;
                    p.add(i, b[0]);
                }

                for(Point &point : p.calculatePoints()){
                    matrix.addPoint(point);
                }
                j = hSize-1;
                for (;j>0;j--) {
                    p.pop(j - 1);
                    if (!p.isEmpty(j - 1)) {
                        break;
                    }
                }
                x++;
                if(j<=0){
                    matrixEnd=true;
                    matrices.push_back(matrix);
                }
            } while (!matrixEnd);
        } ;
        file.close();
    }
}