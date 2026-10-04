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

int main(int argc,char**argv){
    AVLNode*r=nullptr;int c,x,visits;
    if(argc>1 && string(argv[1])=="--demo"){
        traceAVL=true;
        for(int v:{10,20,30,40,50,25}){cout<<"Chen "<<v<<'\n';r=insertNode(r,v);printTree(r);}
        r=insertNode(r,15);r=deleteNode(r,20);printTree(r);inOrder(r);
        cout<<"\nIsAVL="<<IsAVL(r)<<'\n';clearAVL(r);return 0;
    }
    while(true){
        cout<<"\n1 Chen  2 Xoa  3 Tim  4 Cac duyet  5 Ve  6 Kiem AVL  7 Bat/tat trace  8 Xoa het  0 Thoat\n";
        if(!(cin>>c)){if(cin.eof())break;cin.clear();cin.ignore(numeric_limits<streamsize>::max(),'\n');cout<<"Nhap so nguyen.\n";continue;}
        if(!c)break;
        if(c>=1 && c<=3){
            if(!(cin>>x)){cin.clear();cin.ignore(numeric_limits<streamsize>::max(),'\n');cout<<"Khoa sai.\n";continue;}
            bool found=searchAVL(r,x,visits)!=nullptr;
            if(c==1){r=insertNode(r,x);cout<<(found?"Trung khoa\n":"Da chen\n");}
            if(c==2){r=deleteNode(r,x);cout<<(found?"Da xoa\n":"Khong co khoa\n");}
            if(c==3)cout<<"Tim thay="<<found<<" So nut tham="<<visits<<'\n';
        }else if(c==4){cout<<"NLR: ";preOrder(r);cout<<"\nLNR: ";inOrder(r);cout<<"\nLRN: ";postOrder(r);cout<<"\nLevel: ";levelAVL(r);cout<<'\n';}
        else if(c==5)printTree(r);else if(c==6)cout<<"IsAVL="<<IsAVL(r)<<'\n';
        else if(c==7){traceAVL=!traceAVL;cout<<"Trace="<<traceAVL<<'\n';}
        else if(c==8)clearAVL(r);else cout<<"Lua chon sai\n";
    }clearAVL(r);return 0;
}
