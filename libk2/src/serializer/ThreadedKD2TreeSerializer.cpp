#include "ThreadedKD2TreeSerializer.hpp"
#include <iostream>
#include "../util/LabledMatrix.h"
#include <memory>
#include "../util/Triple.h"
#include <thread>
#include <fstream>
#include "math.h"
#include <future>
#include <functional>
#include "../util/Stats.h"
#include "../util/TreeNode.h"
#include <mutex>          // std::mutex
#include <atomic>


k2::ThreadedKD2TreeSerializer::ThreadedKD2TreeSerializer(bool threaded, size_t noOfPredicates, size_t noOfTriples) {
    triples = std::vector<Triple>(noOfTriples);
    matrices = std::vector<LabledMatrix>(noOfPredicates);
    u_int threads=1;
    if(threaded) {
        threads = std::thread::hardware_concurrency();
    }
    threadedMatrices = std::vector<std::vector<size_t>>();
    int count=0;
    for(u_int i=0;i<threads;i++){
        threadedMatrices.push_back(std::vector<size_t>());
    }
    for(size_t i=0;i<noOfPredicates;i++){
        threadedMatrices[count++].push_back(i);
        if(count>=threads){
            count =0;
        }
    }
}

void k2::ThreadedKD2TreeSerializer::serializeMtx(char *out) {
    printMem("1 ");

    std::ofstream outfile;
    outfile.open(out, std::ios::out | std::ios::trunc );

    std::vector<std::future<void>> futures;

    for(auto & threadedMatrice : threadedMatrices){
        std::promise<void> pt;
        futures.push_back(pt.get_future());
        std::thread thr {&ThreadedKD2TreeSerializer::writeTrees, this, &threadedMatrice, &matrices, ref(outfile), move(pt)};
        thr.detach();
        //writeTrees(&threadedMatrice, &matrices, outfile, move(pt));
    }

    for (auto &future : futures) {
        future.get();
    }
    matrices.clear();
    outfile.close();
}


void k2::ThreadedKD2TreeSerializer::writeTrees(std::vector<size_t> *use, std::vector<LabledMatrix> *matrices, std::ofstream &outfile, std::promise<void> pt){
    size_t x = 0;
    for (size_t u = 0; u < use->size(); u++) {
        size_t current = (*use)[u];
        LabledMatrix matrix = (*matrices)[current];
        TreeNode::TreeNodeBuffer nodeBuffer{};
        createTree(matrix, nodeBuffer, outfile);

        x++;
        if (x % 100 == 0) {
            std::cout << "Created " << x << " k2 trees of " << use->size() << std::endl;
        }
    }
    use->clear();
    pt.set_value();
}





void k2::ThreadedKD2TreeSerializer::createTree(LabledMatrix &matrix, TreeNode::TreeNodeBuffer &treeNodeBuffer, std::ofstream &outfile) {
	TreeNode root = treeNodeBuffer.constructTreeNode();
    double h = matrix.getH();
    double size = pow(2, h);
    size_t count=0;

    for(const Point &p :matrix.getPoints()){
        size_t c1=0 ;
        size_t r1=0;
        size_t c2=size;
        size_t r2=size;
        count++;

        TreeNode pnode = root;


        for(int i=0;i<h;i++){
            char node;
            node = getNode(p, c1, r1, c2, r2);

            pnode = pnode.setChildIfAbsent(node, treeNodeBuffer);

            if(node==0){
                r2 = (r2 - r1) / 2 + r1;
                c2 = (c2 - c1) / 2 + c1;
            }
            else if(node==1){
                ///y=row, x =col
                r2 = (r2 - r1) / 2 + r1;
                c1 = (c2 - c1) / 2 + c1;
            }
            else if(node==2){
                r1 = (r2 - r1) / 2 + r1;
                c2 = (c2 - c1) / 2 + c1;
            }
            else if(node==3){
                r1 = (r2 - r1) / 2 + r1;
                c1 = (c2 - c1) / 2 + c1;
            }

        }

    }
    matrix.clear();

    std::vector<u_char> baos = std::vector<u_char>();

    u_char asH = h;
    std::atomic_uchar last = 0;
    bool shift= true;
    shift = this->merge(root, baos, shift, last, treeNodeBuffer);
    if(!shift){
        baos.push_back(last);
    }
    //Write evertyhing
    mtx.lock();
    u_int32_t label = matrix.getLabel();

    for(u_char i=0;i<=(sizeof(label)-1)*8;i+=8){
        u_char w= ((label >> i) & 255);
        outfile << (char) w;
    }
    outfile << asH;
    for (size_t j = 0; j < baos.size(); j++) {
        outfile << baos[j];
    }
    mtx.unlock();
}

bool k2::ThreadedKD2TreeSerializer::merge(TreeNode root, std::vector<u_char> &baos, bool shift, std::atomic_uchar &last, TreeNode::TreeNodeBuffer& treeNodeBuffer){
    if(!bool(root) || root.isLeaf(treeNodeBuffer)){
        return shift;
    }
    unsigned char b =root.getRawValue(false,treeNodeBuffer);
    if(shift) {
        last.store(b << 4);
        shift=false;
    }
    else{
        unsigned char write = last | b;
        baos.push_back( write);
        last.store(0);
        shift=true;
    }
    TreeNode c0 = root.getChild(0, treeNodeBuffer);
    TreeNode c1 = root.getChild(1, treeNodeBuffer);
    TreeNode c2 = root.getChild(2, treeNodeBuffer);
    TreeNode c3 = root.getChild(3, treeNodeBuffer);

    shift = merge(c0, baos, shift, last, treeNodeBuffer);
    shift = merge(c1, baos,  shift, last, treeNodeBuffer);
    shift = merge(c2, baos,  shift, last, treeNodeBuffer);
    shift = merge(c3, baos,  shift, last, treeNodeBuffer);
    root.clear(treeNodeBuffer);
    return shift;
}


char k2::ThreadedKD2TreeSerializer::getNode(const Point &p, size_t c1, size_t r1, size_t c2, size_t r2) const{
    size_t rCenter = (r2 - r1) / 2 + r1;
    size_t cCenter = (c2 - c1) / 2 + c1;
    if(p.getCol()<cCenter && p.getRow() <rCenter){
        return 0;
    }
    if(p.getCol()>=cCenter && p.getRow() <rCenter){
        return 1;
    }
    if(p.getCol()<cCenter && p.getRow() >= rCenter){
        return 2;
    }
    else{
        return 3;
    }
}

void k2::ThreadedKD2TreeSerializer::initSpace(std::vector<size_t> *sizeList) {
    for(u_int32_t i=0;i<matrices.size();i++){
        matrices[i]=LabledMatrix(i, (*sizeList)[i]);
    }
}

void k2::ThreadedKD2TreeSerializer::flush() {
    size_t count=0;
    for(auto & triple : triples){
        u_int32_t predicate = static_cast<u_int32_t>(triple.get(1));
        matrices[predicate].set(triple.get(0), triple.get(2));
        count++;
        if(count %100000==0){
            std::cout << "\rwrote " << count << " triples to matrix.";
        }
    }
    std::cout << "wrote " << count << " triples to matrix" << std::endl;
    triples.clear();
}

void k2::ThreadedKD2TreeSerializer::add(size_t sID, size_t pID, size_t oID){
    Triple triple = Triple(sID, pID, oID);
    triples[tripleCount++] = triple;
}