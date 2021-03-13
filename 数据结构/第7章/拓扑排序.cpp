#include <bits/stdc++.h>

using namespace std;

const int maxn = 100;

int in[maxn], V, E, ans[maxn];              //记录入度
vector<int> v[maxn];       //记录后驱节点
queue<int> q;

int main()
{
    int V, E, x, y;
    scanf("%d %d", &V, &E);
    for(int i = 1; i <= E; i++)
    {
        scanf("%d %d", &x, &y);
        in[y]++;
        v[x].push_back(y);
    }

    for(int i = 1; i <= V; i++)
        if(!in[i])
            q.push(i);

    int cnt = 0;
    while(!q.empty())
    {
        int tmp = q.front();  q.pop();
        ans[cnt++] = tmp;
        for(int i : v[tmp])
        {
            in[i]--;
            if(!in[i]) q.push(i);
        }
    }

    if(cnt != V) printf("这个有向图不存在拓扑序列");
    else
    {
        printf("存在拓扑排序：");
        for(int i = 0; i < cnt; i++) printf("%d ", ans[i]);
    }
}
