//
// Created by minimal on 12/18/20.
//

#include "RDFDecompressor.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <FourSectionDictionary.hpp>

#include <bit>
#include <bitset>
#include "../util/Path.h"
#include <cstring>
#include "../util/StringUtils.h"
#include "../util/RDFTools.h"
#include <regex>
#include <chrono>
#include <cmath>

#include "../io/ByteBuffer.h"



void k2::RDFDecompressor::writeRDF(char *in, char* out){
    std::chrono::high_resolution_clock::time_point p1 = chrono::high_resolution_clock::now();
    std::vector<k2::LabledMatrix> matrices{};
    readK2(in, matrices);
    std::chrono::high_resolution_clock::time_point p2 = chrono::high_resolution_clock::now();
    std::chrono::duration<double, milli> time_span = p2 - p1;
    std::cout << "Reading k2 trees took " << time_span.count() << " ms " << endl;
    hdt::FourSectionDictionary dict{};

    char dictIn[strlen(in) + 5];
    strcpy(dictIn, in);
    strcat(dictIn, ".dict");
    readDict(dictIn, dict);
    p1 = chrono::high_resolution_clock::now();
    time_span = p1 - p2;
    std::cout << "Reading Dict took " << time_span.count() << " ms " << endl;

    //TODO for now just use ntriples, but later on we want to use SERD
    writeNTRIPLES(out, matrices, dict);
    p2 = chrono::high_resolution_clock::now();

    time_span = p2 - p1;
    std::cout << "Writing triples took " << time_span.count() << " ms " << endl;
}

void k2::RDFDecompressor::writeNTRIPLES(char *out, std::vector<k2::LabledMatrix> &matrices, hdt::FourSectionDictionary &dict){
    std::ofstream output;
    output.open(out, ios::out | ios::trunc );
    size_t count = 0;
    for(k2::LabledMatrix &matrix: matrices) {
        std::string writeBuffer{};
        std::string predicate = dict.idToString(matrix.getLabel()+1, hdt::TripleComponentRole::PREDICATE);
        for(const k2::Point &p : matrix.getPoints()) {
            std::string subject = dict.idToString(p.getRow()+1, hdt::TripleComponentRole::SUBJECT);
            std::string object = dict.idToString(p.getCol()+1, hdt::TripleComponentRole::OBJECT);
            writeBuffer += k2::RDFTools::getTerm(subject)+" <"+predicate+"> "+k2::RDFTools::getTerm(object)+" .\n";
            count++;
            if(count % 1'000 ==0){
                output << writeBuffer ;
                output.flush();
                writeBuffer="";
            }
        }
        output << writeBuffer ;
        output.flush();
    }
    std::cout << "Wrote " << count << " triples " << std::endl ;

    output.close();
}


void  k2::RDFDecompressor::readDict(char *dictIn, hdt::FourSectionDictionary &dict){
    //std::ifstream file{dictIn, std::ios::in|std::ios::binary|std::ios::ate};
    //std::istream& s   = file;
    hdt::ControlInformation ci =hdt::ControlInformation();
    ci.setFormat(dict.getType());

    u_char * memblock;
    std::streampos size;
    std::ifstream file (dictIn, std::ios::in|std::ios::binary|std::ios::ate);
    if (file.is_open())
    {
        size = file.tellg();
        memblock = new u_char [size];
        file.seekg (0, std::ios::beg);
        file.read ((char *)memblock, size);
        file.close();
    }
    dict.load(memblock, memblock+size);
    file.close();
}


void k2::RDFDecompressor::readK2(char *in, std::vector<LabledMatrix>& matrices){
    std::ifstream file (in, std::ios::in|std::ios::binary|std::ios::ate);
    std::streampos size;

    if (file.is_open()) {
        //read long, read h
        size = file.tellg();
        file.seekg(0, std::ios::beg);
        size_t s = 1*size;
        k2::ByteBuffer byteBuffer{&file, s};
        bool matrixEnd;
        u_char pLabel[sizeof(u_int32_t)];
        size_t count=0;
        //file.eof or file.good won't work here
        while (!byteBuffer.eos()){
        //while (count<size){
            matrixEnd=false;

            size_t pLabelLen= sizeof(pLabel);
            //file.read((char *) &pLabel, sizeof(pLabel));
            byteBuffer.next(pLabelLen, (u_char *) &pLabel);
            count+=sizeof(pLabel);
            u_int32_t label=0;
            //use this instead of memcpy so we don't have to care about little/big endian
            for(u_char byteShift=0;byteShift<sizeof(label);byteShift++){
                u_int32_t add = pLabel[byteShift] << byteShift*8;
                label += add;
            }
            //convert pLabel to label.
            //std::memcpy(&label, &pLabel, sizeof pLabel);
            //long label = *((long *)pLabel);

            LabledMatrix matrix{label};
            //read n bytes
            u_char h[1];
            byteBuffer.next(1, h);
            //file.read((char *) h, 1);
            count++;
            u_char hSize = 1*h[0];

            u_char k = hSize;
            //here begins the while loop
            Path p{hSize};
            u_char j = 0;
            int x=0;
            do {
                if (p.hasLast()) {
                    p.addLast(j);
                    j++;
                }
                u_char b[1];
                for (u_char i = j; i < k; i += 2) {

                    //file.read((char *) b, sizeof(u_char));
                    byteBuffer.next(1, b);
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
            if(matrices.size()%100==0){
                std::cout << "\r converted " << matrices.size() << " matrices";
            }
        } ;
        std::cout << std::endl;
        file.close();
    }
}