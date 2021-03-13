#include <bits/stdc++.h>

using namespace std;
typedef long long ll;
const ll maxn = 1e18 + 10;
const ll INF = 1e17;

ll fib[100], dp[100][2], x;
int cnt, t, pos[100];

void init()
{
    fib[0] = fib[1] = 1;
    for(cnt = 2; fib[cnt-1] <= maxn; cnt++)
        fib[cnt] = fib[cnt-1] + fib[cnt-2];
    cnt--;
}

int main()
{
    init();
    scanf("%d", &t);
    for(int cas = 0; cas < t; cas++)
    {
        scanf("%lld", &x);
        int cur = 0;
        for(int i = cnt; x; i--)
        {
            if(x >= fib[i])
            {
                pos[cur++] = i;
                x -= fib[i];
            }
        }

        dp[cur-1][0] = 1;  dp[cur-1][1] = (pos[cur-1] - 1) / 2;
        //cout<<dp[cur-1][0]<<" "<<dp[cur-1][1]<<endl;
        for(int i = cur - 2; i >= 0; i--)
        {
            dp[i][0] = dp[i+1][0] + dp[i+1][1];
            dp[i][1] = dp[i+1][0] * ((pos[i] - pos[i+1] - 1) / 2) + dp[i+1][1] * ((pos[i] - pos[i+1]) / 2);
            //cout<<dp[i][0]<<" "<<dp[i][1]<<endl;
        }

        printf("case #%d:\n%lld\n", cas, dp[0][0] + dp[0][1]);
    }
}