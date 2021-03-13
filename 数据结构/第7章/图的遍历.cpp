#include<bits/stdc++.h>

using namespace std;

const int maxn = 100;                           //设定一个最大值
const int INF = 0x3f3f3f3f;

int cost[maxn][maxn], mark[maxn];                               //邻接矩阵
vector<int> adj_list[maxn];                     //邻接表
int V, E;

void dfs1(int x)                         //邻接表的深度优先
{
    printf("%d ", x);
    mark[x] = 1;
    for(int i : adj_list[x])
        if(!mark[i])
            dfs1(i);

}

void bfs1()                             //邻接表的广度优先
{
    queue<int> q;
    while(!q.empty()) q.pop();
    q.push(1);
    mark[1] = 1;

    while(!q.empty())
    {
        int x = q.front();  q.pop();
        printf("%d ", x);
        for(int i : adj_list[x])
            if(!mark[i])
            {
                q.push(i);
                mark[i] = 1;
            }
    }
}
void dfs2(int x)                            //邻接矩阵的深度优先
{
    printf("%d ", x);
    mark[x] = 1;
    for(int i = 1; i <= V; i++)
        if(!mark[i] && cost[x][i] != INF)
            dfs2(i);
}

void bfs2()                                 //邻接矩阵的广度优先
{
    queue<int> q;
    while(!q.empty()) q.pop();
    q.push(1);
    mark[1] = 1;

    while(!q.empty())
    {
        int x = q.front();  q.pop();
        printf("%d ", x);
        for(int i = 1; i <= V; i++)
            if(!mark[i] && cost[x][i] != INF)
            {
                q.push(i);
                mark[i] = 1;
            }
    }
}


int main()
{
    int x, y, z;
    memset(cost, INF, sizeof(cost));
    printf("请在第一行输入顶点个数与边的个数\n");
    scanf("%d %d", &V, &E);
    printf("请在之后%d行输入每条边连接的结点序号\n", E);
    for(int i = 1;  i <= E; i++)
    {
        scanf("%d %d %d", &x, &y, &z);
        cost[x][y] = cost[y][x] = z;
        adj_list[x].push_back(y);
        adj_list[y].push_back(x);
    }

    printf("邻接表的深度优先:\n");   dfs1(1);
    memset(mark, 0, sizeof(mark));
    printf("\n邻接表的广度优先:\n");  bfs1();
    memset(mark, 0, sizeof(mark));
    printf("\n邻接矩阵的深度优先:\n");   dfs2(1);
    memset(mark, 0, sizeof(mark));
    printf("\n邻接矩阵的广度优先:\n");  bfs2();
}
