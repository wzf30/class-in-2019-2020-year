#include <windows.h>
#include <stdio.h>
#include <tlhelp32.h> // 快照函数的头文件


int  main( )
{
   printf("Windows进程列表\n\n"); //Title

   PROCESSENTRY32 pe32;

   // 给所有进程拍快照
   HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

   // 在使用这个结构之前，先设置它的大小
   pe32.dwSize = sizeof(pe32);

  // 遍历进程快照，逐个显示进程信息
  BOOL bMore = Process32First(hProcessSnap, &pe32);
  while (bMore)
  {  printf(" 进程ID:  %05x   模块名:  %s\n", (unsigned)pe32.th32ProcessID,pe32.szExeFile);
     // %S for UNICODE string
     bMore = Process32Next(hProcessSnap, &pe32);
  }
  CloseHandle(hProcessSnap); // 关闭snapshot对象

  return 0;
}
