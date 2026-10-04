#define main original_main
#include "D:/DV_Course/exercise/PROJECT/FIFO_SYNC/output/ThucHanh_Phan3/source/N7_DungCay.cpp"
#undef main
#include <cassert>

int main(){TNode*r=BuildFromTraversals({1,2,4,5,3},{4,2,5,1,3});assert(r->pLeft->pRight->data==5);Clear(r);assert(!BuildFromTraversals({},{}));int errors=0;
    for(auto in:vector<vector<int>>{{1,2},{1,1,3},{3,1,2}}){try{r=BuildFromTraversals({1,2,3},in);Clear(r);}catch(const exception&){++errors;}}assert(errors==3);cout<<"PASS traversal reconstruction and invalid inputs\n";return 0;}