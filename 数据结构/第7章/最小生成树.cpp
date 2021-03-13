#include<bits/stdc++.h>
const int maxn = 100;
const int INF = 0x3f3f3f3f;
using namespace std;

struct edge                           //定义边
{
    int u, v, cost;
    bool operator<(const edge& m1)
    {
        return cost < m1.cost;
    }
}e[maxn * maxn];

int V, E, pre[maxn], d[maxn], mark[maxn];
int cost[maxn][maxn], lowcost[maxn], closest[maxn];

int Find(int x)                    //并查集查找操作
{
    if(pre[x] != x)
        pre[x] = Find(pre[x]);
    return pre[x];
}

int kruskal()
{
    sort(e, e + E);
    for(int i = 1; i <= V; i++) pre[i] = i;
    int ans = 0, x, y, cnt = 0;
    for(int i = 0; i < E; i++)
    {
        x = Find(e[i].u);
        y = Find(e[i].v);
        if(x != y)
        {
            printf("%d %d %d\n", e[i].u, e[i].v, e[i].cost);
            cnt++;
            pre[x] = y;
            ans += e[i].cost;
        }
    }
    return ans;
}


int prim(int u)
{
    int res = 0, mx;
    int i , j , k1, k2;
    for(i = 1; i <= V; i ++)
    {
        lowcost[i] = cost[u][i];
        closest[i] = u;
    }
    lowcost[u] = 0;
    for(i = 1; i < V; i++)
    {
        mx = INF;
        int flag = 0;
        for(j = 1; j <= V; j ++)
            if(lowcost[j] != 0 && lowcost[j] < mx)
            {
                mx = lowcost[j];
                k1 = j;  k2 = closest[j];
                flag = 1;
            }
        if(flag)
        {
            res += mx;
            printf("%d %d %d\n", k1, k2, mx);
            lowcost[k1] = 0;
            for(j = 1; j <= V; j ++)
            if(cost[k1][j] != 0 && cost[k1][j] < lowcost[j])
            {
                lowcost[j] = cost[k1][j];
                closest[j] = k1;
            }
        }
        else
        {
            res = -1;
            break;
        }
    }
    return res;
}

int main()
{
    scanf("%d %d", &V, &E);
    memset(cost, INF, sizeof(cost));
    memset(lowcost, INF, sizeof(lowcost));
    for(int i = 0; i < E; i++)
    {
        int x, y, z;
        scanf("%d %d %d", &x, &y, &z);
        cost[x][y] = cost[y][x] = z;
        e[i].u = x;  e[i].v = y;  e[i].cost = z;
    }

    putchar('\n');
    int ans1 = kruskal();
    printf("用kruskal方法算出来最小值为%d\n", ans1);
    int ans2 = prim(1);
    printf("用prim方法算出来最小值为%d\n", ans2);
}
