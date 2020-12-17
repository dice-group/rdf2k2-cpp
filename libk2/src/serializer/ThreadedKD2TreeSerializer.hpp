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
#include <atomic>

class ThreadedKD2TreeSerializer {
    public:
        void add(long sID, long pID, long oID);
        ThreadedKD2TreeSerializer(bool threaded, long predicates, long triples);
        void flush();
        void initSpace(std::vector<long> *sizeList);
        void serializeMtx(char *out);

private:
        std::vector<Triple> triples;
        std::vector<LabledMatrix> matrices;
        std::vector<std::vector<long>> threadedMatrices;
        void createTree(LabledMatrix &matrix, TreeNode::TreeNodeBuffer &treeNodeBuffer, ofstream &outfile);
        char getNode(const Point &p, long c1, long r1, long c2, long r2);
        bool merge(TreeNode root, vector<unsigned char> &baos, bool shift, atomic_uchar &last, TreeNode::TreeNodeBuffer& treeNodeBuffer);
        long tripleCount=0;
        mutex mtx;
    void writeTrees(vector<long> *use, vector<LabledMatrix> *matrices, ofstream &outfile, promise<void> pt);
};

#endif //RDF2K2_CPP_ThreadedKD2TreeSerializer_H
