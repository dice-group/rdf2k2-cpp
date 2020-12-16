#ifndef RDF2K2_CPP_ThreadedKD2TreeSerializer_H
#define RDF2K2_CPP_ThreadedKD2TreeSerializer_H

#include <vector>
#include <map>
#include <memory>
#include <future>

#include "../util/LabledMatrix.h"
#include "../util/Triple.h"
#include "../util/TreeNode.h"
#include <mutex>

class ThreadedKD2TreeSerializer {
    public:
        void serialize(char *out);
        void add(long sID, long pID, long oID);
        ThreadedKD2TreeSerializer(bool threaded, long predicates, long triples);
        void flush();
        void initSpace(std::vector<long> &sizeList);
        void threadedCreationThread(std::vector<long> &use, std::vector<LabledMatrix> &matrices, std::promise<vector<vector<unsigned char> *>*> promise);
    void serializeMtx(char *out);

    std::vector<std::vector<unsigned char> *> *threadedCreation(std::vector<long> &use, std::vector<LabledMatrix> &matrices);
private:
        std::vector<Triple> triples;
        std::vector<LabledMatrix> matrices;
        std::vector<std::vector<long>> threadedMatrices;
        std::unique_ptr<std::vector<unsigned char>> createTree(LabledMatrix &matrix);
        char getNode(Point &p, long c1, long r1, long c2, long r2);
        void merge(shared_ptr<TreeNode> root, std::vector<vector<unsigned char>> &hMap, int h, double max);
        long tripleCount=0;
        mutex mtx;
    void writeTrees(vector<long> *use, vector<LabledMatrix> *matrices, ofstream &outfile, promise<void> pt);
};

#endif //RDF2K2_CPP_ThreadedKD2TreeSerializer_H
