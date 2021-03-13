#include <windows.h>
#include <stdlib.h>
#include <cstdlib>
#include <stdio.h>
#include <tlhelp32.h>
//***************************************************************************************

BOOL InitApplication(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance,     // ��ں���
                   HINSTANCE,
                   LPSTR     lpCmdLine,
                   int       nCmdShow  )
{
    if (!InitApplication(hInstance))       // Ӧ�ó�ʼ��
        return FALSE;

    if (!InitInstance(hInstance,nCmdShow)) // ʵ����ʼ��
        return FALSE;

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))   // ��Ϣѭ��
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

//***************************************************************************************

BOOL InitApplication(HINSTANCE hInstance)   // Ӧ�ó�ʼ��
{
    WNDCLASS  wc;  // Data structure of the window class

    wc.style            = CS_HREDRAW|CS_VREDRAW;
    wc.lpfnWndProc      = (WNDPROC)MainWndProc;  // Name of the Window Function
    wc.cbClsExtra       = 0;
    wc.cbWndExtra       = 0;
    wc.hInstance        = hInstance;
    wc.hIcon            = LoadIcon (NULL, IDI_APPLICATION);
    wc.hCursor          = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground    = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName     = NULL;
    wc.lpszClassName    = TEXT("2020MPADLab1: Windows�����б� ����ѧ�ţ�10185102153 ���������ӷ�");  // Name of the window class

    return RegisterClass(&wc);
}

//***************************************************************************************

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)  // ʵ����ʼ��
{
    HWND hWnd = CreateWindow(TEXT("2020MPADLab1: Windows�����б� ����ѧ�ţ�10185102153 ���������ӷ�"),     // Name of the window class
                             TEXT("2020MPADLab1: Windows�����б� ����ѧ�ţ�10185102153 ���������ӷ�"), // Title of the window
                             WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT,
                             CW_USEDEFAULT,
                             1300,
                             600,
                             NULL,
                             NULL,
                             hInstance,
                             NULL                                        );
    if (!hWnd) return FALSE;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//***************************************************************************************

// ���ڹ��̺���

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    LPCTSTR ID = TEXT("ID");   // ��ʾ������
    LPCTSTR NAME = TEXT("Name");

    PAINTSTRUCT ps;
    HDC hdc;

    switch (message) {

        case WM_PAINT:  // ���ڿͻ�����ˢ��
        {

            hdc = BeginPaint (hWnd, &ps);
            for(int i = 10; i < 400 * 3; i += 400)                  //�����һ��
            {
                TextOut(hdc, i, 10, ID,lstrlen(ID));
                TextOut(hdc, i + 100, 10, NAME,lstrlen(NAME));
                printf("%d %d\n", i, i + 100);
            }

            PROCESSENTRY32 pe32;
            HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
            pe32.dwSize = sizeof(pe32);
            BOOL bMore = Process32First(hProcessSnap, &pe32);

            int cnt = 0;
            TCHAR String_ID[20];
            LPCTSTR String_File;
            while (bMore)
            {
                //printf("%06x\n", (unsigned)pe32.th32ProcessID);
                wsprintf(String_ID, TEXT("%06x"),  (unsigned)pe32.th32ProcessID);

                String_File = pe32.szExeFile;
                TextOut(hdc, cnt%3*400 + 10, (cnt/3+2)*20, String_ID,lstrlen(String_ID));
                TextOut(hdc, cnt%3*400 + 110, (cnt/3+2)*20, String_File,lstrlen(String_File));

                bMore = Process32Next(hProcessSnap, &pe32);
                cnt++;
            }
            CloseHandle(hProcessSnap);


            EndPaint(hWnd, &ps);

            return 0;
        }
        case WM_DESTROY: // ���ڹر�

            PostQuitMessage(0);

            return 0;

       default:  // ȱʡ��Ϣ�Ĵ���

           return DefWindowProc(hWnd, message, wParam, lParam);
   }

}

