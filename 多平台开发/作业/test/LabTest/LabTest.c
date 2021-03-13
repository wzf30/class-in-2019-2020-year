#include <windows.h>
#ifdef __cplusplus
#define EXPORT extern "C" __declspec (dllexport)
#else
#define EXPORT __declspec (dllexport)
#endif


EXPORT WINAPI const char* author(int i);
EXPORT WINAPI void iSort(int *a, int n, int ascending);


BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, PVOID pvReserved)
{
     return TRUE ;
}

EXPORT const char* WINAPI author(int i)
{
    const char *s1 = "10185102153", *s2 = "Íô×Ó·²";
    if(i == 1) return s1;
    return s2;
}



EXPORT void iSort(int *a, int n, int ascending)
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


