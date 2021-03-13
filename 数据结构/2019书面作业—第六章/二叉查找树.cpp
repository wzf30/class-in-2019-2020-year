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
    node *q, *p = root;                     //p为当前比较指针，q为p指向节点的父节点
    while(p)
    {
        q = p;
        if(p->data > k->data) p = p->lchild;
        else if(p->data < k->data) p = p->rchild;
        else return;                                //如果存在相等的值，函数结束
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
    AVL_cal(root, &flag);            //用递归去计算每个节点的平衡度，返回值是以这个节点为根的子树的高度
    return flag;                 //若是有一个不满足，则改变flag的值
}

int cal_compare_time(node *root, int height)                  //height表示树的高度
{
    if(root == nullptr) return 0;
    return cal_compare_time(root->lchild, height + 1) + cal_compare_time(root->rchild, height + 1) + height + 1;
}

int main()
{
    cout<<"请在第一行输入节点个数， 第二行依次输入各个节点"<<endl;

    int n;
    cin>>n;
    node *root = new node();            //建立根节点
    cin>> root->data;
    for(int i = 0; i < n - 1; i++)       //依次插入
    {
        node *k = new node();
        cin>> k->data;
        Insert(root, k);
    }

    cout<<"中序遍历为： ";
    midorder(root);                 //中序输出
    if(isAVL(root)) cout<<endl<<"是平衡树"<<endl;      //判断是否为平衡树
    else cout<<endl<<"不是平衡树"<<endl;
    cout<<"平均比较次数为："<<cal_compare_time(root, 0) * 1.0 / n<<endl;                     //输出比较次数

}

/*
7
36 27 30 13 42 46 39

7
35 20 38 17 37 39 18
*/
