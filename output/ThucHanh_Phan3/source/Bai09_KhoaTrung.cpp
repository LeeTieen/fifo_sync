// Thuc hanh CTDL va GT - Phan 3. Moi file bien dich doc lap: g++ -std=c++11 file.cpp -o app
// Dua tren GiaoTrinhThucHanh_Phan3.pdf. h(NULL)=0, h(la)=1; BF=h(phai)-h(trai).
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <limits>
#include <stdexcept>
#include <cstdlib>
#include <random>
#include <numeric>
#include <cmath>
using namespace std;

struct Node{int data,count;Node*left,*right;explicit Node(int x):data(x),count(1),left(nullptr),right(nullptr){}};
Node* Insert(Node*r,int x){if(!r)return new Node(x);if(x<r->data)r->left=Insert(r->left,x);else if(x>r->data)r->right=Insert(r->right,x);else ++r->count;return r;}
Node* Search(Node*r,int x){while(r&&r->data!=x)r=x<r->data?r->left:r->right;return r;}
// eraseAll dung khi di chuyen nut the mang: phai xoa het nut cu cung khoa.
Node* Delete(Node*r,int x,bool eraseAll=false){
    if(!r)return r;
    if(x<r->data)r->left=Delete(r->left,x,eraseAll);
    else if(x>r->data)r->right=Delete(r->right,x,eraseAll);
    else{
        if(r->count>1&&!eraseAll){--r->count;return r;}
        if(!r->left||!r->right){Node*c=r->left?r->left:r->right;delete r;return c;}
        Node*s=r->right;while(s->left)s=s->left;
        r->data=s->data;r->count=s->count;r->right=Delete(r->right,s->data,true);
    }return r;
}
void Print(Node*r){if(r){Print(r->left);cout<<r->data<<"(count="<<r->count<<") ";Print(r->right);}}
void Clear(Node*&r){if(r){Clear(r->left);Clear(r->right);delete r;r=nullptr;}}
int main(){Node*r=nullptr;for(int x:{8,3,10,3,8,8,9,9})r=Insert(r,x);Print(r);cout<<'\n';
    for(int x:{8,8,8,3,42}){r=Delete(r,x);cout<<"Xoa mot lan "<<x<<": ";Print(r);cout<<'\n';}
    cout<<"Tan suat 9="<<Search(r,9)->count<<'\n';Clear(r);return 0;}
