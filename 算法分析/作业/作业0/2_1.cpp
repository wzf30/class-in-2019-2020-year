#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int maxn = 1e7 + 20;

int a[maxn], n;
ll ans;
map<int, int> mp1, mp2;

int main()
{
   FILE *file = fopen("2000.txt", "r");
   fscanf(file, "%d", &n);
   for(int i = 1; i <= n; i++)
        fscanf(file, "%d", &a[i]);
    fclose(file);
   for(int i = 1; i <= n; i++)
       mp2[a[i]]++;

   for(int i = 1; i <= n - 1; i++)
       for(int j = i + 1; j <= n; j++)
       {
           int tmp = a[i] + a[j];
           ans += mp1[-tmp];
           mp1[tmp]++;
           if(!(tmp & 1))
           {
               tmp >>= 1;
               ans -= mp2[-tmp] - (a[i] == -tmp) - (a[j] == -tmp);
           }
       }
    printf("%lld\n", ans / 3);
}
