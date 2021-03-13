#include <bits/stdc++.h>
using namespace std;

int student[30003], club[1002], cnt[1002];

int Find(int x)
{
    if(x != club[x])
        club[x] = Find(club[x]);
    return club[x];
}

void join(int x, int y)
{
    x = Find(x);
    y = Find(y);
    if(x != y)
    {
        club[x] = y;
        cnt[y] += cnt[x];
    }
}
int main()
{
    int n, m, num, tmp;
    scanf("%d %d", &n, &m);
    for(int i = 1; i <= m; i++)
    {
        scanf("%d", &num);
        club[i] = i;
        for(int j = 1; j <= num; j++)
        {
            scanf("%d", &tmp);
            if(student[tmp]) join(student[tmp], i);
            else
            {
                cnt[Find(i)]++;
                student[tmp] = i;
            }
        }
    }

    int ans = 0;
    for(int i = 1; i <= m; i++)
        if(club[i] == i) ans = max(ans, cnt[i]);
    printf("%d\n", ans);
}