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

TNode* BuildRec(const vector<int>&pre,const vector<int>&in,size_t&pos,int l,int h){
    if(l>=h)return nullptr;
    if(pos==pre.size())throw runtime_error("Thieu khoa NLR");
    int x=pre[pos++],m=l;while(m<h&&in[m]!=x)++m;
    if(m==h)throw runtime_error("Hai day khong tuong thich");
    TNode*r=new TNode(x);
    try{r->pLeft=BuildRec(pre,in,pos,l,m);r->pRight=BuildRec(pre,in,pos,m+1,h);}
    catch(...){Clear(r);throw;}return r;
}
TNode* BuildFromTraversals(const vector<int>&pre,const vector<int>&in){
    if(pre.size()!=in.size())throw runtime_error("Khac so nut");
    vector<int>a=pre,b=in;sort(a.begin(),a.end());sort(b.begin(),b.end());
    if(a!=b || adjacent_find(a.begin(),a.end())!=a.end())throw runtime_error("Khac tap khoa hoac trung khoa");
    size_t pos=0;return BuildRec(pre,in,pos,0,static_cast<int>(in.size()));
}
int main(){
    TNode*r=BuildFromTraversals({1,2,4,5,3},{4,2,5,1,3});PrintTree(r);AllOrders(r);Clear(r);
    TNode*b=Sample();ostringstream out;WritePre(b,out);istringstream inp(out.str());int x;
    while(inp>>x)r=Insert(r,x);
    cout<<"BST dung lai tu NLR giu hinh="<<SameTree(b,r)<<'\n';Clear(b);Clear(r);return 0;
}
