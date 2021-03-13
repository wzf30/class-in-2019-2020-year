#include <bits/stdc++.h>
#define M 100
using namespace std;
int a[100];

struct node
{
    int data;
    node *rchild, *lchild;
};

int siftdown(int i, int n)        //调整以a[i]为根节点的堆,返回值为交换次数
{
    int j, t = a[i], cnt = 0;
    while((j = 2 * i + 1) < n)          //判断是否有子节点
    {
        if(j < n - 1 && a[j] < a[j+1]) j++;    //确定左右节点较大值的下标
        if(t < a[j])
        {
            a[i] = a[j];
            cnt++;
            i = j;                             //i为当前调整到的位置
        }
        else break;
    }
    a[i] = t;
    return cnt;
}

int build(int n)                     //建堆
{
    int cnt = 0;
    for(int i = (n - 2) / 2; i >= 0; i--)
        cnt += siftdown(i, n);
    return cnt;
}

int heap_sort(int n)
{
    int cnt = 0, t;
    for(int i = n - 1; i > 0; i--)       //每次将最大节点调整到最后面
    {
        t = a[0];
        a[0] = a[i];
        a[i] = t;
        cnt++;
        cnt += siftdown(0, i);
    }
    return cnt;
}

int main()
{
    int n;
    cout<<"请在第一行输入节点个数，第二行依次输入各个节点(键值为int)"<<endl;

    cin>>n;
    for(int i = 0; i < n; i++)
        cin>>a[i];

    cout<<"建堆时交换次数： "<<build(n)<<endl;
    int cnt = heap_sort(n);
    cout<<"堆排序好的序列："<<endl;
    for(int i = 0; i < n; i++) cout<<a[i]<<" ";
    cout<<endl<<"交换调整时的比较次数: "<<cnt<<endl;
}

/*
7
7 6 5 2 1 3 4

7
1 2 3 4 5 6 7
*/
