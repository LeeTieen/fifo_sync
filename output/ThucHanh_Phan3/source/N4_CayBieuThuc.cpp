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
