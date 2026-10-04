from pathlib import Path
import subprocess,json
root=Path.cwd(); src=root/'output/ThucHanh_Phan3/source'; work=root/'tmp/phan3'; bins=work/'bin';bins.mkdir(exist_ok=True)
compiler=r'D:\MinGW\bin\g++.exe'
records=[]
for p in sorted(src.glob('*.cpp')):
    exe=bins/(p.stem+'.exe')
    cp=subprocess.run([compiler,'-std=c++11','-O2','-Wall','-Wextra','-pedantic',str(p),'-o',str(exe)],capture_output=True,text=True)
    if cp.returncode: raise RuntimeError(p.name+'\n'+cp.stderr)
    args=[str(exe)]
    if p.stem in ('Bai09_BST','Bai10_AVL'):args+=['--demo']
    if p.stem=='Bai09_MoRong':args+=[str(work/'roundtrip.txt')]
    run=subprocess.run(args,input='',capture_output=True,text=True,timeout=30)
    (work/(p.stem+'.txt')).write_text(run.stdout+run.stderr,encoding='utf-8')
    assert run.returncode==0,(p.name,run.stderr)
    records.append({'file':p.name,'compiled':True,'warnings':cp.stderr,'demo_exit':run.returncode,'output':run.stdout})

def harness(name,source,tests):
    p=work/(name+'.cpp'); exe=bins/(name+'.exe')
    p.write_text('#define main original_main\n#include "'+source.as_posix()+'"\n#undef main\n#include <cassert>\n'+tests,encoding='utf-8')
    c=subprocess.run([compiler,'-std=c++11','-O2','-Wall','-Wextra','-pedantic',str(p),'-o',str(exe)],capture_output=True,text=True)
    assert c.returncode==0,c.stderr
    r=subprocess.run([str(exe)],capture_output=True,text=True,timeout=60,cwd=work)
    assert r.returncode==0,(name,r.stdout,r.stderr)
    print(name,r.stdout.strip())

