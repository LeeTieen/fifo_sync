from pathlib import Path

OUT=Path('output/ThucHanh_Phan3/source')
HEAD='''// Thuc hanh CTDL va GT - Phan 3. Moi file bien dich doc lap: g++ -std=c++11 file.cpp -o app
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
'''
BST=r'''
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
'''
AVL=r'''
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
'''
EXTRA=r'''
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
'''
def write(name,body):
    (OUT/name).write_text(HEAD+body,encoding='utf-8')

write('Bai09_BST.cpp', BST+r'''
int main(int argc,char** argv){
    TNode*root=nullptr;
    if(argc>1 && string(argv[1])=="--demo"){
        root=Sample();PrintTree(root);AllOrders(root);
        cout<<"Nodes="<<CountNodes(root)<<" Height="<<Height(root)<<" IsBST="<<IsBST(root)<<'\n';
        for(int x:{1,14,8}){root=Delete(root,x);cout<<"Xoa "<<x<<": ";LNR(root);cout<<'\n';}
        Clear(root);return 0;
    }
    int c,x;
    while(true){
        cout<<"\n1 Chen  2 Xoa  3 Tim  4 NLR  5 LNR  6 LRN  7 Level\n8 Ve cay  9 Thong ke  10 Kiem BST  11 Nap mau  12 Xoa het  0 Thoat\nChon: ";
        if(!(cin>>c)){if(cin.eof())break;cin.clear();cin.ignore(numeric_limits<streamsize>::max(),'\n');cout<<"Nhap so nguyen.\n";continue;}
        if(c==0)break;
        if(c>=1 && c<=3){
            cout<<"Khoa: ";if(!(cin>>x)){cin.clear();cin.ignore(numeric_limits<streamsize>::max(),'\n');cout<<"Khoa sai.\n";continue;}
            bool found=SearchIterative(root,x)!=nullptr;
            if(c==1){root=Insert(root,x);cout<<(found?"Khoa trung, khong chen.\n":"Da chen.\n");}
            if(c==2){root=Delete(root,x);cout<<(found?"Da xoa.\n":"Khong co khoa.\n");}
            if(c==3)cout<<(found?"Tim thay.\n":"Khong tim thay.\n");
        }else switch(c){
            case 4:NLR(root);break;case 5:LNR(root);break;case 6:LRN(root);break;case 7:LevelOrder(root);break;
            case 8:PrintTree(root);break;
            case 9:cout<<"Rong="<<IsEmpty(root)<<" Nodes="<<CountNodes(root)<<" Height="<<Height(root);break;
            case 10:cout<<"IsBST="<<IsBST(root);break;
            case 11:Clear(root);root=Sample();PrintTree(root);break;
            case 12:Clear(root);break;default:cout<<"Lua chon sai.";
        }
        cout<<'\n';
    }Clear(root);return 0;
}
''')
write('Bai09_MoRong.cpp',BST+EXTRA+r'''
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
''')
write('Bai10_AVL.cpp',AVL+r'''
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
''')
write('Bai11_SoSanh.cpp',BST+AVL+r'''
int searchBSTCount(TNode*r,int x){int c=0;while(r){++c;if(x==r->data)break;r=x<r->data?r->pLeft:r->pRight;}return c;}
void Compare(const vector<int>&a,const string&label,int x){
    TNode*b=nullptr;AVLNode*v=nullptr;
    for(int k:a){b=Insert(b,k);v=insertNode(v,k);}
    int vc=0;searchAVL(v,x,vc);
    cout<<label<<" n="<<a.size()<<" target="<<x<<" hBST="<<Height(b)<<" hAVL="<<getHeight(v)
        <<" cmpBST="<<searchBSTCount(b,x)<<" cmpAVL="<<vc<<'\n';
    if(a.size()<=10){PrintTree(b);printTree(v);}
    Clear(b);clearAVL(v);
}
int main(){
    cout<<"Dem mot so sanh ba nga moi nut, khong dem NULL; h rong=0.\n";
    Compare({1,3,5,7,9,11,13,15,17,19},"Mau",17);
    for(int n:{100,1000}){
        vector<int>a(n);iota(a.begin(),a.end(),1);
        Compare(a,"Tang",n);Compare(a,"Khong thay",n+1);
        reverse(a.begin(),a.end());Compare(a,"Giam",1);
        for(int seed:{7,42,2026}){mt19937 rng(seed);shuffle(a.begin(),a.end(),rng);Compare(a,"Tron seed="+to_string(seed),n);}
    }return 0;
}
''')
write('N1_ThuatNgu_Duyet.cpp',BST+r'''
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
''')
write('N2_Chen_KiemBST.cpp',BST+r'''
int main(){
    TNode*r=nullptr;for(int x:{8,3,10,1,6,4,14,13}){r=Insert(r,x);cout<<"Chen "<<x<<'\n';PrintTree(r);}
    r=Insert(r,8);AllOrders(r);cout<<"IsBST="<<IsBST(r)<<" Nodes="<<CountNodes(r)<<'\n';Clear(r);
    r=new TNode(8);r->pLeft=new TNode(3);r->pRight=new TNode(10);r->pLeft->pRight=new TNode(9);
    PrintTree(r);cout<<"Phan vi du IsBST="<<IsBST(r)<<'\n';Clear(r);return 0;
}
''')
write('N3_XoaBST.cpp',BST+r'''
int main(){TNode*r=Sample();PrintTree(r);for(int x:{1,14,8}){
    if(x==8)cout<<"The mang goc: "<<FindMin(r->pRight)->data<<'\n';
    r=Delete(r,x);cout<<"Sau xoa "<<x<<'\n';PrintTree(r);cout<<"LNR: ";LNR(r);cout<<"\nIsBST="<<IsBST(r)<<'\n';
}Clear(r);return 0;}
''')
write('N5_BonXoay.cpp',BST+AVL+r'''
int main(){
    traceAVL=true;vector<vector<int>>cases={{30,20,10},{10,20,30},{30,10,20},{10,30,20}};
    vector<string>names={"LL","RR","LR","RL"};
    for(size_t i=0;i<cases.size();++i){AVLNode*r=nullptr;cout<<"CASE "<<names[i]<<'\n';for(int x:cases[i])r=insertNode(r,x);printTree(r);cout<<"IsAVL="<<IsAVL(r)<<'\n';clearAVL(r);}
    AVLNode*a=nullptr;TNode*b=nullptr;
    for(int x=1;x<=7;++x){cout<<"Chen "<<x<<'\n';a=insertNode(a,x);b=Insert(b,x);printTree(a);}
    cout<<"Height AVL="<<getHeight(a)<<" BST="<<Height(b)<<'\n';clearAVL(a);Clear(b);return 0;
}
''')
write('N6_Mang_TinhChat.cpp',BST+r'''
void CountTypes(TNode*r,int&n0,int&n1,int&n2){if(!r)return;int c=(r->pLeft?1:0)+(r->pRight?1:0);if(c==0)++n0;else if(c==1)++n1;else ++n2;CountTypes(r->pLeft,n0,n1,n2);CountTypes(r->pRight,n0,n1,n2);}
int main(){
    vector<int>a={10,20,30,40,50,60};
    for(size_t i=0;i<a.size();++i){cout<<"i="<<i<<" value="<<a[i]<<" L=";if(2*i+1<a.size())cout<<2*i+1;else cout<<"null";cout<<" R=";if(2*i+2<a.size())cout<<2*i+2;else cout<<"null";cout<<" Parent=";if(i)cout<<(i-1)/2;else cout<<"null";cout<<'\n';}
    TNode*r=Sample();int n0=0,n1=0,n2=0;CountTypes(r,n0,n1,n2);
    cout<<"n0="<<n0<<" n1="<<n1<<" n2="<<n2<<" n0=n2+1: "<<(n0==n2+1)<<'\n';Clear(r);return 0;
}
''')
write('N7_DungCay.cpp',BST+EXTRA+r'''
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
''')
write('Bai09_KhoaTrung.cpp',r'''
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
''')
write('N4_CayBieuThuc.cpp',r'''
struct Expr{string token;Expr*left,*right;explicit Expr(const string&t):token(t),left(nullptr),right(nullptr){}};
void Clear(Expr*&r){if(r){Clear(r->left);Clear(r->right);delete r;r=nullptr;}}
// Stack con tro hai lop: Cell so huu cay neu con nam trong stack.
class Stack{
    struct Cell{Expr*tree;Cell*next;Cell(Expr*t,Cell*n):tree(t),next(n){}};
    Cell*head=nullptr;size_t n=0;
public:
    Stack()=default;Stack(const Stack&)=delete;Stack&operator=(const Stack&)=delete;
    ~Stack(){while(head){Expr*t=pop();Clear(t);}}
    size_t size()const{return n;}
    void push(Expr*t){head=new Cell(t,head);++n;}
    Expr*pop(){if(!head)throw runtime_error("Stack rong");Cell*c=head;Expr*t=c->tree;head=c->next;delete c;--n;return t;}
};
bool IsOp(const string&t){return t=="+"||t=="-"||t=="*"||t=="/";}
double Number(const string&t){istringstream s(t);double x;char c;if(!(s>>x)||(s>>c)||!isfinite(x))throw runtime_error("So khong hop le: "+t);return x;}
Expr*BuildExpression(const string&text){
    Stack s;istringstream input(text);string t;
    while(input>>t){
        if(IsOp(t)){
            if(s.size()<2)throw runtime_error("Thieu toan hang");
            Expr*r=new Expr(t);r->right=s.pop();r->left=s.pop();s.push(r);
        }else{Number(t);s.push(new Expr(t));}
    }
    if(s.size()!=1)throw runtime_error("Bieu thuc rong hoac thua toan hang");
    return s.pop();
}
void Infix(Expr*r){if(!r)return;if(IsOp(r->token))cout<<'(';Infix(r->left);cout<<r->token;Infix(r->right);if(IsOp(r->token))cout<<')';}
void Postfix(Expr*r){if(r){Postfix(r->left);Postfix(r->right);cout<<r->token<<' ';}}
void Prefix(Expr*r){if(r){cout<<r->token<<' ';Prefix(r->left);Prefix(r->right);}}
double Evaluate(Expr*r){
    if(!r)throw runtime_error("Cay rong");
    if(!IsOp(r->token))return Number(r->token);
    double l=Evaluate(r->left),v=Evaluate(r->right),result;
    if(r->token=="+")result=l+v;else if(r->token=="-")result=l-v;else if(r->token=="*")result=l*v;
    else{if(v==0)throw runtime_error("Chia cho 0");result=l/v;}
    if(!isfinite(result))throw runtime_error("Ket qua vuot mien double");
    return result;
}
void Draw(Expr*r,string pre="",string edge="ROOT "){if(r){cout<<pre<<edge<<r->token<<'\n';Draw(r->left,pre+"|   ","L--- ");Draw(r->right,pre+"|   ","R--- ");}}
int main(int argc,char**argv){
    string text="2 3 + 4 *";if(argc>1){text="";for(int i=1;i<argc;++i){if(i>1)text+=' ';text+=argv[i];}}
    Expr*r=nullptr;
    try{r=BuildExpression(text);Draw(r);cout<<"LNR: ";Infix(r);cout<<"\nLRN: ";Postfix(r);cout<<"\nNLR: ";Prefix(r);cout<<"\nGia tri: "<<Evaluate(r)<<'\n';}
    catch(const exception&e){cerr<<"Loi: "<<e.what()<<'\n';Clear(r);return 1;}
    Clear(r);return 0;
}
''')
write('N8_TuDien.cpp',r'''
// Cung thuat toan con tro BST/AVL, khoa string, gia tri la nghia cua tu.
struct WordNode{string key,meaning;WordNode*left,*right;int height;
    WordNode(const string&k,const string&m):key(k),meaning(m),left(nullptr),right(nullptr),height(1){}};
int H(WordNode*r){return r?r->height:0;}
void Update(WordNode*r){r->height=1+max(H(r->left),H(r->right));}
int BF(WordNode*r){return H(r->right)-H(r->left);}
WordNode*Right(WordNode*y){WordNode*x=y->left,*t=x->right;x->right=y;y->left=t;Update(y);Update(x);return x;}
WordNode*Left(WordNode*x){WordNode*y=x->right,*t=y->left;y->left=x;x->right=t;Update(x);Update(y);return y;}
WordNode*Add(WordNode*r,const string&key,const string&meaning,bool avl){
    if(!r)return new WordNode(key,meaning);
    if(key<r->key)r->left=Add(r->left,key,meaning,avl);
    else if(key>r->key)r->right=Add(r->right,key,meaning,avl);
    else{r->meaning=meaning;return r;}
    Update(r);
    if(avl&&BF(r)<-1){if(BF(r->left)>0)r->left=Left(r->left);return Right(r);}
    if(avl&&BF(r)>1){if(BF(r->right)<0)r->right=Right(r->right);return Left(r);}
    return r;
}
WordNode*Find(WordNode*r,const string&key,int&c){c=0;while(r){++c;if(key==r->key)return r;r=key<r->key?r->left:r->right;}return nullptr;}
void List(WordNode*r){if(r){List(r->left);cout<<r->key<<": "<<r->meaning<<'\n';List(r->right);}}
void Clear(WordNode*&r){if(r){Clear(r->left);Clear(r->right);delete r;r=nullptr;}}
int main(){
    vector<pair<string,string>>words={{"meo","dong vat bat chuot"},{"cho","vat nuoi giu nha"},{"sach","tap trang de doc"},{"ban","do vat de ngoi hoc"},{"hoa","bo phan sinh san cua cay"},{"ong","con trung lay mat"},{"xe","phuong tien di chuyen"},{"ao","do mac than tren"},{"ga","gia cam"},{"lua","cay luong thuc"},{"nha","noi o"},{"tre","cay than dot"}};
    vector<pair<string,string>>more={{"bo","gia suc"},{"ca","dong vat duoi nuoc"},{"dao","cay an qua"},{"ech","dong vat luong cu"},{"gio","khong khi chuyen dong"},{"hat","bo phan de gieo"},{"kem","mon an lanh"},{"lon","gia suc lay thit"}};
    WordNode*b=nullptr,*a=nullptr;
    for(const auto&w:words){b=Add(b,w.first,w.second,false);a=Add(a,w.first,w.second,true);}
    cout<<"12 tu: BST="<<H(b)<<" AVL="<<H(a)<<'\n';
    for(const auto&w:more){b=Add(b,w.first,w.second,false);a=Add(a,w.first,w.second,true);}
    cout<<"Them 8 tu tang alphabet: BST="<<H(b)<<" AVL="<<H(a)<<'\n';List(a);
    int cb,ca;Find(b,"lon",cb);auto hit=Find(a,"lon",ca);cout<<"Tim lon: "<<(hit?hit->meaning:"khong co")<<"; cmpBST="<<cb<<" cmpAVL="<<ca<<'\n';
    Clear(b);Clear(a);
    // Thi nghiem doi chung: chen cung 20 tu vao cay rong theo alphabet.
    words.insert(words.end(),more.begin(),more.end());sort(words.begin(),words.end());
    for(const auto&w:words){b=Add(b,w.first,w.second,false);a=Add(a,w.first,w.second,true);}
    cout<<"20 tu tang tu cay rong: BST="<<H(b)<<" AVL="<<H(a)<<'\n';Clear(b);Clear(a);return 0;
}
''')
print('Created',len(list(OUT.glob('*.cpp'))),'standalone CPP files')
