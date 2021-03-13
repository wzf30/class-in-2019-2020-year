#include <windows.h>
#include <stdio.h>
#include "resource.h"

LPCTSTR showIcon[3] = {"当前使用的图标是：图标1", "当前使用的图标是：图标2", "当前使用的图标是：图标3"};
LPCTSTR showMesg[5] = {"显示信息1", "显示信息2", "显示信息3", "显示信息4", "                 "};
int id_icon[] = {IDI_ICON1, IDI_ICON2, IDI_ICON3};
HWND hWnd;

//***************************************************************************************
BOOL InitApplication(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance,     // 入口函数
                   HINSTANCE,
                   LPSTR     lpCmdLine,
                   int       nCmdShow  )
{
    if (!InitApplication(hInstance))       // 应用初始化
        return FALSE;

    if (!InitInstance(hInstance,nCmdShow)) // 实例初始化
        return FALSE;

    MSG msg;
    HACCEL haccel = LoadAccelerators(hInstance,MAKEINTRESOURCE(IDR_ACCELERATOR1));           //Load进加速键

    while (GetMessage(&msg, NULL, 0, 0))   // 消息循环
    {
        if(!TranslateAccelerator(hWnd, haccel, &msg))
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

//***************************************************************************************

BOOL InitApplication(HINSTANCE hInstance)   // 应用初始化
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
    wc.lpszMenuName     = MAKEINTRESOURCE(IDR_MENU1);               //这个宏将资源的ID表示转化为字符串表示，或者直接用字符串
    wc.lpszClassName    = TEXT("My1stWClass");  // Name of the window class

    return RegisterClass(&wc);
}

//***************************************************************************************

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)  // 实例初始化
{
    hWnd = CreateWindow(TEXT("My1stWClass"),     // Name of the window class
                             TEXT("2020MPADLab3: Windows资源使用(1) 姓名：汪子凡 学号：10185102153"), // Title of the window
                             WS_OVERLAPPEDWINDOW,                   //窗口风格
                             CW_USEDEFAULT,                     //初始位置
                             CW_USEDEFAULT,
                             CW_USEDEFAULT,                      //宽高
                             CW_USEDEFAULT,
                             NULL,                                   //父窗口句柄
                             NULL,                                   //菜单句柄，需要的是句柄,可以在两个地方加载菜单，两个都有，以这个地方为准
                             hInstance,
                             NULL                                        );
    if (!hWnd) return FALSE;

    HMENU hMenuMain = GetMenu(hWnd);
    HMENU hMenuFile = CreatePopupMenu();                                               //动态生成File下拉菜单
    InsertMenu(hMenuMain, 0, MF_BYPOSITION|MF_STRING|MF_POPUP, (UINT)hMenuFile, "&File");  //将File菜单插入主菜单
    AppendMenu(hMenuFile, MF_STRING, ID_FILE_EXIT, (LPTSTR)"Exit\tCtrl+Shift+Delete");     //在File菜单下插入子菜单(并赋予ID)
                                                                                      //ID_FILE_EXIT在rc文件中定义了加速键
    CheckMenuRadioItem(hMenuMain,ID_40002,ID_40004, ID_40002, MF_BYCOMMAND);             //初始选中第一个图标状态

    ShowWindow(hWnd, nCmdShow);                                    //产生VM_SHOWWINDOW消息和VM_SIZE消息
    UpdateWindow(hWnd);                                            //产生VM_PAINT消息

    HDC hdc = GetDC(hWnd);
    TextOut(hdc, 10, 10, showIcon[0], lstrlen(showIcon[0]));                                 //显示文字
    ReleaseDC(hWnd, hdc);

    return TRUE;
}

//***************************************************************************************

