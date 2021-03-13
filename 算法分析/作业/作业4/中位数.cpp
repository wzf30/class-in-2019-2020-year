#include <bits/stdc++.h>

using namespace std;
typedef long long ll;
const int maxn = 1e5 + 10;
const ll INF = 1e17;

int m, n, q, len, mark;
ll arr1[maxn], arr2[maxn], tmp, x, x1, x2;

int main()
{
    scanf("%d %d %d", &m, &n, &q);
    if(m > n)
    {
        int k = (m - n) / 2, i;
        for(i = 1; i <= k; i++) scanf("%lld", &tmp);
        for(; i + k <= m; i++) scanf("%lld", &arr1[i-k]);
        for(; i <= m; i++) scanf("%lld", &tmp);
        for(i = 1; i <= n; i++) scanf("%lld", &arr2[i]);
        if((m + n) & 1)
        {
            arr2[n+1] = -INF;
            len = n + 1;
        }
        else len = n;
    }
    else
    {
        int k = (n - m) / 2, i;
        for(i = 1; i <= m; i++) scanf("%lld", &arr1[i]);
        for(i = 1; i <= k; i++) scanf("%lld", &tmp);
        for(; i + k <= n; i++) scanf("%lld", &arr2[i-k]);
        for(; i <= n; i++) scanf("%lld", &tmp);
        if((m + n) & 1)
        {
            arr1[m+1] = -INF;
            len = m + 1;
        }
        else len = m;
    }
    sort(arr1 + 1, arr1 + 1 + len);
    sort(arr2 + 1, arr2 + 1 + len);

    for(int cas = 1; cas <= q; cas++)
    {
        scanf("%d %lld", &mark, &x);
        if(mark == 1) x1 = x, x2 = 0;
        else x2 = x, x1 = 0;

        int p1 = 1, p2 = 1, curLen = len;
        double ans1, ans2, ans;
        do
        {
            if(curLen & 1)
                ans1 = arr1[p1+curLen/2], ans2 = arr2[p2+curLen/2];
            else
                ans1 = (arr1[p1+curLen/2-1] + arr1[p1+curLen/2]) / 2.0, ans2 = (arr2[p2+curLen/2-1] + arr2[p2+curLen/2]) / 2.0;

           // cout<<p1<<" 开始： "<<ans1<<"  "<<p2<<" 开始： "<<ans2<<endl;
            if(ans1 + x1 == ans2 + x2) break;
            if(ans1 + x1 > ans2 + x2)
                p2 = p2 + (curLen - 1) / 2;
            else
                p1 = p1 + (curLen - 1) / 2;
            curLen -= (curLen - 1) / 2;
        }while(curLen > 2);

       // cout<<"curLen"<<curLen<<endl;
        if(curLen == 1) ans = (arr1[p1] + arr2[p2] + x1 + x2) / 2.0;
        else if(curLen == 2)
        {
            ll tmp[4] = {arr1[p1] + x1, arr1[p1+1] + x1, arr2[p2] + x2, arr2[p2+1] + x2};
            sort(tmp, tmp + 4);
            ans = (tmp[1] + tmp[2]) / 2.0;
        }
        else ans = ans1 + x1;
        if((m + n) & 1)
        {
            if(ans != (ll)ans) ans += 0.5;
            //cout<<ans<<endl;
            int pos1 = lower_bound(arr1 + 1, arr1 + 1 + len, (ll)ans - x1) - arr1;
            int pos2 = lower_bound(arr2 + 1, arr2 + 1 + len, (ll)ans - x2) - arr2;
            if(pos1 == len + 1) ans = arr2[pos2] + x2;
            else if(pos2 == len + 1) ans = arr1[pos1] + x1;
            else ans = min(arr1[pos1] + x1, arr2[pos2] + x2);
        }
        if(ans == (ll)ans) printf("%lld\n", (ll)ans);
        else printf("%.1f\n", ans);
    }
}