harness('test_bst',src/'Bai09_MoRong.cpp',r'''
int main(){
    TNode*r=nullptr;assert(IsBST(r)&&IsComplete(r)&&!FindClosest(r,0)&&!KthSmallest(r,1));
    r=Insert(r,numeric_limits<int>::min());r=Insert(r,numeric_limits<int>::max());assert(IsBST(r));
    assert(FindClosest(r,0)->data==numeric_limits<int>::max());Clear(r);
    r=Sample();assert(Height(r)==4&&CountNodes(r)==9&&FindClosest(r,5)->data==4);
    assert(!IsComplete(r)&&!KthSmallest(r,0)&&!KthSmallest(r,10)&&KthSmallest(r,4)->data==6);
    r=Delete(r,1);r=Delete(r,14);r=Delete(r,8);vector<int>a;Collect(r,a);assert((a==vector<int>{3,4,6,7,10,13})&&r->data==10);Clear(r);
    r=new TNode(8);r->pLeft=new TNode(3);r->pRight=new TNode(10);r->pLeft->pRight=new TNode(9);assert(!IsBST(r));Clear(r);
    r=new TNode(1);r->pRight=new TNode(2);assert(!IsComplete(r));Clear(r);
    vector<int>ref;mt19937 gen(2026);
    for(int i=0;i<5000;++i){int x=static_cast<int>(gen()%201)-100;
        if(gen()%2){r=Insert(r,x);if(find(ref.begin(),ref.end(),x)==ref.end())ref.push_back(x);}
        else{r=Delete(r,x);ref.erase(remove(ref.begin(),ref.end(),x),ref.end());}
        sort(ref.begin(),ref.end());a.clear();Collect(r,a);assert(a==ref&&IsBST(r));
    }
    SaveTree(r,"test_tree.txt");TNode*copy=LoadTree("test_tree.txt");assert(SameTree(r,copy));Clear(copy);
    r=BalanceBST(r);a.clear();Collect(r,a);assert(a==ref&&IsBST(r));Clear(r);
    for(string data:{"8 3 10 9 2","1 1","abc","2147483648"}){ofstream f("bad_tree.txt");f<<data;f.close();bool bad=false;try{copy=LoadTree("bad_tree.txt");Clear(copy);}catch(const exception&){bad=true;}assert(bad);}
    SaveTree(nullptr,"empty_tree.txt");copy=LoadTree("empty_tree.txt");assert(!copy);
    cout<<"PASS 5000 mixed BST operations and edge/file tests\n";return 0;
}''')
harness('test_avl',src/'Bai10_AVL.cpp',r'''
void collect(AVLNode*r,vector<int>&a){if(r){collect(r->left,a);a.push_back(r->data);collect(r->right,a);}}
int main(){
    AVLNode*r=nullptr;assert(IsAVL(r));
    for(auto v:vector<vector<int>>{{30,20,10},{10,20,30},{30,10,20},{10,30,20}}){for(int x:v)r=insertNode(r,x);assert(r->data==20&&IsAVL(r));clearAVL(r);}
    for(int x:{4,2,5,1,3})r=insertNode(r,x);r=deleteNode(r,5);assert(IsAVL(r)&&r->data==2);clearAVL(r);
    for(int x:{2,1,4,3,5})r=insertNode(r,x);r=deleteNode(r,1);assert(IsAVL(r)&&r->data==4);clearAVL(r);
    r=insertNode(r,numeric_limits<int>::min());r=insertNode(r,numeric_limits<int>::max());assert(IsAVL(r));++r->height;assert(!IsAVL(r));--r->height;clearAVL(r);
    vector<int>ref;mt19937 gen(42);
    for(int i=0;i<10000;++i){int x=static_cast<int>(gen()%501)-250;
        if(gen()%2){r=insertNode(r,x);if(find(ref.begin(),ref.end(),x)==ref.end())ref.push_back(x);}
        else{r=deleteNode(r,x);ref.erase(remove(ref.begin(),ref.end(),x),ref.end());}
        sort(ref.begin(),ref.end());vector<int>a;collect(r,a);assert(a==ref&&IsAVL(r));
    }
    for(int x:ref){r=deleteNode(r,x);assert(IsAVL(r));}assert(!r);
    cout<<"PASS 10000 mixed AVL operations, four rotations, child BF=0 deletion, stale height\n";return 0;
}''')
harness('test_duplicate',src/'Bai09_KhoaTrung.cpp',r'''
int main(){Node*r=nullptr;for(int x:{8,3,10,9,9})r=Insert(r,x);r=Delete(r,8);assert(r->data==9&&r->count==2&&r->right->left==nullptr);r=Delete(r,9);assert(r->count==1);r=Delete(r,9);assert(!Search(r,9));Clear(r);cout<<"PASS successor with count=2\n";return 0;}''')
harness('test_rebuild',src/'N7_DungCay.cpp',r'''
int main(){TNode*r=BuildFromTraversals({1,2,4,5,3},{4,2,5,1,3});assert(r->pLeft->pRight->data==5);Clear(r);assert(!BuildFromTraversals({},{}));int errors=0;
    for(auto in:vector<vector<int>>{{1,2},{1,1,3},{3,1,2}}){try{r=BuildFromTraversals({1,2,3},in);Clear(r);}catch(const exception&){++errors;}}assert(errors==3);cout<<"PASS traversal reconstruction and invalid inputs\n";return 0;}''')
expr=bins/'N4_CayBieuThuc.exe'
for data,ok,expected in [('2 3 + 4 *',True,'20'),('2 3 4 * +',True,'14'),('8 2 -',True,'6'),('8 2 /',True,'4'),('2 +',False,''),('2 3',False,''),('2 0 /',False,''),('',False,''),('abc',False,'')]:
    r=subprocess.run([str(expr),data],capture_output=True,text=True)
    assert (r.returncode==0)==ok,(data,r.stdout,r.stderr)
    if ok: assert 'Gia tri: '+expected in r.stdout
print('test_expression PASS 9 valid/invalid inputs')
for stem in ['Bai09_BST','Bai10_AVL']:
    r=subprocess.run([str(bins/(stem+'.exe'))],input='abc\n1\nabc\n1\n8\n1\n8\n3\n8\n2\n99\n0\n',capture_output=True,text=True,timeout=5)
    assert r.returncode==0
print('test_menus PASS malformed input, duplicate, missing key, exit')
(work/'results.json').write_text(json.dumps(records,ensure_ascii=False,indent=2),encoding='utf-8')
print('ALL PASS:',len(records),'standalone programs')
