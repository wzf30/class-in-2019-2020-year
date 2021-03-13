#include<bits/stdc++.h>
using namespace std;
const double e = 1e-6;

map<string, char> mp1;       //��¼ÿ�������Ӧ��data
map<char, string> mp2;       //��¼ÿ��data��Ӧ�ı���

struct node
{
    node()                              //���캯��
    {
        lchild = rchild = 0;
    }
    node(const node& m)
    {
        data = m.data;    mx_data = m.mx_data;
        f = m.f;          num = m.num;
        lchild = m.lchild;   rchild = m.rchild;
    }

    char data, mx_data;   //data��¼����ڵ�ļ�ֵ, mx_data��¼������ڵ�Ϊ������������Сdata
    double f;             //��¼Ƶ��
    int num;              //��¼������ڵ�Ϊ����Ҷ�ӽڵ����
    node *lchild, *rchild;
    bool operator<(const node& m) const                   //����С�ں�
    {
        if(fabs(f - m.f) > e) return f < m.f;
        if(num != m.num) return num < m.num;
        return data < m.data;
    }
    bool operator>(const node& m) const                        //������ں�
    {
        return !(*this < m);
    }
};

void encode(node *root, string s)                          //���źõ�Huffman������
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

void show_code(node *root)                                  //չʾ����
{
    if(root->num == 1)
    {
        cout<< root->data << " " << mp2[root->data] <<endl;
        return;
    }
    show_code(root->lchild);
    show_code(root->rchild);
}

void read_code(string s)                                       //�������
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
    cout<<"���ڵ�һ������ڵ����n, Ȼ��n�зֱ����ÿ���ڵ�ļ�ֵ���Ӧ����"<<endl;
    cin>>n;
    node temp, k1, k2;
    for(int i = 0; i < n; i++)                       //���ڵ�������ȶ���
    {
        cin>>temp.data>>temp.f;
        temp.mx_data = temp.data;
        temp.num = 1;
        q.push(temp);
    }

    for(int i = 0; i < n - 1; i++)                   //����
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

    encode(root, "");                                                       //����
    cout<<endl<<"�Ѿ�����Huffman�������ݹ���õ��ı������£�"<<endl;
    show_code(root);

    cout<<endl<<"������һ�ζ����ƴ�:"<<endl;                             //����
    string s;
    cin>>s;
    cout<<"����������:"<<endl;
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
