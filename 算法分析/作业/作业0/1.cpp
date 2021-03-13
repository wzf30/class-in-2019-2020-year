#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int maxn = 1e7 + 20;

//num����ļ�����������ݣ�ÿ��ȡ��ͬkʱ����Ҫ��num�����ݿ�����a����
//���ڹ鲢�Ĺ����У���Ҫ�õ��������飬����a�����b����ѭ��ʹ��
//k������鲢ʱ��Left��Right˫ָ������ʾĳ�����黹δ��ѵķ�Χ
//h����С�ѣ���¼������С������h2��¼����h��Ӧ���������ĸ�����
int num[maxn], a[maxn], b[maxn], Left[200], Right[200], h[200], h2[200], n;

void siftdown(int i,int n)                      //���ڴ���[0, n-1]����С�ѣ�����iλ����
{
    int j, t = h[i], t2 = h2[i];
    while((j = 2 * i + 1) < n)
    {
        if(j < n - 1 && h[j] > h[j+1]) j++;
        if(t > h[j])
        {
            h[i] = h[j];  h2[i] = h2[j];
            i = j;
        }
        else break;
    }
    h[i]= t;  h2[i] = t2;
}

void k_sort(int l, int r, int k, int *a, int *b)     //��һ���鲢�Ľ����b���飬��һ���鲢��¼��a����
{
    if(r - l <= k)                                  //�����ֽ���ʱ(С�ڵ���K),����ѡ������
    {
        for(int i = l; i < r; i++) a[i] = num[i];
        for(int i = l; i < r - 1; i++)
        {
            int mx = i;
            for(int j = i + 1; j < r; j++)
                if(a[j] < a[mx]) mx = j;
            swap(a[i], a[mx]);
        }
        return;
    }

    int gap = (r - l) / k;
    for(int i = 0; i < k - 1; i++)       //��[L, r)��Χ�����ֳ�k�飬�ȷֱ�鲢��Ȼ��ϲ�
    {
        k_sort(l + i * gap, l + (i + 1) * gap, k, b, a);
    }
    k_sort(l + (k - 1) * gap, r, k, b, a);

    for(int i = 0; i < k; i++)                   //ȷ��k���������ʼ���β��ͬʱ��ÿ������ĵ�һ����ջ
    {
        Left[i] = l + i * gap;
        if(i < k - 1) Right[i] = Left[i] + gap;
        else Right[i] = r;
        h[i] = b[Left[i]];
        h2[i] = i;
    }

    for(int i = (k - 2) / 2; i >= 0; i--)             //���Ѿ���k�����Ķѵ�������С��
        siftdown(i, k);

    int cur = k;
    for(int i = l; i < r - 1; i++)
    {
        a[i] = h[0];                                 //ÿ��ȡ�Ѷ���Ȼ�󽫶�Ӧ������һ�������
        if(++Left[h2[0]] < Right[h2[0]])
            h[0] = b[Left[h2[0]]];
        else                                         //���������ȡ����
        {
            h[0] = h[cur-1];
            h2[0] = h2[cur-1];
            cur--;
        }
        siftdown(0, cur);
    }
    a[r-1] = h[0];
}


int main()
{
    FILE *file = fopen("data1m.txt","r");                //�ȴ��ļ��ж�ȡ���ݵ�num����
    fscanf(file, "%d", &n);
    for(int i = 0; i < n; i++)
        fscanf(file, "%d", &num[i]);
    fclose(file);

    for(int k = 2; k <= 200; k++)                          //��K����ö��
    {
        for(int i = 0; i < n; i++)
            a[i] = num[i];
        clock_t start = clock();
        k_sort(0, n, k, a, b);
        clock_t finish = clock();
        printf("k = %d, ִ��ʱ��Ϊ��%d\n", k, finish - start);
        //for(int i = 0; i < n; i++) printf("%d ", a[i]);
        putchar('\n');
    }
}
