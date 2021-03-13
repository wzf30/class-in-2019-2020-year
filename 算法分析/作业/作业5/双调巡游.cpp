#include <bits/stdc++.h>

using namespace std;
typedef long long ll;
const int maxn = 1e4 + 10;
const ll INF = 1e17;

struct node
{
    double x, y;
    bool operator<(const node& m1)
    {
        return x < m1.x;
    }
}coord[maxn];

int n;
double dp[2][maxn], mx;

double dis(int i, int j)
{
    return sqrt((coord[i].x - coord[j].x)*(coord[i].x - coord[j].x) +(coord[i].y - coord[j].y)*(coord[i].y - coord[j].y));
}

int main()
{
    scanf("%d", &n);
    for(int i = 1; i <= n; i++)
        scanf("%lf %lf", &coord[i].x, &coord[i].y);
    sort(coord + 1, coord + 1 + n);

    int cur = 0;
    dp[cur][1] = dis(1, 2);
    for(int i = 3; i < n; i++)
    {
        cur ^= 1;
        dp[cur][i-1] = DBL_MAX;
        double d = dis(i, i - 1);
        for(int j = 1; j <= i - 2; j++)
        {
            dp[cur][j] = dp[1-cur][j] + d;
            dp[cur][i-1] = min(dp[cur][i-1], dp[1-cur][j] + dis(j, i));
        }
    }

    if(n == 2) printf("%.4f\n", dp[cur][1] * 2);
    else
    {
        mx = DBL_MAX;
        for(int i = 1; i <= n - 2; i++)
            mx = min(mx, dp[cur][i] + dis(i, n));
        mx += dis(n, n - 1);
        printf("%.4f\n", mx);
    }

}
