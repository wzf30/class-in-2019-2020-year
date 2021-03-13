#include <windows.h>
#include <stdio.h>
#include <tlhelp32.h> // ���պ�����ͷ�ļ�


int  main( )
{
   printf("Windows�����б�\n\n"); //Title

   PROCESSENTRY32 pe32;

   // �����н����Ŀ���
   HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

   // ��ʹ������ṹ֮ǰ�����������Ĵ�С
   pe32.dwSize = sizeof(pe32);

  // �������̿��գ������ʾ������Ϣ
  BOOL bMore = Process32First(hProcessSnap, &pe32);
  while (bMore)
  {  printf(" ����ID:  %05x   ģ����:  %s\n", (unsigned)pe32.th32ProcessID,pe32.szExeFile);
     // %S for UNICODE string
     bMore = Process32Next(hProcessSnap, &pe32);
  }
  CloseHandle(hProcessSnap); // �ر�snapshot����

  return 0;
}
