#include<bits/stdc++.h>
#define INF 0x3f3f3f3f
const int maxn = 20;
using namespace std;

int x[maxn], y[maxn]; //表示一行与一列
int a[2*maxn], b[2*maxn]; //表示主对角线与次对角线
int cnt, tmp[14], ans[14], n;

void dfs(int k1, int k2)
{
    if(x[k1] || y[k2] || a[k1+k2] || b[k1-k2+n])          //剪枝
        return;
    tmp[k1] = k2;                          //记录
    if(k1 == n)                                  //若找到最后答案
    {
        cnt++;
        if(cnt == 1)
        {
            for(int i = 1; i <= n; i++)
                ans[i] = tmp[i];
        }
        return;
    }
    x[k1] = y[k2] = a[k1+k2] = b[k1-k2+n] = 1;
    for(int i = 1; i <= n; i++)
        dfs(k1+1, i);
    x[k1] = y[k2] = a[k1+k2] = b[k1-k2+n] = 0;
}

int main()
{
    scanf("%d", &n);
    for(int i = 1; i <= n; i++)
        dfs(1, i);
    printf("%d\n", cnt);
    if(!cnt) return 0;
    for(int j = 1; j < n; j++)
        printf("%d ", ans[j]-1);
    printf("%d\n", ans[n]-1);
}
