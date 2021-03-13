#include<bits/stdc++.h>
using namespace std;
const double e = 1e-6;

map<string, char> mp1;       //记录每个编码对应的data
map<char, string> mp2;       //记录每个data对应的编码

struct node
{
    node()                              //构造函数
    {
        lchild = rchild = 0;
    }
    node(const node& m)
    {
        data = m.data;    mx_data = m.mx_data;
        f = m.f;          num = m.num;
        lchild = m.lchild;   rchild = m.rchild;
    }

    char data, mx_data;   //data记录这个节点的键值, mx_data记录以这个节点为根的子树的最小data
    double f;             //记录频率
    int num;              //记录以这个节点为根的叶子节点个数
    node *lchild, *rchild;
    bool operator<(const node& m) const                   //定义小于号
    {
        if(fabs(f - m.f) > e) return f < m.f;
        if(num != m.num) return num < m.num;
        return data < m.data;
    }
    bool operator>(const node& m) const                        //定义大于号
    {
        return !(*this < m);
    }
};

void encode(node *root, string s)                          //将排好的Huffman树编码
{
    if(root->num == 1)
    {
        mp1[s] = root->data;
        mp2[root->data] = s;
        return;
    }
    encode(root->lchild, s + "0");
    encode(root->rchild, s + "1");
}

void show_code(node *root)                                  //展示编码
{
    if(root->num == 1)
    {
        cout<< root->data << " " << mp2[root->data] <<endl;
        return;
    }
    show_code(root->lchild);
    show_code(root->rchild);
}

void read_code(string s)                                       //处理编码
{
    unsigned int l = 0, len = 1;
    string tmp;
    while(l < s.length())
    {
        while(mp1.find(s.substr(l, len)) == mp1.end()) len++;
        cout<<mp1[s.substr(l, len)];
        l = l + len;
        len = 1;
    }
}

priority_queue<node, vector<node>, greater<node> > q;
int main()
{
    int n;
    cout<<"请在第一行输入节点个数n, 然后n行分别输出每个节点的键值与对应序列"<<endl;
    cin>>n;
    node temp, k1, k2;
    for(int i = 0; i < n; i++)                       //将节点存入优先队列
    {
        cin>>temp.data>>temp.f;
        temp.mx_data = temp.data;
        temp.num = 1;
        q.push(temp);
    }

    for(int i = 0; i < n - 1; i++)                   //建树
    {
        k1 = q.top();  q.pop();
        k2 = q.top();  q.pop();
        temp.mx_data = min(k1.mx_data, k2.mx_data);
        temp.num = k1.num + k2.num;
        temp.f = k1.f + k2.f;
        if(k1 < k2)
        {
            temp.lchild = new node(k1);
            temp.rchild = new node(k2);
        }
        else
        {
            temp.lchild = new node(k2);
            temp.lchild = new node(k1);
        }
        q.push(temp);
    }
    temp = q.top();
    node* root = &temp;

    encode(root, "");                                                       //编码
    cout<<endl<<"已经建好Huffman树，根据规则得到的编码如下："<<endl;
    show_code(root);

    cout<<endl<<"请输入一段二进制串:"<<endl;                             //解码
    string s;
    cin>>s;
    cout<<"解码结果如下:"<<endl;
    read_code(s);
}

/*
8
A 0.4
B 0.15
C 0.15
D 0.1
E 0.1
F 0.05
G 0.04
H 0.01

001101001110010101011110110

5
A 0.1
B 0.2
C 0.05
D 0.2
E 0.25

000101101001
*/
