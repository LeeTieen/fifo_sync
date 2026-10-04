#define main original_main
#include "D:/DV_Course/exercise/PROJECT/FIFO_SYNC/output/ThucHanh_Phan3/source/Bai10_AVL.cpp"
#undef main
#include <cassert>

void collect(AVLNode*r,vector<int>&a){if(r){collect(r->left,a);a.push_back(r->data);collect(r->right,a);}}
int main(){
    AVLNode*r=nullptr;assert(IsAVL(r));
    for(auto v:vector<vector<int>>{{30,20,10},{10,20,30},{30,10,20},{10,30,20}}){for(int x:v)r=insertNode(r,x);assert(r->data==20&&IsAVL(r));clearAVL(r);}
    for(int x:{4,2,5,1,3})r=insertNode(r,x);r=deleteNode(r,5);assert(IsAVL(r)&&r->data==2);clearAVL(r);
    for(int x:{2,1,4,3,5})r=insertNode(r,x);r=deleteNode(r,1);assert(IsAVL(r)&&r->data==4);clearAVL(r);
    r=insertNode(r,numeric_limits<int>::min());r=insertNode(r,numeric_limits<int>::max());assert(IsAVL(r));++r->height;assert(!IsAVL(r));--r->height;clearAVL(r);
    vector<int>ref;mt19937 gen(42);
    for(int i=0;i<10000;++i){int x=static_cast<int>(gen()%501)-250;
        if(gen()%2){r=insertNode(r,x);if(find(ref.begin(),ref.end(),x)==ref.end())ref.push_back(x);}
        else{r=deleteNode(r,x);ref.erase(remove(ref.begin(),ref.end(),x),ref.end());}
        sort(ref.begin(),ref.end());vector<int>a;collect(r,a);assert(a==ref&&IsAVL(r));
    }
    for(int x:ref){r=deleteNode(r,x);assert(IsAVL(r));}assert(!r);
    cout<<"PASS 10000 mixed AVL operations, four rotations, child BF=0 deletion, stale height\n";return 0;
}