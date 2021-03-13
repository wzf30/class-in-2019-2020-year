#include <bits/stdc++.h>

using namespace std;
typedef long long ll;
const int maxn = 1e4 + 10;
const ll INF = 1e17;

int d[2][maxn], word[maxn], n, M;
ll ans[2][maxn], mx;


int main()
{
    scanf("%d %d", &n, &M);
    for(int i = 1; i <= n; i++)
        scanf("%d", &word[i]);

    int cur = 0, cnt = 1;
    d[cur][0] = word[1];
    for(int i = 2; i <= n; i++)
    {
        int tmp = 0, j;
        cur ^= 1;
        d[cur][tmp] = word[i];
        ans[cur][tmp++] = INF;

        for(j = 0; j < cnt; j++)
        {
            if(d[1-cur][j] + word[i] + 1 <= M)
            {
                d[cur][tmp] = d[1-cur][j] + word[i] + 1;
                ans[cur][tmp++] = ans[1-cur][j];
            }
            ans[cur][0] = min(1LL * (M - d[1-cur][j])*(M - d[1-cur][j])*(M - d[1-cur][j]) + ans[1-cur][j], ans[cur][0]);
        }
        cnt = tmp;
    }

    mx = INF;
    for(int i = 0; i < cnt; i++)
        mx = min(mx, ans[cur][i]);
    printf("%lld\n", mx);
}
