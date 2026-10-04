#define main original_main
#include "D:/DV_Course/exercise/PROJECT/FIFO_SYNC/output/ThucHanh_Phan3/source/Bai09_MoRong.cpp"
#undef main
#include <cassert>

int main(){
    TNode*r=nullptr;assert(IsBST(r)&&IsComplete(r)&&!FindClosest(r,0)&&!KthSmallest(r,1));
    r=Insert(r,numeric_limits<int>::min());r=Insert(r,numeric_limits<int>::max());assert(IsBST(r));
    assert(FindClosest(r,0)->data==numeric_limits<int>::max());Clear(r);
    r=Sample();assert(Height(r)==4&&CountNodes(r)==9&&FindClosest(r,5)->data==4);
    assert(!IsComplete(r)&&!KthSmallest(r,0)&&!KthSmallest(r,10)&&KthSmallest(r,4)->data==6);
    r=Delete(r,1);r=Delete(r,14);r=Delete(r,8);vector<int>a;Collect(r,a);assert((a==vector<int>{3,4,6,7,10,13})&&r->data==10);Clear(r);
    r=new TNode(8);r->pLeft=new TNode(3);r->pRight=new TNode(10);r->pLeft->pRight=new TNode(9);assert(!IsBST(r));Clear(r);
    r=new TNode(1);r->pRight=new TNode(2);assert(!IsComplete(r));Clear(r);
    vector<int>ref;mt19937 gen(2026);
    for(int i=0;i<5000;++i){int x=static_cast<int>(gen()%201)-100;
        if(gen()%2){r=Insert(r,x);if(find(ref.begin(),ref.end(),x)==ref.end())ref.push_back(x);}
        else{r=Delete(r,x);ref.erase(remove(ref.begin(),ref.end(),x),ref.end());}
        sort(ref.begin(),ref.end());a.clear();Collect(r,a);assert(a==ref&&IsBST(r));
    }
    SaveTree(r,"test_tree.txt");TNode*copy=LoadTree("test_tree.txt");assert(SameTree(r,copy));Clear(copy);
    r=BalanceBST(r);a.clear();Collect(r,a);assert(a==ref&&IsBST(r));Clear(r);
    for(string data:{"8 3 10 9 2","1 1","abc","2147483648"}){ofstream f("bad_tree.txt");f<<data;f.close();bool bad=false;try{copy=LoadTree("bad_tree.txt");Clear(copy);}catch(const exception&){bad=true;}assert(bad);}
    SaveTree(nullptr,"empty_tree.txt");copy=LoadTree("empty_tree.txt");assert(!copy);
    cout<<"PASS 5000 mixed BST operations and edge/file tests\n";return 0;
}