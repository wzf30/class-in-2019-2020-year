#include <windows.h>
#ifdef __cplusplus
#define EXPORT extern "C" __declspec (dllexport)
#else
#define EXPORT __declspec (dllexport)
#endif

EXPORT WINAPI int sum(int* a, int n);
EXPORT WINAPI const char* author(int i);
EXPORT WINAPI int f(int x, int *a);
EXPORT WINAPI char* lower(char* s);


BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, PVOID pvReserved)
{
     return TRUE ;
}

EXPORT const char* WINAPI author(int i)
{
    const char *s1 = "10185102153", *s2 = "���ӷ�";
    if(i == 1) return s1;
    return s2;
}

EXPORT int WINAPI f(int x, int *a)
{
    return a[0]*x*x + a[1]*x + a[2];
}

EXPORT char* WINAPI lower(char* s)
{
    for(int i = 0; s[i]; i++)
        if(s[i] >= 'A' && s[i] <= 'Z')
            s[i] = s[i] - 'A' + 'a';
    return s;
}


