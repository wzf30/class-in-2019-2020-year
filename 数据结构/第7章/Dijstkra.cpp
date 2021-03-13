#include <bits/stdc++.h>

using namespace std;

const int INF = 0x3f3f3f3f;
const int maxn = 102;
typedef pair<int, int> P;

struct edge
{
    int to, cost;
    edge(int k1, int k2): to{k1}, cost{k2} {}
};

int d[maxn], pre[maxn];
//bool used[maxn];
vector<edge> G[maxn];
int V, E;
vector<int> path;
priority_queue< P, vector<P>, greater<P> > que;

void Dijkstra(int s)
{
    memset(d, INF, sizeof(d));
    d[s] = 0;
    que.push(P(0, s));

    while(!que.empty())
    {
        P p = que.top(); que.pop();
        int v = p.second;
        if(d[v] < p.first) continue;
        for(unsigned int i = 0; i < G[v].size(); i++)
        {
            edge e = G[v][i];
            if(d[e.to] > d[v] + e.cost)
            {
                pre[e.to] = v;
                d[e.to] = d[v] + e.cost;
                que.push(P(d[e.to], e.to));
            }
        }
    }
}
int main()
{
    int x, y, z, s;
    scanf("%d %d", &V, &E);
    for(int i = 1; i <= E; i++)
    {
        scanf("%d %d %d", &x, &y, &z);
        G[x].push_back(edge(y, z));
        G[y].push_back(edge(x, z));
    }

    printf("请指顶点:\n");
    scanf("%d", &s);
    Dijkstra(s);

    for(int i = 1; i <= V; i++)
    {
        if(i == s) continue;
        if(d[V] == INF)
        printf("顶点%d: 无法到达!", i);
        else
        {
            path.clear();
            printf("顶点%d: 路径长度为%d ", i, d[i]);
            int t = i;
            for(; t; t = pre[t]) path.push_back(t);
            for(unsigned i = path.size() - 1; i > 0; i--)
                printf("%d -> ", path[i]);
            printf("%d\n", path[0]);
        }
    }
}
