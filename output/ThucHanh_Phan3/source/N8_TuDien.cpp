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
