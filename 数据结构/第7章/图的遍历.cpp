#include<bits/stdc++.h>

using namespace std;

const int maxn = 100;                           //�趨һ�����ֵ
const int INF = 0x3f3f3f3f;

int cost[maxn][maxn], mark[maxn];                               //�ڽӾ���
vector<int> adj_list[maxn];                     //�ڽӱ�
int V, E;

void dfs1(int x)                         //�ڽӱ���������
{
    printf("%d ", x);
    mark[x] = 1;
    for(int i : adj_list[x])
        if(!mark[i])
            dfs1(i);

}

void bfs1()                             //�ڽӱ�Ĺ������
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
void dfs2(int x)                            //�ڽӾ�����������
{
    printf("%d ", x);
    mark[x] = 1;
    for(int i = 1; i <= V; i++)
        if(!mark[i] && cost[x][i] != INF)
            dfs2(i);
}

void bfs2()                                 //�ڽӾ���Ĺ������
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
    printf("���ڵ�һ�����붥�������ߵĸ���\n");
    scanf("%d %d", &V, &E);
    printf("����֮��%d������ÿ�������ӵĽ�����\n", E);
    for(int i = 1;  i <= E; i++)
    {
        scanf("%d %d %d", &x, &y, &z);
        cost[x][y] = cost[y][x] = z;
        adj_list[x].push_back(y);
        adj_list[y].push_back(x);
    }

    printf("�ڽӱ���������:\n");   dfs1(1);
    memset(mark, 0, sizeof(mark));
    printf("\n�ڽӱ�Ĺ������:\n");  bfs1();
    memset(mark, 0, sizeof(mark));
    printf("\n�ڽӾ�����������:\n");   dfs2(1);
    memset(mark, 0, sizeof(mark));
    printf("\n�ڽӾ���Ĺ������:\n");  bfs2();
}
