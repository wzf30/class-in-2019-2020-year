#include <bits/stdc++.h>
using namespace std;

int pre[100003], cnt[100003];
map<string, int> mp;

int Find(int x)
{
    if(x != pre[x])
        pre[x] = Find(pre[x]);
    return pre[x];
}

void join(int x, int y)
{
    x = Find(x);
    y = Find(y);
    if(x != y)
    {
        pre[y] = x;
        cnt[x] += cnt[y];
    }
    cout<<cnt[x]<<endl;
}
int main()
{
    int T, m, k1, k2;
    cin>>T;
    string tmp;
    while(T--)
    {
        for(int i = 1; i <= 100002; i++) pre[i] = i;
        for(int i = 1; i <= 100002; i++) cnt[i] = 1;
        int cur = 1;
        cin>>m;
        for(int i = 0; i < m; i++)
        {
            cin>>tmp;
            if(mp.find(tmp) != mp.end()) k1 = mp[tmp];
            else
            {
                k1 = cur++;
                mp[tmp] = k1;
            }
            cin>>tmp;
            if(mp.find(tmp) != mp.end()) k2 = mp[tmp];
            else
            {
                k2 = cur++;
                mp[tmp] = k2;
            }
            join(k1, k2);
        }
    }
}