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

struct AVLNode {
    int data;
    AVLNode *left,*right;
    int height;
    explicit AVLNode(int x): data(x),left(nullptr),right(nullptr),height(1) {}
};
bool traceAVL=false;
int getHeight(AVLNode* r) { return r?r->height:0; }
int getBalance(AVLNode* r) { return r?getHeight(r->right)-getHeight(r->left):0; }
void updateHeight(AVLNode* r) { r->height=1+max(getHeight(r->left),getHeight(r->right)); }
void printTree(AVLNode* r,string pre="",string edge="ROOT ") {
    cout<<pre<<edge;
    if(!r){cout<<"null\n";return;}
    cout<<r->data<<" [h="<<r->height<<", BF="<<getBalance(r)<<"]\n";
    if(r->left || r->right){printTree(r->left,pre+"|   ","L--- ");printTree(r->right,pre+"|   ","R--- ");}
}
AVLNode* rightRotate(AVLNode* y) {
    AVLNode* x=y->left; AVLNode* T2=x->right;
    if(traceAVL){cout<<"Xoay phai tai "<<y->data<<"; T2="; if(T2) cout<<T2->data; else cout<<"null"; cout<<'\n';}
    x->right=y; y->left=T2;
    updateHeight(y); updateHeight(x); // Nut xuong truoc, nut len sau.
    return x;
}
AVLNode* leftRotate(AVLNode* x) {
    AVLNode* y=x->right; AVLNode* T2=y->left;
    if(traceAVL){cout<<"Xoay trai tai "<<x->data<<"; T2="; if(T2) cout<<T2->data; else cout<<"null"; cout<<'\n';}
    y->left=x; x->right=T2;
    updateHeight(x); updateHeight(y);
    return y;
}
AVLNode* rebalance(AVLNode* r) {
    if(!r) return r;
    updateHeight(r);
    int b=getBalance(r);
    // Dung BF cua con: dung cho ca chen va xoa, ke ca BF(con)=0.
    if(b < -1) {
        if(traceAVL) {cout<<"Mat can bang "<<(getBalance(r->left)>0?"LR":"LL")<<" tai "<<r->data<<'\n';printTree(r);}
        if(getBalance(r->left)>0) r->left=leftRotate(r->left);
        return rightRotate(r);
    }
    if(b > 1) {
        if(traceAVL) {cout<<"Mat can bang "<<(getBalance(r->right)<0?"RL":"RR")<<" tai "<<r->data<<'\n';printTree(r);}
        if(getBalance(r->right)<0) r->right=rightRotate(r->right);
        return leftRotate(r);
    }
    return r;
}
AVLNode* insertNode(AVLNode* r,int x) {
    if(!r) return new AVLNode(x);
    if(x<r->data) r->left=insertNode(r->left,x);
    else if(x>r->data) r->right=insertNode(r->right,x);
    else return r;
    return rebalance(r);
}
AVLNode* findMin(AVLNode* r) {while(r && r->left) r=r->left; return r;}
AVLNode* deleteNode(AVLNode* r,int x) {
    if(!r) return r;
    if(x<r->data) r->left=deleteNode(r->left,x);
    else if(x>r->data) r->right=deleteNode(r->right,x);
    else {
        if(!r->left || !r->right) {
            AVLNode* child=r->left?r->left:r->right;
            delete r; return child;
        }
        r->data=findMin(r->right)->data;
        r->right=deleteNode(r->right,r->data);
    }
    return rebalance(r);
}
AVLNode* searchAVL(AVLNode* r,int x,int& visits) {
    visits=0;
    while(r) {
        ++visits; // Mot so sanh ba nga khoa tim voi khoa nut; NULL khong dem.
        if(r->data==x) return r;
        r=x<r->data?r->left:r->right;
    }
    return nullptr;
}
// Tinh lai chieu cao, khong tin vao height dang luu. Tra -1 neu sai.
int checkAVL(AVLNode* r,long long lo,long long hi) {
    if(!r) return 0;
    if(r->data<=lo || r->data>=hi) return -1;
    int l=checkAVL(r->left,lo,r->data),h=checkAVL(r->right,r->data,hi);
    if(l<0 || h<0 || abs(h-l)>1 || r->height!=1+max(l,h)) return -1;
    return 1+max(l,h);
}
bool IsAVL(AVLNode* r) {return checkAVL(r,numeric_limits<long long>::min(),numeric_limits<long long>::max())>=0;}
void inOrder(AVLNode* r){if(r){inOrder(r->left);cout<<r->data<<' ';inOrder(r->right);}}
void preOrder(AVLNode* r){if(r){cout<<r->data<<' ';preOrder(r->left);preOrder(r->right);}}
void postOrder(AVLNode* r){if(r){postOrder(r->left);postOrder(r->right);cout<<r->data<<' ';}}
void levelAVL(AVLNode* r){
    if(!r) return;
    queue<AVLNode*> q;q.push(r);
    while(!q.empty()){AVLNode* p=q.front();q.pop();cout<<p->data<<' ';if(p->left)q.push(p->left);if(p->right)q.push(p->right);}
}
void clearAVL(AVLNode*& r){if(r){clearAVL(r->left);clearAVL(r->right);delete r;r=nullptr;}}

int main(){
    traceAVL=true;vector<vector<int>>cases={{30,20,10},{10,20,30},{30,10,20},{10,30,20}};
    vector<string>names={"LL","RR","LR","RL"};
    for(size_t i=0;i<cases.size();++i){AVLNode*r=nullptr;cout<<"CASE "<<names[i]<<'\n';for(int x:cases[i])r=insertNode(r,x);printTree(r);cout<<"IsAVL="<<IsAVL(r)<<'\n';clearAVL(r);}
    AVLNode*a=nullptr;TNode*b=nullptr;
    for(int x=1;x<=7;++x){cout<<"Chen "<<x<<'\n';a=insertNode(a,x);b=Insert(b,x);printTree(a);}
    cout<<"Height AVL="<<getHeight(a)<<" BST="<<Height(b)<<'\n';clearAVL(a);Clear(b);return 0;
}
