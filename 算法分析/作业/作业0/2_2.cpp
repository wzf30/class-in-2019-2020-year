#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int maxn = 1e4 + 20;

int a[maxn], n;
ll ans;
unordered_map<int, int> mp;

int main()
{
   FILE *file = fopen("10000.txt", "r");
   fscanf(file, "%d", &n);
   for(int i = 1; i <= n; i++)
        fscanf(file, "%d", &a[i]);
   sort(a + 1, a + 1 + n);

   mp[a[n]+a[n-1]] = 1;
   for(int i = n - 2; i >= 2; i--)
   {
       for(int j = i - 1; j >= 1; j--)
       {
           int w = a[i] + a[j];
           ans += mp[-w];
       }
       for(int j = i + 1; j <= n; j++)
           mp[a[i]+a[j]]++;
   }
   printf("%lld\n", ans);
}
