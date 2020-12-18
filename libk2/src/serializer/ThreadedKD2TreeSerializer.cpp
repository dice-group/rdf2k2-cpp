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

using namespace std;

ThreadedKD2TreeSerializer::ThreadedKD2TreeSerializer(bool threaded, long noOfPredicates, long noOfTriples) {
    triples = std::vector<Triple>(noOfTriples);
    matrices = std::vector<LabledMatrix>(noOfPredicates);
    int threads=1;
    if(threaded) {
        threads = std::thread::hardware_concurrency();
    }
    threadedMatrices = vector<vector<long>>();
    int count=0;
    for(long i=0;i<threads;i++){
        threadedMatrices.push_back(vector<long>());
    }
    for(long i=0;i<noOfPredicates;i++){
        threadedMatrices[count++].push_back(i);
        if(count>=threads){
            count =0;
        }
    }
}

void ThreadedKD2TreeSerializer::serializeMtx(char *out) {
    printMem("1 ");

    ofstream outfile;
    outfile.open(out, ios::out | ios::trunc );

    vector<future<void>> futures;

    for(auto & threadedMatrice : threadedMatrices){
        promise<void> pt;
        futures.push_back(pt.get_future());
        thread thr {&ThreadedKD2TreeSerializer::writeTrees, this, &threadedMatrice, &matrices, ref(outfile), move(pt)};
        thr.detach();
        //writeTrees(&threadedMatrice, &matrices, outfile, move(pt));
    }

    for (auto &future : futures) {
        future.get();
    }
    matrices.clear();
    outfile.close();
}


void ThreadedKD2TreeSerializer::writeTrees(vector<long> *use, vector<LabledMatrix> *matrices, ofstream &outfile, promise<void> pt){
    long x = 0;
    for (unsigned long u = 0; u < use->size(); u++) {
        long current = (*use)[u];
        LabledMatrix matrix = (*matrices)[current];
        TreeNode::TreeNodeBuffer nodeBuffer{};
        createTree(matrix, nodeBuffer, outfile);

        x++;
        if (x % 100 == 0) {
            cout << "Created " << x << " k2 trees of " << use->size() << endl;
        }
    }
    use->clear();
    pt.set_value();
}





void ThreadedKD2TreeSerializer::createTree(LabledMatrix &matrix, TreeNode::TreeNodeBuffer &treeNodeBuffer, ofstream &outfile){
	TreeNode root = treeNodeBuffer.constructTreeNode();
    double h = matrix.getH();
    double size = pow(2, h);
    long mSize=matrix.getPoints().size();
    long count=0;

    for(const Point &p :matrix.getPoints()){
        long c1=0 ;
        long r1=0;
        long c2=size;
        long r2=size;
        count++;

        TreeNode pnode = root;


        for(int i=0;i<h;i++){
            char node = getNode(p, c1, r1, c2, r2);

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
/*
    vector<vector<unsigned char>> hMap = vector<vector<unsigned char>>();
    for(int i=0;i<h;i++){
        hMap.push_back(vector<unsigned char>());
    }
    TreeNode * root = &treeNodeBuffer.getTreeNode(0);
    merge(root, hMap, 0, h, treeNodeBuffer);
*/
    vector<unsigned char> baos = vector<unsigned char>();

    u_char asH = h;
    atomic_uchar last = 0;
    bool shift= true;
    shift = this->merge(root, baos, shift, last, treeNodeBuffer);
    if(!shift){
        baos.push_back(last);
    }

    //Write evertyhing
    mtx.lock();
    long label = matrix.getLabel();


 /*   for(char i=(sizeof(long)-1)*8;i>=0;i-=8){
        u_char ch = (label >> i);
        u_char w= (ch & 255);
        outfile << (char) w;
    }*/
    for(char i=0;i<=(sizeof(long)-1)*8;i+=8){
        u_char w= ((label >> i) & 255);
        outfile << (char) w;
    }
    outfile << asH;
    for (unsigned int j = 0; j < baos.size(); j++) {
        outfile << baos[j];
    }
    mtx.unlock();
}

bool ThreadedKD2TreeSerializer::merge(TreeNode root, vector<unsigned char> &baos, bool shift, atomic_uchar &last, TreeNode::TreeNodeBuffer& treeNodeBuffer){
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



vector<unsigned char> intToBytes(int paramInt)
{
    vector<unsigned char> arrayOfByte(4);
    for (int i = 0; i < 4; i++)
        arrayOfByte[3 - i] = (paramInt >> (i * 8));
    return arrayOfByte;
}

char ThreadedKD2TreeSerializer::getNode(const Point &p, long c1, long r1, long c2, long r2){
    long rCenter = (r2 - r1) / 2 + r1;
    long cCenter = (c2 - c1) / 2 + c1;
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

void ThreadedKD2TreeSerializer::initSpace(vector<long> *sizeList) {
    for(unsigned int i=0;i<matrices.size();i++){
        matrices[i]=LabledMatrix(i, (*sizeList)[i]);
    }
}

void ThreadedKD2TreeSerializer::flush() {
    unsigned long count=0;
    for(auto & triple : triples){
        matrices[triple.get(1)].set(triple.get(0), triple.get(2));
        count++;
        if(count %100000==0){
            cout << "\rwrote " << count << " triples to matrix.";
        }
    }
    cout << endl;
    triples.clear();
}

void ThreadedKD2TreeSerializer::add(long sID, long pID, long oID){
    Triple triple = Triple(sID, pID, oID);
    triples[tripleCount++] = triple;
}