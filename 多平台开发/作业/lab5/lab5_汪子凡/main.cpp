#include <windows.h>
#include <stdio.h>
#include "resource.h"

TCHAR buffer[60];
HWND hWnd, hDlgModeless = 0;
int is_check[3], is_choose;

//***************************************************************************************
BOOL InitApplication(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK ModelDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK ModelessDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

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
    HACCEL haccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));           //Load进加速键

    while (GetMessage(&msg, NULL, 0, 0))   // 消息循环
    {
        if((!IsWindow(hDlgModeless) || !IsDialogMessage(hDlgModeless, &msg)) && (!TranslateAccelerator(hWnd, haccel, &msg)))
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
    wc.lpszMenuName     = MAKEINTRESOURCE(IDR_MENU_CH);               //这个宏将资源的ID表示转化为字符串表示，或者直接用字符串
    wc.lpszClassName    = TEXT("My1stWClass");  // Name of the window class

    return RegisterClass(&wc);
}

//***************************************************************************************

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)  // 实例初始化
{
    hWnd = CreateWindow(TEXT("My1stWClass"),     // Name of the window class
                             TEXT("2020MPADLab5: Windows资源使用(2)  姓名：汪子凡 学号：10185102153"), // Title of the window
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

    ShowWindow(hWnd, nCmdShow);                                    //产生VM_SHOWWINDOW消息和VM_SIZE消息
    UpdateWindow(hWnd);                                            //产生VM_PAINT消息

    return TRUE;
}

//***************************************************************************************

// 窗口过程函数

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;                         //设备上下文句柄
    PAINTSTRUCT ps;
    static HINSTANCE hInst;
    static HMENU hMenuMain;
    static int id_bitmap[3] = {IDB_BITMAP1, IDB_BITMAP2, IDB_BITMAP3}, Bitmap_width, Bitmap_height;
    static int language_choose, cursor_choose, id_text;                    //记录相应的语言选择，光标选择，以及这时字符串表的起始位置

    switch (message)
    {
       case WM_CREATE:                            //获取应用的句柄
         {
             hInst = ((LPCREATESTRUCT)lParam)-> hInstance;
             language_choose = ID_LANGUAGE_CH;
             id_text = IDS_CHINESE1;
             cursor_choose = 0;

             HBITMAP hbitmap = LoadBitmap(hInst, MAKEINTRESOURCE(id_bitmap[0]));
             BITMAP bitmap;
             GetObject(hbitmap, sizeof(BITMAP), &bitmap);          //将位图有关信息放入bitmap
             Bitmap_width = bitmap.bmWidth;
             Bitmap_height = bitmap.bmHeight;
             DeleteObject(hbitmap);
             break;
         }

         case WM_PAINT:
         {
            hdc = BeginPaint(hWnd, &ps);
            LoadString(hInst, id_text + cursor_choose, buffer, 60);      //在字符串表里获取字符串
            TextOut(hdc, 10, 10, buffer, lstrlen(buffer));

            HDC hmemDC = CreateCompatibleDC(hdc);            //内存设备描述表
            HBITMAP hbitmap;
            for(int i = 0; i < 3; i++)                   //依次检查三个复选和单选位图哪些要显示
            {
                if(is_check[i])
                {
                    hbitmap = LoadBitmap(hInst, MAKEINTRESOURCE(id_bitmap[i]));
                    SelectObject(hmemDC, hbitmap);
                    BitBlt(hdc, 100, 50 + 200*i, Bitmap_width, Bitmap_height, hmemDC, 0, 0, SRCCOPY);
                    //DELETEDC(hMemDC);
                    DeleteObject(hbitmap);
                }
            }
            hbitmap = LoadBitmap(hInst, MAKEINTRESOURCE(id_bitmap[is_choose]));
            SelectObject(hmemDC, hbitmap);
            BitBlt(hdc, 400, 50, Bitmap_width, Bitmap_height, hmemDC, 0, 0, SRCCOPY);

            DeleteObject(hbitmap);
            DeleteDC(hmemDC);                        //释放资源
            EndPaint(hWnd, &ps);                    //用这两个函数可以告诉系统需要重画的地方已经重画，而getDC不行，会一直重复命令
         }

         case WM_INITMENU:                       //显示勾选内容
         {
            hMenuMain = (HMENU)wParam;
            CheckMenuRadioItem(hMenuMain, ID_CUSSOR1, ID_CUSSOR3, cursor_choose + ID_CUSSOR1, MF_BYCOMMAND);
            CheckMenuRadioItem(hMenuMain, ID_LANGUAGE_CH, ID_LANGUAGE_EG, language_choose, MF_BYCOMMAND);
         }

         case WM_COMMAND:                                      //若是COMMAND消息
         {
            HMENU hMenuMain = GetMenu(hWnd);
            switch(LOWORD(wParam))
            {
                printf("%d\n", LOWORD(wParam));
                case ID_CUSSOR1:
                case ID_CUSSOR2:
                case ID_CUSSOR3:
                {
                    int iSelection = LOWORD(wParam) - ID_CUSSOR1;
                    if(!(GetMenuState(hMenuMain,LOWORD(wParam), MF_BYCOMMAND) & MF_CHECKED))
                    {
                        CheckMenuRadioItem(hMenuMain,ID_CUSSOR1,ID_CUSSOR3, LOWORD(wParam), MF_BYCOMMAND);
                        if(iSelection == 0)
                            SetClassLong(hWnd, GCL_HCURSOR, (long)LoadCursor(NULL,IDC_ARROW));
                        else if(iSelection == 1)
                            SetClassLong(hWnd, GCL_HCURSOR, (long)LoadCursor(NULL,IDC_CROSS));
                        else
                            SetClassLong(hWnd, GCL_HCURSOR, (long)LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR1)));
                        cursor_choose = iSelection;
                        DrawMenuBar(hWnd);
                        InvalidateRect(hWnd, NULL, TRUE);
                    }
                    break;
                }

                case ID_DIALOG_A:                                                //创建模式对话框
                    DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1),hWnd, ModelDlgProc);
                    break;
                case ID_DIALOG_B:
                    if(!IsWindow(hDlgModeless))                                  //创建无模式对话框(防止重复打开)
                    {
                       hDlgModeless = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG2), hWnd, ModelessDlgProc);
                       ShowWindow(hDlgModeless, SW_SHOW);
                    }
                   break;
                case ID_DIALOG_C:
                {
                    OPENFILENAME dlgfile;
                    TCHAR szFile[300];
                    ZeroMemory(&dlgfile, sizeof(dlgfile));
                    dlgfile.lStructSize = sizeof(dlgfile);      //确定结构的大小
                    dlgfile.hwndOwner = hWnd;                 //指定它的父窗口，如果为NULL，表示通用对话框
                    dlgfile.lpstrFile = szFile;                 //用于保存文件的完整路径及文件名
                    dlgfile.lpstrFile[0] = '\0';
                    dlgfile.nMaxFile = sizeof(szFile);         //指示上面结构的大小
                    dlgfile.lpstrFilter = ("All Files(*.*)\0*.*\0Python source(*.py)\0*.py\0C++ Files(*.cpp)\0*.cpp\0\0");
                    dlgfile.nFilterIndex = 1;
                    dlgfile.lpstrFileTitle = NULL;              //用于保存文件名
                    dlgfile.nMaxFileTitle = 0;
                    dlgfile.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
                    dlgfile.lpstrTitle = ("打开");
                    if(GetOpenFileName(&dlgfile))
                        MessageBox(hWnd, dlgfile.lpstrFile, "文件名", MB_ICONINFORMATION|MB_OK);
                    break;
                }

                case ID_LANGUAGE_CH:
                    if(language_choose == ID_LANGUAGE_CH)
                            break;
                    language_choose = ID_LANGUAGE_CH;
                    SetMenu(hWnd, LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENU_CH)));
                    DestroyMenu(hMenuMain);
                    id_text = IDS_CHINESE1;
                    InvalidateRect(hWnd, NULL, TRUE);
                    break;
                case ID_LANGUAGE_EG:
                    if(language_choose == ID_LANGUAGE_EG)
                            break;
                    language_choose = ID_LANGUAGE_EG;
                    SetMenu(hWnd, LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENU_EG)));
                    DestroyMenu(hMenuMain);
                    id_text = IDS_ENGLISH1;
                    InvalidateRect(hWnd, NULL, TRUE);
                    break;

                case ID_MESSAGESHOW:
                    MessageBox (hWnd, TEXT ("2020MPADLab5(SDK)资源2\n光标,对话框，位图，字符串\n\n作者学号: 10185102153 姓名: 汪子凡"),
                    "2020MPADlab5", MB_ICONINFORMATION|MB_OK);
                    break;
                case ID_FILE_EXIT:
                    PostQuitMessage(0);
                    return 0;
            }
            break;
         }


        case WM_DESTROY: // 窗口关闭
            PostQuitMessage(0);                    //设置为0表示程序正常结束
            return 0;

    }
    return DefWindowProc(hWnd, message, wParam, lParam);

}

