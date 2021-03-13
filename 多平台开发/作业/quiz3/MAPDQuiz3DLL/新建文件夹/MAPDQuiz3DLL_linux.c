const char* author(int i)
{
    const char *s1 = "10185102153", *s2 = "汪子凡";
    if(i == 1) return s1;
    return s2;
}

int f(int x, int *a)
{
    return a[0]*x*x + a[1]*x + a[2];
}

 char*  lower(char* s)
{
    for(int i = 0; s[i]; i++)
        if(s[i] >= 'A' && s[i] <= 'Z')
            s[i] = s[i] - 'A' + 'a';
    return s;
}


