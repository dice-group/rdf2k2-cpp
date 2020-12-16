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

    }

    for (auto &future : futures) {
        future.get();
    }
    matrices.clear();
    outfile.flush();
    outfile.close();
}

void ThreadedKD2TreeSerializer::serialize(char *out){
    future<vector<vector<unsigned char>*>*> futures[threadedMatrices.size()];

    for(int i=0;i<threadedMatrices.size();i++){
        promise<vector<vector<unsigned char>*> *> pt;
        futures[i] = pt.get_future();
        thread thr {&ThreadedKD2TreeSerializer::threadedCreationThread, this, ref(threadedMatrices[i]), ref(matrices), move(pt)};
        thr.detach();
    }

    ofstream outfile;
    outfile.open(out, ios::out | ios::trunc );

     for(int i=0;i<threadedMatrices.size();i++){
         vector<vector<unsigned char>*> * threadPtr = futures[i].get();
         for(vector<unsigned char> *treePtr : *threadPtr){

             for(int j=0;j<treePtr->size();j++) {
                 outfile << (*treePtr)[j];
             }
             treePtr->clear();
             delete treePtr;
        }
         delete threadPtr;
      }
     matrices.clear();
    outfile.flush();
    outfile.close();
}

void ThreadedKD2TreeSerializer::threadedCreationThread(vector<long> &use, vector<LabledMatrix> &matrices, promise<vector<vector<unsigned char> *> *> promise) {
    vector<vector<unsigned char> *> *vec = threadedCreation(use, matrices);
    promise.set_value(vec);
}

void ThreadedKD2TreeSerializer::writeTrees(vector<long> *use, vector<LabledMatrix> *matrices, ofstream &outfile, promise<void> pt){
    long x = 0;
    for (long u = 0; u < use->size(); u++) {
        long current = (*use)[u];
        LabledMatrix matrix = (*matrices)[current];
        unique_ptr<vector<unsigned char>> treePtr = createTree(matrix);

        mtx.lock();
        for (int j = 0; j < treePtr->size(); j++) {
            outfile << (*treePtr)[j];
        }
        mtx.unlock();
        treePtr->clear();

        x++;
        if (x % 10 == 0) {
            cout << "Created " << x << " k2 trees of " << use->size() << endl;
        }
    }
    use->clear();
    pt.set_value();
}

vector<vector<unsigned char> *> *ThreadedKD2TreeSerializer::threadedCreation(vector<long> &use, vector<LabledMatrix> &matrices){
    long x=0;
    vector<vector<unsigned char> *> *trees = new vector<vector<unsigned char>*>();
    for(long current : use){
        LabledMatrix &matrix = matrices[current];
        printMem("1 ");

        unique_ptr<vector<unsigned char>> treePtr = createTree(matrix);
        printMem("2 ");

        trees->push_back(treePtr.get());
        printMem("3 ");

        x++;
        if(x%10==0) {
            cout << "Created " << x << " k2 trees of " << use.size() << endl;
        }
    }
    return trees;
}



unique_ptr<vector<unsigned char>> ThreadedKD2TreeSerializer::createTree(LabledMatrix &matrix){
    auto root = make_unique<TreeNode>();
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

        TreeNode * pnode = root.get();

        if(count % 100000 ==0){
            cout << "\r" << count << "/" << mSize << endl;
        }

        for(int i=0;i<h;i++){
            char node = getNode(p, c1, r1, c2, r2);

            pnode = pnode->setChildIfAbsent(node).get();

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

    vector<vector<unsigned char>> hMap = vector<vector<unsigned char>>();
    for(int i=0;i<h;i++){
        hMap.push_back(vector<unsigned char>());
    }
    merge(root, hMap, 0, h);

    unique_ptr<vector<unsigned char>> baos = make_unique<vector<unsigned char>>();
    //vector<unsigned char> *baos = new vector<unsigned char>();
    // convert from an unsigned long int to a 4-byte array
    long label = matrix.getLabel();
    baos->push_back((int)((label >> 24) & 0xFF));
    baos->push_back((int)((label >> 16) & 0xFF)) ;
    baos->push_back((int)((label >> 8) & 0XFF));
    baos->push_back((int)((label & 0XFF)));


    bool shift=true;
    unsigned char last=0;
    for(int i=0; i<hMap.size();i++){
        for(unsigned char b : hMap[i]) {
            //tree.addNodeNeighbor(b);
            if(shift) {
                last = b << 4;
                shift=false;
            }
            else{
                baos->push_back( last | b);
                shift=true;
            }
        }
        hMap[i].clear();
    }
    if(!shift){
        baos->push_back(last);
    }
    baos->push_back(0);
    hMap.clear();
    return baos;
}

void ThreadedKD2TreeSerializer::merge(unique_ptr<TreeNode> &root, std::vector<vector<unsigned char>> &hMap, int h, double max){
    // https://en.cppreference.com/w/cpp/memory/shared_ptr/operator_bool
    if(bool(root) || h>=max){
        return;
    }

    vector<unsigned char> &arr = hMap[h];
    arr.push_back(root->getRawValue(true));
    unique_ptr<TreeNode> &c0 = root->getChild(0);
    unique_ptr<TreeNode> &c1 = root->getChild(1);
    unique_ptr<TreeNode> &c2 = root->getChild(2);
    unique_ptr<TreeNode> &c3 = root->getChild(3);

    merge(c0, hMap, h+1, max);
    merge(c1, hMap, h+1, max);
    merge(c2, hMap, h+1, max);
    merge(c3, hMap, h+1, max);
    root->clear();


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
    for(int i=0;i<matrices.size();i++){
        matrices[i]=LabledMatrix(i, (*sizeList)[i]);
    }
}

void ThreadedKD2TreeSerializer::flush() {
    long count=0;
    for(long i=0; i<triples.size(); i++){
        Triple &triple = triples[i];
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