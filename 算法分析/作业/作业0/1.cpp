#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int maxn = 1e7 + 20;

//num存从文件里读出的数据，每次取不同k时，需要将num的数据拷贝到a数组
//由于归并的过程中，需要用到两个数组，所以a数组和b数组循环使用
//k个数组归并时，Left，Right双指针来表示某个数组还未入堆的范围
//h是最小堆，记录的是最小的数，h2记录的是h对应的数来自哪个数组
int num[maxn], a[maxn], b[maxn], Left[200], Right[200], h[200], h2[200], n;

void siftdown(int i,int n)                      //对于存在[0, n-1]的最小堆，将第i位调整
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

void k_sort(int l, int r, int k, int *a, int *b)     //上一步归并的结果在b数组，这一步归并记录到a数组
{
    if(r - l <= k)                                  //当数字较少时(小于等于K),进行选择排序
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
    for(int i = 0; i < k - 1; i++)       //将[L, r)范围的数分成k组，先分别归并，然后合并
    {
        k_sort(l + i * gap, l + (i + 1) * gap, k, b, a);
    }
    k_sort(l + (k - 1) * gap, r, k, b, a);

    for(int i = 0; i < k; i++)                   //确定k个数组的起始与结尾，同时将每个数组的第一个入栈
    {
        Left[i] = l + i * gap;
        if(i < k - 1) Right[i] = Left[i] + gap;
        else Right[i] = r;
        h[i] = b[Left[i]];
        h2[i] = i;
    }

    for(int i = (k - 2) / 2; i >= 0; i--)             //将已经有k个数的堆调整成最小堆
        siftdown(i, k);

    int cur = k;
    for(int i = l; i < r - 1; i++)
    {
        a[i] = h[0];                                 //每次取堆顶，然后将对应数组下一个数入堆
        if(++Left[h2[0]] < Right[h2[0]])
            h[0] = b[Left[h2[0]]];
        else                                         //若这个数组取完了
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
    FILE *file = fopen("data1m.txt","r");                //先从文件中读取数据到num数组
    fscanf(file, "%d", &n);
    for(int i = 0; i < n; i++)
        fscanf(file, "%d", &num[i]);
    fclose(file);

    for(int k = 2; k <= 200; k++)                          //对K进行枚举
    {
        for(int i = 0; i < n; i++)
            a[i] = num[i];
        clock_t start = clock();
        k_sort(0, n, k, a, b);
        clock_t finish = clock();
        printf("k = %d, 执行时间为：%d\n", k, finish - start);
        //for(int i = 0; i < n; i++) printf("%d ", a[i]);
        putchar('\n');
    }
}
