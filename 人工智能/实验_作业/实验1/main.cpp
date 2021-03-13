/*Dijkstra求最短路径，每次选取离起始点最近的点进行扩展*/

#include <bits/stdc++.h>

using namespace std;

const double INF = 1e9;
const int maxn = 1e3 + 3;
typedef pair<double, int> P;

struct edge
{
    int to;
    double cost;
    edge(int k1, double k2): to{k1}, cost{k2} {}
};

int pre[maxn], ans[maxn], V, E;       //pre记录一条路径，state记录条数
double d[maxn], coord[maxn][2];
vector<edge> G[maxn];
priority_queue< P, vector<P>, greater<P> > que;

void Dijkstra(int s)
{
    for(int i = 0; i < V; i++)
        d[i] = INF;
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
            if(d[e.to] > d[v] + e.cost)     //小于情况
            {
                pre[e.to] = v;                  //更新前驱结点
                d[e.to] = d[v] + e.cost;
                que.push(P(d[e.to], e.to));
            }
        }
    }
}

/*int main(int argv, char** args)
{
    int x, y, tmp;
    V = 1000;
    FILE *p1 = fopen(args[1], "r");
    for(int i = 0; i < V; i++)
    {
        fscanf(p1, "%d %lf %lf", &tmp, &coord[i][0], &coord[i][1]);
    }
    fclose(p1);

    p1 = fopen(args[2], "r");
    while(~fscanf(p1, "%d %d", &x, &y))
    {
        E++;
        double c = sqrt((coord[x][0] - coord[y][0])*(coord[x][0] - coord[y][0]) + (coord[x][1] - coord[y][1])*(coord[x][1] - coord[y][1]));
        G[x].push_back(edge(y, c));
        G[y].push_back(edge(x, c));
    }
    fclose(p1);

    Dijkstra(123);
    printf("length: %.4f\n\n", d[321]);

    int cnt = 0;
    for(int i = 321; i != 123; i = pre[i])
        ans[cnt++] = i;
    printf("path: 123");
    for(int i = cnt - 1; i >= 0; i--)
        printf(" -> %d", ans[i]);
    putchar('\n');
}*/