// 窗口过程函数

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;                         //设备上下文句柄
    //PAINTSTRUCT ps;
    static HINSTANCE hInst;
    switch (message) {
       case WM_CREATE:                            //获取应用的句柄
         {
             hInst = ((LPCREATESTRUCT)lParam)-> hInstance;
         }

         /*case WM_PAINT:
         {
            hdc = BeginPaint(hWnd, &ps);
            TextOut(hdc, 500, 500, showMesg[0], lstrlen(showMesg[0]));
            EndPaint(hWnd, &ps);                    //用这两个函数可以告诉系统需要重画的地方已经重画，而getDC不行，会一直重复命令
         }*/

         case WM_INITMENU:
         {
           // HMENU hMenuMain = (HMENU)wParam;
         }

         case WM_COMMAND:                                      //若是COMMAND消息
         {
            HMENU hMenuMain = GetMenu(hWnd);
            switch(LOWORD(wParam))
            {
                printf("%d\n", LOWORD(wParam));
                case ID_40002:                                 //若是选中图标下的按键
                case ID_40003:
                case ID_40004:
                {
                    printf("%d\n", LOWORD(wParam));
                    int d = LOWORD(wParam) - ID_40002;                        //计算一下要改成几个图标(从0开始)
                    //if(!(GetMenuState(hMenuMain,LOWORD(wParam), MF_BYCOMMAND) & MF_CHECKED))     //如果这个图标未被选中
                    {
                        printf("123");
                     if(MessageBox (hWnd, TEXT("确认要修改图标吗？"), TEXT("确认"),  MB_YESNO|MB_ICONQUESTION ) == IDYES )   //消息框进行确认
                     {
                         CheckMenuRadioItem(hMenuMain,ID_40002,ID_40004, LOWORD(wParam), MF_BYCOMMAND);           //先改变菜单的选中状态，再改变图标
                         SetClassLong(hWnd, GCL_HICON, (long)LoadIcon(hInst, MAKEINTRESOURCE(id_icon[d])));
                         if(d == 2)                                                  //若是第三个图标被选中，灰化
                         {
                             EnableMenuItem(hMenuMain, 2, MF_GRAYED|MF_BYPOSITION);
                             DrawMenuBar(hWnd);
                         }
                         else                                                       //若不是第三个图标，不灰化
                         {
                             EnableMenuItem(hMenuMain, 2, MF_ENABLED|MF_BYPOSITION);
                             DrawMenuBar(hWnd);
                         }
                         hdc = GetDC(hWnd);                                         //更新文字
                         TextOut(hdc, 10, 10, showIcon[d], lstrlen(showIcon[d]));
                         ReleaseDC(hWnd, hdc);
                     }
                    }
                    return 0;
                }

                case ID_40005:                                      //若是要打印消息
                case ID_40006:
                case ID_40007:
                case ID_40008:
                {
                    int d2 = LOWORD(wParam) - ID_40005;                           //计算要打印第几个消息(从0开始)
                    if(!(GetMenuState(hMenuMain,LOWORD(wParam), MF_BYCOMMAND) & MF_CHECKED))       //先检查一开始是否已被选中
                    {
                        CheckMenuItem(hMenuMain, LOWORD(wParam), MF_CHECKED);                  //先改变菜单的选中状态，再改变文字
                        hdc = GetDC(hWnd);
                        TextOut(hdc, 100 + d2 / 2 * 100, 100 + d2 % 2 * 100, showMesg[d2], lstrlen(showMesg[d2]));
                        ReleaseDC(hWnd, hdc);
                    }
                    else                                                               //若是已被选中，则让消息消失
                    {
                        CheckMenuItem(hMenuMain, LOWORD(wParam), MF_UNCHECKED);
                        hdc = GetDC(hWnd);
                        TextOut(hdc, 100 + d2 / 2 * 100, 100 + d2 % 2 * 100, showMesg[4], lstrlen(showMesg[4]));
                        ReleaseDC(hWnd, hdc);
                    }
                    return 0;
                }

                case ID_40009:                                             //处理关于消息
                {
                    MessageBox(hWnd, TEXT("2020MPADLab3: Windows资源使用(1)\n图标、菜单、动态菜单、加速键、消息框\n作者学号：10185102153 姓名：汪子凡\n"),
                               TEXT("2020MPAD"), MB_ICONEXCLAMATION|MB_OK);
                    return 0;
                }

                case ID_FILE_EXIT:                                       //处理动态生成的Exit消息
                {
                    printf("11111");
                    PostQuitMessage(0);
                    return 0;
                }
                return 0;
            }
         }

        //case WM_DESTROY: // 窗口关闭
       //     printf("11111111");
         //   PostQuitMessage(0);                    //设置为0表示程序正常结束

            return 0;

       default:  // 缺省消息的处理

           return DefWindowProc(hWnd, message, wParam, lParam);
   }

}
