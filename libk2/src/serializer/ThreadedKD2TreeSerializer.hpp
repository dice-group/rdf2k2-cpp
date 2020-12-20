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
namespace k2 {

    class ThreadedKD2TreeSerializer {
    public:
        void add(size_t sID, size_t pID, size_t oID);

        ThreadedKD2TreeSerializer(bool threaded, size_t predicates, size_t triples);

        void flush();

        void initSpace(std::vector<size_t> *sizeList);

        void serializeMtx(char *out);

    private:
        std::vector<Triple> triples;
        std::vector<LabledMatrix> matrices;
        std::vector<std::vector<size_t>> threadedMatrices;

        void createTree(LabledMatrix &matrix, TreeNode::TreeNodeBuffer &treeNodeBuffer, std::ofstream &outfile);

        char getNode(const Point &p, size_t c1, size_t r1, size_t c2, size_t r2) const;

        bool merge(TreeNode root, std::vector<u_char> &baos, bool shift, std::atomic_uchar &last,
                   TreeNode::TreeNodeBuffer &treeNodeBuffer);

        long tripleCount = 0;
        std::mutex mtx;

        void writeTrees(std::vector<size_t> *use, std::vector<LabledMatrix> *matrices, std::ofstream &outfile,
                        std::promise<void> pt);
    };
}
#endif //RDF2K2_CPP_ThreadedKD2TreeSerializer_H
