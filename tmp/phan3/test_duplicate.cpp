#define main original_main
#include "D:/DV_Course/exercise/PROJECT/FIFO_SYNC/output/ThucHanh_Phan3/source/Bai09_KhoaTrung.cpp"
#undef main
#include <cassert>

int main(){Node*r=nullptr;for(int x:{8,3,10,9,9})r=Insert(r,x);r=Delete(r,8);assert(r->data==9&&r->count==2&&r->right->left==nullptr);r=Delete(r,9);assert(r->count==1);r=Delete(r,9);assert(!Search(r,9));Clear(r);cout<<"PASS successor with count=2\n";return 0;}