const char* WINAPI author(int i)
{
    const char *s1 = "10185102153", *s2 = "汪子凡";
    if(i == 1) return s1;
    return s2;
}



void iSort(int *a, int n, int ascending)
{
    int i, j, cur, tmp;
    if(ascending == 1)
    {
        for(i = 0; i < n - 1; i++)
        {
            cur = i;
            for(j = i + 1; j < n; j++)
                if(a[cur] > a[j])
                    cur = j;
            tmp = a[cur];
            a[cur] = a[i];
            a[i] = tmp;
        }
    }
    else
    {
        for(i = 0; i < n - 1; i++)
        {
            cur = i;
            for(j = i + 1; j < n; j++)
                if(a[cur] < a[j])
                    cur = j;
            tmp = a[cur];
            a[cur] = a[i];
            a[i] = tmp;
        }
    }
}
