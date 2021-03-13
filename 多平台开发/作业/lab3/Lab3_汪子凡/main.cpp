#include <windows.h>
#include <stdio.h>
#include "resource.h"

LPCTSTR showIcon[3] = {"��ǰʹ�õ�ͼ���ǣ�ͼ��1", "��ǰʹ�õ�ͼ���ǣ�ͼ��2", "��ǰʹ�õ�ͼ���ǣ�ͼ��3"};
LPCTSTR showMesg[5] = {"��ʾ��Ϣ1", "��ʾ��Ϣ2", "��ʾ��Ϣ3", "��ʾ��Ϣ4", "                 "};
int id_icon[] = {IDI_ICON1, IDI_ICON2, IDI_ICON3};
HWND hWnd;

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
    HACCEL haccel = LoadAccelerators(hInstance,MAKEINTRESOURCE(IDR_ACCELERATOR1));           //Load�����ټ�

    while (GetMessage(&msg, NULL, 0, 0))   // ��Ϣѭ��
    {
        if(!TranslateAccelerator(hWnd, haccel, &msg))
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
    //wc.hIcon            = LoadIcon (NULL, IDI_APPLICATION);
    wc.hIcon            = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wc.hCursor          = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground    = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName     = MAKEINTRESOURCE(IDR_MENU1);               //����꽫��Դ��ID��ʾת��Ϊ�ַ�����ʾ������ֱ�����ַ���
    wc.lpszClassName    = TEXT("My1stWClass");  // Name of the window class

    return RegisterClass(&wc);
}

//***************************************************************************************

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)  // ʵ����ʼ��
{
    hWnd = CreateWindow(TEXT("My1stWClass"),     // Name of the window class
                             TEXT("2020MPADLab3: Windows��Դʹ��(1) ���������ӷ� ѧ�ţ�10185102153"), // Title of the window
                             WS_OVERLAPPEDWINDOW,                   //���ڷ��
                             CW_USEDEFAULT,                     //��ʼλ��
                             CW_USEDEFAULT,
                             CW_USEDEFAULT,                      //���
                             CW_USEDEFAULT,
                             NULL,                                   //�����ھ��
                             NULL,                                   //�˵��������Ҫ���Ǿ��,�����������ط����ز˵����������У�������ط�Ϊ׼
                             hInstance,
                             NULL                                        );
    if (!hWnd) return FALSE;

    HMENU hMenuMain = GetMenu(hWnd);
    HMENU hMenuFile = CreatePopupMenu();                                               //��̬����File�����˵�
    InsertMenu(hMenuMain, 0, MF_BYPOSITION|MF_STRING|MF_POPUP, (UINT)hMenuFile, "&File");  //��File�˵��������˵�
    AppendMenu(hMenuFile, MF_STRING, ID_FILE_EXIT, (LPTSTR)"Exit\tCtrl+Shift+Delete");     //��File�˵��²����Ӳ˵�(������ID)
                                                                                      //ID_FILE_EXIT��rc�ļ��ж����˼��ټ�
    CheckMenuRadioItem(hMenuMain,ID_40002,ID_40004, ID_40002, MF_BYCOMMAND);             //��ʼѡ�е�һ��ͼ��״̬

    ShowWindow(hWnd, nCmdShow);                                    //����VM_SHOWWINDOW��Ϣ��VM_SIZE��Ϣ
    UpdateWindow(hWnd);                                            //����VM_PAINT��Ϣ

    HDC hdc = GetDC(hWnd);
    TextOut(hdc, 10, 10, showIcon[0], lstrlen(showIcon[0]));                                 //��ʾ����
    ReleaseDC(hWnd, hdc);

    return TRUE;
}

//***************************************************************************************

