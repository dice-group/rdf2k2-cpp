//
// Created by minimal on 12/18/20.
//

#include "RDFDecompressor.h"
#include <iostream>
#include <fstream>
#include <vector>

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

void RDFDecompressor::readK2(char *in, std::vector<LabledMatrix>& matrices){
    std::ifstream file (in, std::ios::in|std::ios::binary|std::ios::ate);
    if (file.is_open()) {
        //read long, read h
        file.seekg(0, std::ios::beg);

        bool matrixEnd=false;
        do {
            matrixEnd=false;
            char pLabel[sizeof(long)];
            file.read((char *) &pLabel, sizeof(pLabel));
            long label = (long) pLabel;

            LabledMatrix matrix{label};
            //read n bytes
            u_char *h;
            file.read((char *) h, sizeof(u_char));
            u_int32_t hSize = (u_int32_t) h[0];

            u_int32_t k = hSize;
            //here begins the while loop
            Path p{hSize};
            int j = 0;
            do {
                u_char *b;
                if (p.hasLast()) {
                    p.addLast();
                    j++;
                }
                for (int i = j; i < k; i += 2) {
                    file.read((char *) b, sizeof(u_char));
                    p.add(i, *b);
                }
                Point point = p.calculatePoint();
                matrix.set(point.getRow(), point.getCol());
                j = hSize;
                for (p.pop(j); p.isEmpty(j) || j != -1;j--) {
                    //empty as we pop before
                }
                if(j==-1){
                    matrixEnd=true;
                    matrices.push_back(matrix);
                }
            } while (!matrixEnd);
        } while (!file.eof());
    }
}