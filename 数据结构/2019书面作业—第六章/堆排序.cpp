#include <bits/stdc++.h>
#define M 100
using namespace std;
int a[100];

struct node
{
    int data;
    node *rchild, *lchild;
};

int siftdown(int i, int n)        //������a[i]Ϊ���ڵ�Ķ�,����ֵΪ��������
{
    int j, t = a[i], cnt = 0;
    while((j = 2 * i + 1) < n)          //�ж��Ƿ����ӽڵ�
    {
        if(j < n - 1 && a[j] < a[j+1]) j++;    //ȷ�����ҽڵ�ϴ�ֵ���±�
        if(t < a[j])
        {
            a[i] = a[j];
            cnt++;
            i = j;                             //iΪ��ǰ��������λ��
        }
        else break;
    }
    a[i] = t;
    return cnt;
}

int build(int n)                     //����
{
    int cnt = 0;
    for(int i = (n - 2) / 2; i >= 0; i--)
        cnt += siftdown(i, n);
    return cnt;
}

int heap_sort(int n)
{
    int cnt = 0, t;
    for(int i = n - 1; i > 0; i--)       //ÿ�ν����ڵ�����������
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
    cout<<"���ڵ�һ������ڵ�������ڶ���������������ڵ�(��ֵΪint)"<<endl;

    cin>>n;
    for(int i = 0; i < n; i++)
        cin>>a[i];

    cout<<"����ʱ���������� "<<build(n)<<endl;
    int cnt = heap_sort(n);
    cout<<"������õ����У�"<<endl;
    for(int i = 0; i < n; i++) cout<<a[i]<<" ";
    cout<<endl<<"��������ʱ�ıȽϴ���: "<<cnt<<endl;
}

/*
7
7 6 5 2 1 3 4

7
1 2 3 4 5 6 7
*/
