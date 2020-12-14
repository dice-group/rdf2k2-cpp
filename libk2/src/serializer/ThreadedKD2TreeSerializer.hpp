#ifndef RDF2K2_CPP_ThreadedKD2TreeSerializer_H
#define RDF2K2_CPP_ThreadedKD2TreeSerializer_H

#include <vector>
#include <map>
#include <memory>
#include "../util/LabledMatrix.h"
#include "../util/Triple.h"


class ThreadedKD2TreeSerializer {
    public:
        void serialize(char *out);
        void add(long sID, long pID, long oID);
        ThreadedKD2TreeSerializer(bool threaded, long predicates, long triples);
        void flush();
        void initSpace(std::shared_ptr<long []>sizeList);
    private:
        std::vector<Triple> triples;
        std::vector<LabledMatrix> matrices;
        std::vector<std::vector<long>> threadedMatrices;
        std::vector<std::shared_ptr<char[]>> threadedCreation(std::vector<long> &use, std::vector<LabledMatrix> &matrices);
        std::shared_ptr<char[]> createTree(LabledMatrix &matrix);
        char getNode(Point &p, long c1, long r1, long c2, long r2);
};

#endif //RDF2K2_CPP_ThreadedKD2TreeSerializer_H