// ���ڹ��̺���

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;                         //�豸�����ľ��
    //PAINTSTRUCT ps;
    static HINSTANCE hInst;
    switch (message) {
       case WM_CREATE:                            //��ȡӦ�õľ��
         {
             hInst = ((LPCREATESTRUCT)lParam)-> hInstance;
         }

         /*case WM_PAINT:
         {
            hdc = BeginPaint(hWnd, &ps);
            TextOut(hdc, 500, 500, showMesg[0], lstrlen(showMesg[0]));
            EndPaint(hWnd, &ps);                    //���������������Ը���ϵͳ��Ҫ�ػ��ĵط��Ѿ��ػ�����getDC���У���һֱ�ظ�����
         }*/

         case WM_INITMENU:
         {
           // HMENU hMenuMain = (HMENU)wParam;
         }

         case WM_COMMAND:                                      //����COMMAND��Ϣ
         {
            HMENU hMenuMain = GetMenu(hWnd);
            switch(LOWORD(wParam))
            {
                printf("%d\n", LOWORD(wParam));
                case ID_40002:                                 //����ѡ��ͼ���µİ���
                case ID_40003:
                case ID_40004:
                {
                    printf("%d\n", LOWORD(wParam));
                    int d = LOWORD(wParam) - ID_40002;                        //����һ��Ҫ�ĳɼ���ͼ��(��0��ʼ)
                    //if(!(GetMenuState(hMenuMain,LOWORD(wParam), MF_BYCOMMAND) & MF_CHECKED))     //������ͼ��δ��ѡ��
                    {
                        printf("123");
                     if(MessageBox (hWnd, TEXT("ȷ��Ҫ�޸�ͼ����"), TEXT("ȷ��"),  MB_YESNO|MB_ICONQUESTION ) == IDYES )   //��Ϣ�����ȷ��
                     {
                         CheckMenuRadioItem(hMenuMain,ID_40002,ID_40004, LOWORD(wParam), MF_BYCOMMAND);           //�ȸı�˵���ѡ��״̬���ٸı�ͼ��
                         SetClassLong(hWnd, GCL_HICON, (long)LoadIcon(hInst, MAKEINTRESOURCE(id_icon[d])));
                         if(d == 2)                                                  //���ǵ�����ͼ�걻ѡ�У��һ�
                         {
                             EnableMenuItem(hMenuMain, 2, MF_GRAYED|MF_BYPOSITION);
                             DrawMenuBar(hWnd);
                         }
                         else                                                       //�����ǵ�����ͼ�꣬���һ�
                         {
                             EnableMenuItem(hMenuMain, 2, MF_ENABLED|MF_BYPOSITION);
                             DrawMenuBar(hWnd);
                         }
                         hdc = GetDC(hWnd);                                         //��������
                         TextOut(hdc, 10, 10, showIcon[d], lstrlen(showIcon[d]));
                         ReleaseDC(hWnd, hdc);
                     }
                    }
                    return 0;
                }

                case ID_40005:                                      //����Ҫ��ӡ��Ϣ
                case ID_40006:
                case ID_40007:
                case ID_40008:
                {
                    int d2 = LOWORD(wParam) - ID_40005;                           //����Ҫ��ӡ�ڼ�����Ϣ(��0��ʼ)
                    if(!(GetMenuState(hMenuMain,LOWORD(wParam), MF_BYCOMMAND) & MF_CHECKED))       //�ȼ��һ��ʼ�Ƿ��ѱ�ѡ��
                    {
                        CheckMenuItem(hMenuMain, LOWORD(wParam), MF_CHECKED);                  //�ȸı�˵���ѡ��״̬���ٸı�����
                        hdc = GetDC(hWnd);
                        TextOut(hdc, 100 + d2 / 2 * 100, 100 + d2 % 2 * 100, showMesg[d2], lstrlen(showMesg[d2]));
                        ReleaseDC(hWnd, hdc);
                    }
                    else                                                               //�����ѱ�ѡ�У�������Ϣ��ʧ
                    {
                        CheckMenuItem(hMenuMain, LOWORD(wParam), MF_UNCHECKED);
                        hdc = GetDC(hWnd);
                        TextOut(hdc, 100 + d2 / 2 * 100, 100 + d2 % 2 * 100, showMesg[4], lstrlen(showMesg[4]));
                        ReleaseDC(hWnd, hdc);
                    }
                    return 0;
                }

                case ID_40009:                                             //���������Ϣ
                {
                    MessageBox(hWnd, TEXT("2020MPADLab3: Windows��Դʹ��(1)\nͼ�ꡢ�˵�����̬�˵������ټ�����Ϣ��\n����ѧ�ţ�10185102153 ���������ӷ�\n"),
                               TEXT("2020MPAD"), MB_ICONEXCLAMATION|MB_OK);
                    return 0;
                }

                case ID_FILE_EXIT:                                       //����̬���ɵ�Exit��Ϣ
                {
                    printf("11111");
                    PostQuitMessage(0);
                    return 0;
                }
                return 0;
            }
         }

        //case WM_DESTROY: // ���ڹر�
       //     printf("11111111");
         //   PostQuitMessage(0);                    //����Ϊ0��ʾ������������

            return 0;

       default:  // ȱʡ��Ϣ�Ĵ���

           return DefWindowProc(hWnd, message, wParam, lParam);
   }

}
