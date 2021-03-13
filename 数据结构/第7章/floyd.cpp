#include <bits/stdc++.h>

using namespace std;

const int INF = 0x3f3f3f3f;
const int maxn = 102;

int V, E, d[maxn][maxn], pre[maxn][maxn], ans[maxn];

void floyd()
{
    for(int k = 1; k <= V; k++)
        for(int i = 1; i <= V; i++)
            for(int j = 1; j <= V; j++)
            {
                if(d[i][k] + d[k][j] < d[i][j])
                {
                    d[i][j] = d[i][k] + d[k][j];
                    pre[i][j] = k;
                }
            }
}

int main()
{
    int x, y, z;
    memset(d, INF, sizeof(d));
    scanf("%d %d", &V, &E);
    for(int i = 1; i <= E; i++)
    {
        scanf("%d %d %d", &x, &y, &z);
        d[x][y] = z;
        pre[x][y] = x;
    }

    floyd();

    for(int i = 1; i <= V; i++)
    {
        for(int j = 1; j <= V; j++)
        {
            if(i == j) continue;
            if(d[i][j] == INF)
            {
                printf("顶点%d到顶点%d: 不可达\n", i, j);
                continue;
            }
            printf("顶点%d到顶点%d: 距离为%d  ", i, j, d[i][j]);
            int cnt = 0, k = j;
            ans[cnt++] = k;
            while(k != i)
            {
                k = pre[i][k];
                ans[cnt++] = k;
            }
            for(int m = cnt - 1; m > 0; m--) printf("%d->", ans[m]);
            printf("%d\n", ans[0]);
        }
        putchar('\n');
    }
}

/*
5 7
1 2 4
2 4 4
4 3 4
3 5 4
5 1 4
1 3 5
2 3 3
*/
