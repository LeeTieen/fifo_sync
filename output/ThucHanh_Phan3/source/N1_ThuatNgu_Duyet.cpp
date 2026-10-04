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

struct TNode {
    int data;
    TNode *pLeft, *pRight;
    explicit TNode(int x): data(x), pLeft(nullptr), pRight(nullptr) {}
};
void KhoiTao(TNode*& root) { root=nullptr; }
bool IsEmpty(TNode* root) { return root==nullptr; }
TNode* TaoNode(int x) { return new TNode(x); }
// Khong tao nut thu hai cho khoa trung.
TNode* Insert(TNode* root, int x) {
    if (!root) return TaoNode(x);
    if (x<root->data) root->pLeft=Insert(root->pLeft,x);
    else if (x>root->data) root->pRight=Insert(root->pRight,x);
    return root;
}
TNode* Search(TNode* r,int x) {
    if (!r || r->data==x) return r;
    return Search(x<r->data?r->pLeft:r->pRight,x);
}
TNode* SearchIterative(TNode* r,int x) {
    while (r && r->data!=x) r=x<r->data?r->pLeft:r->pRight;
    return r;
}
TNode* FindMin(TNode* r) { while(r && r->pLeft) r=r->pLeft; return r; }
TNode* FindMax(TNode* r) { while(r && r->pRight) r=r->pRight; return r; }
TNode* Delete(TNode* r,int x) {
    if (!r) return nullptr;
    if (x<r->data) r->pLeft=Delete(r->pLeft,x);
    else if (x>r->data) r->pRight=Delete(r->pRight,x);
    else {
        if (!r->pLeft || !r->pRight) {
            TNode* child=r->pLeft?r->pLeft:r->pRight;
            delete r; return child;
        }
        // Hai con: min phai the mang, giu nguyen cay con trai.
        TNode* next=FindMin(r->pRight);
        r->data=next->data;
        r->pRight=Delete(r->pRight,next->data);
    }
    return r;
}
void NLR(TNode* r) { if(r){ cout<<r->data<<' '; NLR(r->pLeft); NLR(r->pRight); } }
void LNR(TNode* r) { if(r){ LNR(r->pLeft); cout<<r->data<<' '; LNR(r->pRight); } }
void LRN(TNode* r) { if(r){ LRN(r->pLeft); LRN(r->pRight); cout<<r->data<<' '; } }
void LevelOrder(TNode* r) {
    if(!r) return;
    queue<TNode*> q; q.push(r);
    while(!q.empty()) {
        TNode* p=q.front(); q.pop(); cout<<p->data<<' ';
        if(p->pLeft) q.push(p->pLeft);
        if(p->pRight) q.push(p->pRight);
    }
}
int Height(TNode* r) { return r?1+max(Height(r->pLeft),Height(r->pRight)):0; }
int CountNodes(TNode* r) { return r?1+CountNodes(r->pLeft)+CountNodes(r->pRight):0; }
// Khoang mo long long cho phep khoa INT_MIN va INT_MAX.
bool IsBST(TNode* r,long long lo=numeric_limits<long long>::min(),
           long long hi=numeric_limits<long long>::max()) {
    if(!r) return true;
    return lo<r->data && r->data<hi && IsBST(r->pLeft,lo,r->data)
           && IsBST(r->pRight,r->data,hi);
}
void PrintTree(TNode* r,string prefix="",string edge="ROOT ") {
    cout<<prefix<<edge;
    if(!r) { cout<<"null\n"; return; }
    cout<<r->data<<'\n';
    if(r->pLeft || r->pRight) {
        PrintTree(r->pLeft,prefix+"|   ","L--- ");
        PrintTree(r->pRight,prefix+"|   ","R--- ");
    }
}
void Clear(TNode*& r) { if(r){ Clear(r->pLeft); Clear(r->pRight); delete r; r=nullptr; } }
void AllOrders(TNode* r) {
    cout<<"NLR: "; NLR(r); cout<<"\nLNR: "; LNR(r);
    cout<<"\nLRN: "; LRN(r); cout<<"\nLevel: "; LevelOrder(r); cout<<'\n';
}
TNode* Sample() {
    TNode* r=nullptr;
    for(int x: {8,3,10,1,6,14,4,7,13}) r=Insert(r,x);
    return r;
}

void NLRIterative(TNode*r){if(!r)return;stack<TNode*>s;s.push(r);while(!s.empty()){auto p=s.top();s.pop();cout<<p->data<<' ';if(p->pRight)s.push(p->pRight);if(p->pLeft)s.push(p->pLeft);}}
int Leaves(TNode*r){if(!r)return 0;if(!r->pLeft&&!r->pRight)return 1;return Leaves(r->pLeft)+Leaves(r->pRight);}
int TwoChildren(TNode*r){return r?(r->pLeft&&r->pRight?1:0)+TwoChildren(r->pLeft)+TwoChildren(r->pRight):0;}
int Level(TNode*r,int x,int depth=1){if(!r)return 0;if(r->data==x)return depth;int l=Level(r->pLeft,x,depth+1);return l?l:Level(r->pRight,x,depth+1);}
int main(){
    TNode*r=nullptr;for(int x:{8,3,10,1,6,14,4})r=Insert(r,x);
    PrintTree(r);AllOrders(r);cout<<"NLR stack: ";NLRIterative(r);
    cout<<"\nMuc(4)="<<Level(r,4)<<" Height="<<Height(r)<<" La="<<Leaves(r)<<" Hai con="<<TwoChildren(r)<<'\n';
    Clear(r);for(int x:{1,2,3,4})r=Insert(r,x);PrintTree(r);AllOrders(r);cout<<"Height="<<Height(r)<<'\n';Clear(r);return 0;
}
