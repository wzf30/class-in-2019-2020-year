#include <bits/stdc++.h>
using namespace std;

struct node
{
    node()
    {
        lchild = rchild = nullptr;
    }
    int data;
    node *lchild, *rchild;
};

void Insert(node *root, node *k)
{
    node *q, *p = root;                     //pΪ��ǰ�Ƚ�ָ�룬qΪpָ��ڵ�ĸ��ڵ�
    while(p)
    {
        q = p;
        if(p->data > k->data) p = p->lchild;
        else if(p->data < k->data) p = p->rchild;
        else return;                                //���������ȵ�ֵ����������
    }
    if(q->data > k->data) q->lchild = k;
    else q->rchild = k;
}

void midorder(node *root)
{
    if(root != nullptr)
    {
        midorder(root->lchild);
        cout<< root->data << " ";
        midorder(root->rchild);
    }
}

int AVL_cal(node *root, bool *flag)
{
    if(root == nullptr) return 0;
    int x = AVL_cal(root->lchild, flag), y = AVL_cal(root->rchild, flag);
    if(abs(x - y) > 1) *flag = false;
    return max(x, y) + 1;
}

bool isAVL(node *root)
{
    bool flag = true;
    AVL_cal(root, &flag);            //�õݹ�ȥ����ÿ���ڵ��ƽ��ȣ�����ֵ��������ڵ�Ϊ���������ĸ߶�
    return flag;                 //������һ�������㣬��ı�flag��ֵ
}

int cal_compare_time(node *root, int height)                  //height��ʾ���ĸ߶�
{
    if(root == nullptr) return 0;
    return cal_compare_time(root->lchild, height + 1) + cal_compare_time(root->rchild, height + 1) + height + 1;
}

int main()
{
    cout<<"���ڵ�һ������ڵ������ �ڶ���������������ڵ�"<<endl;

    int n;
    cin>>n;
    node *root = new node();            //�������ڵ�
    cin>> root->data;
    for(int i = 0; i < n - 1; i++)       //���β���
    {
        node *k = new node();
        cin>> k->data;
        Insert(root, k);
    }

    cout<<"�������Ϊ�� ";
    midorder(root);                 //�������
    if(isAVL(root)) cout<<endl<<"��ƽ����"<<endl;      //�ж��Ƿ�Ϊƽ����
    else cout<<endl<<"����ƽ����"<<endl;
    cout<<"ƽ���Ƚϴ���Ϊ��"<<cal_compare_time(root, 0) * 1.0 / n<<endl;                     //����Ƚϴ���

}

/*
7
36 27 30 13 42 46 39

7
35 20 38 17 37 39 18
*/
