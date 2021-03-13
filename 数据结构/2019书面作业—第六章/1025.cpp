#include <bits/stdc++.h>
using namespace std;

int n,x1, ya, x2, y2, mark[103][103] = {0};  //0表示走过，1表示没有走过
char maze[103][103];

bool dfs(int x, int y)
{
    if(x == x2 && y == y2) return true;
    if(maze[x][y] == '1' || x < 0 || x >= n || y < 0 || y >= n || mark[x][y]) return false;
    mark[x][y] = 1;
    return dfs(x + 1, y) || dfs(x - 1, y) || dfs(x, y + 1) || dfs(x, y - 1) ||
     dfs(x + 1, y + 1) || dfs(x + 1, y - 1) || dfs(x - 1, y + 1) || dfs(x - 1, y - 1);

}
int main()
{
    scanf("%d %d %d %d %d", &n, &x1, &ya, &x2, &y2);
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            scanf(" %c", &maze[i][j]);      //1表示障碍物，0表示可以通行
    if(dfs(x1, ya)) printf("yes\n");
    else printf("no\n");
}