BOOL CALLBACK ModelDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int id_check[3] = {IDC_CHECK1,IDC_CHECK2,IDC_CHECK3};
    static int tmp_check[3];

    switch (message)
    {
        case WM_INITDIALOG:
        {
            for(int i = 0; i < 3; i++)
               if(is_check[i])
                    CheckDlgButton(hDlg, id_check[i], BST_CHECKED);
            for(int i = 0; i < 3; i++)
                tmp_check[i] = 0;

            SetFocus(GetDlgItem(hDlg, IDOK1));           //设置默认焦点
            return FALSE;
        }

        case WM_COMMAND:
        {
            switch (LOWORD(wParam))
            {
                case IDOK1:
                    for(int i = 0; i < 3; i++)
                        is_check[i] = (is_check[i] + tmp_check[i]) % 2;
                    InvalidateRect(GetParent(hDlg), NULL, TRUE);      //表示刷新整个客户区(原来的要清空)
                    EndDialog(hDlg, 0);
                    return TRUE;

                case IDCANCEL1:
                    EndDialog(hDlg, 0);
                    return TRUE;

                case IDC_CHECK1:
                case IDC_CHECK2:
                case IDC_CHECK3:
                {
                    int iSelection = LOWORD(wParam) - IDC_CHECK1;
                    tmp_check[iSelection] ^= 1;
                    return TRUE;
                }
            }
        }

        case WM_DESTROY:
        {
            EndDialog(hDlg, 0);
            return TRUE;
        }
    }
    return FALSE;
}

BOOL CALLBACK ModelessDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int id_choose[3] = {IDC_RADIO1, IDC_RADIO2, IDC_RADIO3};
    static int tmp_choose;

    switch (message)
    {
        case WM_INITDIALOG:
        {
            CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO3, id_choose[is_choose]);
            tmp_choose = is_choose;
            return FALSE;
        }

        case WM_COMMAND:
        {
            switch (LOWORD(wParam))
            {
                case IDOK2:
                    if(is_choose != tmp_choose)
                    {
                        is_choose = tmp_choose;
                        InvalidateRect(GetParent(hDlg), NULL, TRUE);      //表示刷新整个客户区(原来的要清空)
                    }
                    DestroyWindow(hDlg);
                    return TRUE;

                case IDCANCEL2:
                    DestroyWindow(hDlg);
                    return TRUE;

                case IDC_RADIO1:
                case IDC_RADIO2:
                case IDC_RADIO3:
                {
                    tmp_choose = LOWORD(wParam) - IDC_RADIO1;
                    return TRUE;
                }
            }
        }

        case WM_CLOSE:
        {
            DestroyWindow(hDlg);
            return TRUE;
        }
    }
    return FALSE;
}
