/*A*算法，用优先队列来模拟open表(按照f值从小到大排序)，每次取出f最小的
用一个数组去模拟Close表(0表示在，1表示不在)
*/
#include <bits/stdc++.h>

using namespace std;
typedef pair<double, int> P;                      //first为节点的f值，second为标号
const int maxn = 1e3 + 3;
const double INF = 1e9;

struct node
{
    int to;
    double c;
    node(int k1, double k2): to{k1}, c{k2} {}
};
vector<node> G[maxn];
priority_queue< P, vector<P>, greater<P> > Open;
double coord[maxn][2], h[maxn], g[maxn];
int Close[maxn], pre[maxn], V, s, t, ans[maxn];

double Astar()
{
    for(int i = 0; i < V; i++)
        g[i] = INF;
    g[s] = 0;                              //开始时 g[s] = 0
    Open.push(P(g[s] + h[s], s));                  //将结点s放入open表
    while(!Open.empty())
    {
        P p = Open.top();  Open.pop();                    //取出open表的第一个结点
        int v = p.second;                              //fist是第一个结点的f值，second是这个结点的标号
        if(Close[v]) continue;                         //若是这个结点已经被放入close表，则不需要再次遍历
        Close[v] = 1;
        if(v == t) return p.first;                     //如果为所求, Exit(Success)

        for(unsigned i = 0; i < G[v].size(); i++)      //Expand
        {
            node e = G[v][i];
            if(Close[e.to] || g[e.to] < g[v] + e.c) continue;     //如果这个结点在Close表中，或者在open表中但比现在情况更优，则不需要再次扩展
            pre[e.to] = v;                             //记录前驱结点
            Open.push(P(g[v] + e.c + h[e.to], e.to));           //f = g(pre) + c + h
            g[e.to] = g[v] + e.c;
        }
    }
    return -1;                  //当open表为空，Exit(fail)
}

int main(int argv, char** args)
{
    int x, y, tmp;
    s = 123, t = 321;
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
        double c = sqrt((coord[x][0] - coord[y][0])*(coord[x][0] - coord[y][0]) + (coord[x][1] - coord[y][1])*(coord[x][1] - coord[y][1]));
        G[x].push_back(node(y, c));
        G[y].push_back(node(x, c));
    }
    fclose(p1);

    for(int i = 0; i < V; i++)
        h[i] = sqrt((coord[i][0] - coord[t][0])*(coord[i][0] - coord[t][0]) + (coord[i][1] - coord[t][1])*(coord[i][1] - coord[t][1]));

    double res = Astar();
    if(res == -1) printf("No Path\n");
    else
    {
        printf("length: %.4f\n\n", res);

        int cnt = 0;
        for(int i = t; i != s; i = pre[i])
            ans[cnt++] = i;
        printf("path: 123");
        for(int i = cnt - 1; i >= 0; i--)
            printf(" -> %d", ans[i]);
        putchar('\n');
    }
}
