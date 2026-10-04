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

TNode* FindClosest(TNode* r,int x) {
    TNode* best=nullptr; long long dist=numeric_limits<long long>::max();
    while(r){
        long long d=llabs(static_cast<long long>(r->data)-x);
        if(d<dist || (d==dist && r->data<best->data)){best=r;dist=d;}
        if(r->data==x) break;
        r=x<r->data?r->pLeft:r->pRight;
    }
    return best; // Hoa khoang cach: chon khoa nho hon. Cay rong: nullptr.
}
bool IsComplete(TNode* r) {
    if(!r) return true;
    queue<TNode*> q; q.push(r); bool gap=false;
    while(!q.empty()){
        TNode* p=q.front();q.pop();
        if(!p){gap=true;continue;}
        if(gap) return false;
        q.push(p->pLeft);q.push(p->pRight);
    }
    return true;
}
void Collect(TNode* r,vector<int>& a){if(r){Collect(r->pLeft,a);a.push_back(r->data);Collect(r->pRight,a);}}
TNode* BuildBalanced(const vector<int>& a,int l,int r){
    if(l>=r) return nullptr;
    int m=l+(r-l)/2; TNode* p=new TNode(a[m]);
    p->pLeft=BuildBalanced(a,l,m);p->pRight=BuildBalanced(a,m+1,r);return p;
}
// So huu cay duoc chuyen cho ham; phai gan root=BalanceBST(root).
TNode* BalanceBST(TNode* r){vector<int>a;Collect(r,a);Clear(r);return BuildBalanced(a,0,static_cast<int>(a.size()));}
void WritePre(TNode* r,ostream& f){if(r){f<<r->data<<' ';WritePre(r->pLeft,f);WritePre(r->pRight,f);}}
void SaveTree(TNode* r,const string& name){
    ofstream f(name);if(!f)throw runtime_error("Khong mo duoc file ghi");
    WritePre(r,f);f<<'\n';f.close();if(!f)throw runtime_error("Ghi file that bai");
}
// Dung BST duy nhat tu NLR (khong trung), O(n); file hong bi tu choi.
TNode* ReadPre(const vector<int>& a,size_t& i,long long lo,long long hi){
    if(i==a.size() || a[i]<=lo || a[i]>=hi)return nullptr;
    TNode* r=new TNode(a[i++]);
    r->pLeft=ReadPre(a,i,lo,r->data);r->pRight=ReadPre(a,i,r->data,hi);return r;
}
TNode* LoadTree(const string& name){
    ifstream f(name);if(!f)throw runtime_error("Khong mo duoc file doc");
    vector<int>a; string token;
    while(f>>token){istringstream s(token);int x;char tail;if(!(s>>x) || (s>>tail))throw runtime_error("Token khong hop le");a.push_back(x);}
    if(f.bad())throw runtime_error("Doc file that bai");
    size_t i=0;TNode*r=ReadPre(a,i,numeric_limits<long long>::min(),numeric_limits<long long>::max());
    if(i!=a.size()){Clear(r);throw runtime_error("Day NLR sai hoac trung khoa");}
    return r;
}
void RangeSearch(TNode*r,int lo,int hi){
    if(!r || lo>hi)return;
    if(lo<r->data)RangeSearch(r->pLeft,lo,hi);
    if(lo<=r->data && r->data<=hi)cout<<r->data<<' ';
    if(hi>r->data)RangeSearch(r->pRight,lo,hi);
}
TNode* KthSmallest(TNode*r,int k){
    if(k<=0)return nullptr;
    stack<TNode*> s;
    while(r || !s.empty()){
        while(r){s.push(r);r=r->pLeft;}
        r=s.top();s.pop();if(--k==0)return r;r=r->pRight;
    }return nullptr;
}
TNode* CopyTree(TNode*r){if(!r)return nullptr;TNode*p=new TNode(r->data);p->pLeft=CopyTree(r->pLeft);p->pRight=CopyTree(r->pRight);return p;}
bool SameTree(TNode*a,TNode*b){if(!a || !b)return a==b;return a->data==b->data && SameTree(a->pLeft,b->pLeft) && SameTree(a->pRight,b->pRight);}
void DrawTree(TNode*r){PrintTree(r);}

int main(int argc,char**argv){
    // File dau ra tuy chon; mac dinh tao cay_bst.txt trong thu muc chay.
    string path=argc>1?argv[1]:"cay_bst.txt";
    TNode*r=Sample(),*copy=nullptr;
    try{
        cout<<"Gan 5 nhat: "<<FindClosest(r,5)->data<<"\nMin="<<FindMin(r)->data<<" Max="<<FindMax(r)->data<<'\n';
        cout<<"Complete="<<IsComplete(r)<<"\nRange [4,10]: ";RangeSearch(r,4,10);
        cout<<"\nK=4: "<<KthSmallest(r,4)->data<<'\n';
        copy=CopyTree(r);cout<<"Copy dung="<<SameTree(r,copy)<<'\n';Clear(copy);
        SaveTree(r,path);copy=LoadTree(path);cout<<"Luu/doc giu hinh="<<SameTree(r,copy)<<'\n';Clear(copy);
        Clear(r);for(int x=1;x<=7;++x)r=Insert(r,x);
        cout<<"Height truoc="<<Height(r)<<'\n';r=BalanceBST(r);
        cout<<"Height sau="<<Height(r)<<" Complete="<<IsComplete(r)<<'\n';DrawTree(r);
    }catch(const exception&e){cerr<<e.what()<<'\n';Clear(r);Clear(copy);return 1;}
    Clear(r);return 0;
}
