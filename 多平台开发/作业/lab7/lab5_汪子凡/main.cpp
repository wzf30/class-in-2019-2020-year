#include <windows.h>
#include <stdio.h>
#include "resource.h"

HWND hWnd;
POINT point[4], point2[4];
int nState = 0, nState_triangle = 0;
char myfile[300];
char buffer[3][200], tmp_read[200];
//***************************************************************************************
BOOL InitApplication(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK ModelDlgProc1(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK ModelDlgProc2(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

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
                             TEXT("2020MPADLab7: 绘制图形 姓名：汪子凡 学号：10185102153"), // Title of the window
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

    static LPCTSTR showMesg[8] = {"Beizer曲线绘制：",
                                "1.按下鼠标左键，即确定端点P0，可以拖动，放开左键确定端点P3",
                                "2.再次按下左键，可以拖动，放开左键确定控制点P1(直线P0P1与曲线相切)",
                                "3.再次按下左键，可以拖动，放开左键确定控制点P2(直线P2P3与曲线相切)",
                                "三角形绘制：",
                                "1.按下鼠标左键，即确定顶点P0，可以拖动，放开左键确定端点P1",
                                "2.再次按下左键，可以拖动，放开左键确定控制点P2",
                                "绘制完成！可以重新绘制或者保存"};
    static int choose = 0;
    static BOOL fErase=FALSE;
    int xPos,yPos;

    switch (message)
    {
        case WM_CREATE:                            //获取应用的句柄
        {
             hInst = ((LPCREATESTRUCT)lParam)-> hInstance;

             FILE *fp = fopen("Recently.txt", "r");                            //读入最近浏览的文件
             fscanf(fp, "%s %s %s", buffer[0], buffer[1], buffer[2]);
             fclose(fp);
             DrawMenuBar(hWnd);
             break;
       }

       case WM_PAINT:
       {
            hdc = BeginPaint(hWnd, &ps);
            if(choose == 0)                 //若是画Beizer曲线
            {
                TextOut(hdc, 10, 10, showMesg[0], lstrlen(showMesg[0]));
                TextOut(hdc, 10, 30, showMesg[1], lstrlen(showMesg[1]));
                TextOut(hdc, 10, 50, showMesg[2], lstrlen(showMesg[2]));
                TextOut(hdc, 10, 70, showMesg[3], lstrlen(showMesg[3]));
                if(nState == 2)                                             //若是只完成了第一步
                {
                    MoveToEx(hdc,point[0].x,point[0].y,NULL);
                    LineTo(hdc,point[3].x,point[3].y);
                }
                else if(nState > 2)                                           //已完成第一步之后
                {
                    TextOut(hdc, 10, 100, showMesg[7], lstrlen(showMesg[7]));
                    PolyBezier(hdc,point,4);
                }
            }
            else                             //若是画三角形
            {
                TextOut(hdc, 10, 10, showMesg[4], lstrlen(showMesg[4]));
                TextOut(hdc, 10, 30, showMesg[5], lstrlen(showMesg[5]));
                TextOut(hdc, 10, 50, showMesg[6], lstrlen(showMesg[6]));
                if(nState_triangle == 2)                                   //若是只完成了第一步
                {
                    MoveToEx(hdc, point2[0].x, point2[0].y, NULL);
                    LineTo(hdc, point2[1].x, point2[1].y);
                }
                else if(nState_triangle > 2)
                {
                    TextOut(hdc, 10, 100, showMesg[7], lstrlen(showMesg[7]));
                    MoveToEx(hdc, point2[0].x, point2[0].y, NULL);
                    LineTo(hdc, point2[1].x, point2[1].y);
                    LineTo(hdc, point2[2].x, point2[2].y);
                    LineTo(hdc, point2[0].x, point2[0].y);
                }
            }
            EndPaint(hWnd, &ps);                    //用这两个函数可以告诉系统需要重画的地方已经重画，而getDC不行，会一直重复命令

            break;
        }

        case WM_INITMENU:                       //显示勾选内容
        {
            hMenuMain = (HMENU)wParam;                           //显示是三角形还是曲线
            CheckMenuRadioItem(hMenuMain, ID_CHANGE_BEIZER, ID_CHANGE_TRIANGLE, choose + 40007, MF_BYCOMMAND);

            ModifyMenu(hMenuMain, ID_RECENTFILES_1, MF_BYCOMMAND, ID_RECENTFILES_1, buffer[0]);
            ModifyMenu(hMenuMain, ID_RECENTFILES_2, MF_BYCOMMAND, ID_RECENTFILES_2, buffer[1]);
            ModifyMenu(hMenuMain, ID_RECENTFILES_3, MF_BYCOMMAND, ID_RECENTFILES_3, buffer[2]);
            break;
        }

        case WM_LBUTTONDOWN:
            xPos = (signed short)LOWORD(lParam);   yPos = (signed short)HIWORD(lParam);            //强制类型转换保证往左和上不会出bug
            SetCapture(hWnd);
            hdc = GetDC(hWnd);
            SetROP2(hdc,R2_NOTXORPEN);
            if(choose == 0)
                switch (nState)
                {
                    case 0:
                        point[0].x = xPos;  point[0].y = yPos;                 //获取端点P0
                        nState = 1;
                        break;
                    case 2:
                        MoveToEx(hdc,point[0].x,point[0].y,NULL);            //消除原来的直线
                        LineTo(hdc,point[3].x,point[3].y);
                        point[1].x=xPos;   point[1].y=yPos;
                        point[2].x=xPos;   point[2].y=yPos;
                        PolyBezier(hdc,point,4);
                        nState=3;
                        fErase=TRUE;
                        break;
                    case 4:
                        PolyBezier(hdc,point,4);
                        point[2].x=xPos;   point[2].y=yPos;
                        PolyBezier(hdc,point,4);
                        nState=5;
                        fErase=TRUE;
                        break;
                }
            else
                switch(nState_triangle)
                {
                    case 0:
                        point2[0].x = xPos;  point2[0].y = yPos;
                        nState_triangle =1;
                        break;
                    case 2:
                        point2[2].x = xPos;   point2[2].y = yPos;
                        point2[2].x = xPos;   point2[2].y = yPos;

                        MoveToEx(hdc, point2[0].x, point2[0].y, NULL);
                        LineTo(hdc, point2[2].x, point2[2].y);
                        MoveToEx(hdc, point2[1].x, point2[1].y, NULL);
                        LineTo(hdc, point2[2].x, point2[2].y);

                        nState_triangle = 3;
                        fErase = TRUE;
                        break;

                }
            ReleaseDC(hWnd,hdc);
            break;

        case WM_LBUTTONUP:
            xPos = (signed short)LOWORD(lParam);   yPos = (signed short)HIWORD(lParam);
            fErase = FALSE;
            if(choose == 0)
            {
                switch (nState)
                {
                    case 1:
                        point[3].x = xPos;   point[3].y = yPos;
                        nState=2;
                        break;
                    case 3:
                        point[1].x = xPos;   point[1].y = yPos;
                        nState=4;
                        break;
                    case 5:
                        point[2].x = xPos;   point[2].y = yPos;
                        nState = 6;                                      //状态结束，置nstate为6
                        InvalidateRect(hWnd, NULL, TRUE);
                        break;
                }
            }
            else
            {
                switch(nState_triangle)
                {
                    case 1:
                        point2[1].x = xPos;   point2[1].y = yPos;
                        nState_triangle = 2;
                        break;
                    case 3:
                        point2[2].x = xPos;   point2[2].y = yPos;
                        nState_triangle = 4;                                  //状态结束，置 nState_triangle为4
                        InvalidateRect(hWnd, NULL, TRUE);
                        break;
                }
            }
           ReleaseCapture();
           break;

        case WM_MOUSEMOVE:
            xPos = (signed short)LOWORD(lParam);   yPos = (signed short)HIWORD(lParam);
            if(wParam&MK_LBUTTON)
            {
                hdc=GetDC(hWnd);
                SetROP2(hdc,R2_NOTXORPEN);
                if(choose == 0)                              //bizer曲线
                {
                    switch (nState)
                    {
                        case 1:
                            if(fErase)                  //第一次划线的时候fErase还是0，没有线可以擦
                            {
                                MoveToEx(hdc,point[0].x,point[0].y,NULL);
                                LineTo(hdc,point[3].x,point[3].y);
                            }
                            point[3].x=xPos;    point[3].y=yPos;                        //划线
                            MoveToEx(hdc,point[0].x,point[0].y,NULL);
                            LineTo(hdc,point[3].x,point[3].y);
                            break;
                        case 3:
                            if(fErase)  PolyBezier(hdc,point,4);
                            point[1].x=xPos;     point[1].y=yPos;
                            point[2].x=xPos;     point[2].y=yPos;
                            PolyBezier(hdc,point,4);
                            break;
                        case 5:
                            if(fErase)  PolyBezier(hdc,point,4);
                            point[2].x=xPos;     point[2].y=yPos;
                            PolyBezier(hdc,point,4);
                            break;
                    }
              }
              else
              {
                  switch(nState_triangle)
                  {
                        case 1:
                            if(fErase)                  //第一次划线的时候fErase还是0，没有线可以擦
                            {
                                MoveToEx(hdc,point2[0].x,point2[0].y,NULL);
                                LineTo(hdc,point2[1].x,point2[1].y);
                            }
                            point2[1].x = xPos;    point2[1].y = yPos;                        //划线
                            MoveToEx(hdc, point2[0].x, point2[0].y, NULL);
                            LineTo(hdc,point2[1].x, point2[1].y);
                            break;
                        case 3:
                            if(fErase)
                            {
                                MoveToEx(hdc, point2[0].x, point2[0].y, NULL);
                                LineTo(hdc, point2[2].x, point2[2].y);
                                MoveToEx(hdc, point2[1].x, point2[1].y, NULL);
                                LineTo(hdc, point2[2].x, point2[2].y);
                            }
                            point2[2].x = xPos;    point2[2].y = yPos;

                            MoveToEx(hdc, point2[0].x, point2[0].y, NULL);
                            LineTo(hdc, point2[2].x, point2[2].y);
                            MoveToEx(hdc, point2[1].x, point2[1].y, NULL);
                            LineTo(hdc, point2[2].x, point2[2].y);
                            break;

                  }
              }
              fErase=TRUE;
              ReleaseDC(hWnd,hdc);
          }
          break;


         case WM_COMMAND:                                      //若是COMMAND消息
         {
            HMENU hMenuMain = GetMenu(hWnd);
            switch(LOWORD(wParam))
            {
                case ID_CHANGE_BEIZER:
                case ID_CHANGE_TRIANGLE:                      //更改曲线类型
                {
                    int iSelection = LOWORD(wParam) - 40007;
                    if(iSelection == choose)
                        break;
                    if(MessageBox (hWnd, TEXT("确认要修改画图类型吗？"), TEXT("确认"),  MB_YESNO|MB_ICONQUESTION ) == IDYES )   //消息框进行确认
                    {
                        CheckMenuRadioItem(hMenuMain,ID_CHANGE_BEIZER, ID_CHANGE_TRIANGLE, LOWORD(wParam), MF_BYCOMMAND);
                        choose = iSelection;
                        fErase = false;  nState = nState_triangle = 0;
                        InvalidateRect(hWnd, NULL, true);
                    }
                    break;
                }

                case ID_SETTINGS_REPAINT:                        //重新绘制
                    if(MessageBox (hWnd, TEXT("确认要重新绘制吗？"), TEXT("确认"),  MB_YESNO|MB_ICONQUESTION ) == IDYES )
                    {
                        fErase = false;  nState = nState_triangle = 0;
                        InvalidateRect(hWnd, NULL, true);
                    }
                    break;

                case ID_SETTINGS_MODIFY:
                    if(choose == 0)
                    {
                        DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1),hWnd, ModelDlgProc1);
                    }
                    else
                    {
                        DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG2),hWnd, ModelDlgProc2);
                    }
                    break;

                case ID_HELP_LAB7HELP:
                    MessageBox (hWnd, TEXT ("本次作业可以绘制两种曲线(用鼠标画或者输入坐标),实现了基本的文件保存新建打开功能(以txt格式)"),
                    "2020MPADlab7", MB_ICONINFORMATION|MB_OK);
                    break;

                case ID_HELP_ABOUT:
                    MessageBox (hWnd, TEXT ("2020MPADLab7 绘图\n\n作者学号: 10185102153 姓名: 汪子凡"),
                    "2020MPADlab7", MB_ICONINFORMATION|MB_OK);
                    break;

                case ID_FILE_NEW:
                case ID_FILE_OPEN:
                case ID_FILE_SAVE:
                case ID_FILE_SAVEAS:
                {
                    OPENFILENAME dlgfile;
                    TCHAR szFile[300];
                    ZeroMemory(&dlgfile, sizeof(dlgfile));
                    dlgfile.lStructSize = sizeof(dlgfile);      //确定结构的大小
                    dlgfile.hwndOwner = hWnd;                 //指定它的父窗口，如果为NULL，表示通用对话框
                    dlgfile.lpstrFile = szFile;                 //用于保存文件的完整路径及文件名
                    dlgfile.lpstrFile[0] = '\0';
                    dlgfile.nMaxFile = sizeof(szFile);         //指示上面结构的大小
                    dlgfile.lpstrFilter = ("txt source(*.txt)\0*.txt\0");
                    dlgfile.nFilterIndex = 1;
                    dlgfile.lpstrFileTitle = NULL;              //用于保存文件名
                    dlgfile.nMaxFileTitle = 0;
                    dlgfile.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

                    int tmp_array[4][2], tmp_choose;
                    if(LOWORD(wParam) == ID_FILE_OPEN)
                    {
                        dlgfile.lpstrTitle = ("打开");
                        if(GetOpenFileName(&dlgfile))
                        {
                            FILE *fp = fopen(dlgfile.lpstrFile, "r");               //打开文件后先读取文件信息
                            if(fscanf(fp, "%d %d %d %d %d %d %d %d %d", &tmp_choose, &tmp_array[0][0], &tmp_array[0][1], &tmp_array[1][0],
                            &tmp_array[1][1], &tmp_array[2][0], &tmp_array[2][1], &tmp_array[3][0], &tmp_array[3][1]) == 9)
                            {
                                sprintf(myfile, "%s", dlgfile.lpstrFile);                //更改当前的myfile
                                choose = tmp_choose;
                                if(choose == 0)
                                {
                                    for(int i = 0; i < 4; i++)
                                        point[i].x = tmp_array[i][1], point[i].y = tmp_array[i][0];
                                    nState = 6;
                                }
                                else
                                {
                                    for(int i = 0; i < 3; i++)
                                        point2[i].x = tmp_array[i][1], point2[i].y = tmp_array[i][0];
                                    nState_triangle = 4;
                                }
                            }
                            else                                              //若是文件内信息读取格式不正确
                                MessageBox (hWnd, TEXT ("打开失败"), "提示", MB_ICONINFORMATION|MB_OK);
                            fclose(fp);
                        }
                        else                                                 //若是文件没有打开
                             MessageBox (hWnd, TEXT ("打开失败"), "提示", MB_ICONINFORMATION|MB_OK);
                    }

                    else if(LOWORD(wParam) == ID_FILE_NEW)
                    {
                        dlgfile.lpstrTitle = ("新建");
                        if(GetSaveFileName(&dlgfile))
                        {
                            sprintf(myfile, "%s", dlgfile.lpstrFile);
                            char b[]=".txt", *pa = myfile;            //创建文件时需要加上.txt后缀
                            while(*pa) pa++;
                            sprintf(pa, "%s", b);
                            printf("%s", myfile);
                            FILE *fp = fopen(myfile, "w");
                            fclose(fp);
                            choose = 0;  nState = 0;  fErase = FALSE;           //默认新建的文件为bizer曲线
                        }
                    }

                    else if(LOWORD(wParam) == ID_FILE_SAVE && myfile[0] != '\0')     //若是保存文件
                    {
                        if(nState < 6 && nState_triangle < 4)
                        {
                            MessageBox (hWnd, TEXT ("保存失败！图形未绘制完成"), "提示", MB_ICONINFORMATION|MB_OK);
                            break;
                        }
                        FILE *fp = fopen(myfile, "w");
                        if(choose == 0)
                            fprintf(fp, "0\n%ld %ld\n%ld %ld\n%ld %ld\n%ld %ld", point[0].y, point[0].x, point[1].y, point[1].x,
                            point[2].y, point[2].x, point[3].y, point[3].x);
                        else
                            fprintf(fp, "1\n%ld %ld\n%ld %ld\n%ld %ld\n%ld %ld",point2[0].y, point2[0].x, point2[1].y, point2[1].x,
                            point2[2].y, point2[2].x, point2[3].y, point2[3].x);
                        fclose(fp);
                    }

                    else
                    {
                        if(GetSaveFileName(&dlgfile))
                        {
                            char b[]=".txt", *pa = myfile;                //要加后缀
                            sprintf(myfile, "%s", dlgfile.lpstrFile);
                            while(*pa)
                                pa++;
                            sprintf(pa, "%s", b);
                            FILE *fp = fopen(myfile, "w");
                            if(choose == 0)
                                fprintf(fp, "0\n%ld %ld\n%ld %ld\n%ld %ld\n%ld %ld",point[0].y, point[0].x, point[1].y, point[1].x,
                                point[2].y, point[2].x, point[3].y, point[3].x);
                            else
                                fprintf(fp, "1\n%ld %ld\n%ld %ld\n%ld %ld\n%ld %ld",point2[0].y, point2[0].x, point2[1].y, point2[1].x,
                                point2[2].y, point2[2].x, point2[3].y, point2[3].x);
                            fclose(fp);
                        }
                    }

                    int cur = 0;
                    char tmp_buffer[2][200];
                    for(int i = 0; cur < 2 && i < 3; i++)
                    {
                         if(strcmp(myfile, buffer[i]) != 0)
                            strcpy(tmp_buffer[cur++], buffer[i]);
                    }
                    strcpy(buffer[0], myfile);
                    strcpy(buffer[1], tmp_buffer[0]);
                    strcpy(buffer[2], tmp_buffer[1]);
                    for(int i = 0; i < 3; i++) printf("%s\n", buffer[i]);
                    InvalidateRect(hWnd, NULL, TRUE);
                    DrawMenuBar(hWnd);
                    break;
                }

                case ID_RECENTFILES_1:
                case ID_RECENTFILES_2:
                case ID_RECENTFILES_3:
                {
                    FILE *fp = fopen(buffer[LOWORD(wParam) - ID_RECENTFILES_1], "r");
                    int tmp_array[4][2], tmp_choose;
                    if(fp)
                    {
                        if(fscanf(fp, "%d %d %d %d %d %d %d %d %d", &tmp_choose, &tmp_array[0][0], &tmp_array[0][1], &tmp_array[1][0],
                        &tmp_array[1][1], &tmp_array[2][0], &tmp_array[2][1], &tmp_array[3][0], &tmp_array[3][1]) == 9)
                        {
                            sprintf(myfile, "%s", buffer[LOWORD(wParam) - ID_RECENTFILES_1]);
                            choose = tmp_choose;
                            if(choose == 0)
                            {
                                for(int i = 0; i < 4; i++)
                                    point[i].x = tmp_array[i][1], point[i].y = tmp_array[i][0];
                                nState = 6;
                            }
                            else
                            {
                                for(int i = 0; i < 3; i++)
                                    point2[i].x = tmp_array[i][1], point2[i].y = tmp_array[i][0];
                                nState_triangle = 4;
                            }
                        }
                        else
                            MessageBox (hWnd, TEXT ("打开失败"), "提示", MB_ICONINFORMATION|MB_OK);
                        fclose(fp);
                    }
                    else
                        MessageBox (hWnd, TEXT ("打开失败"), "提示", MB_ICONINFORMATION|MB_OK);
                    InvalidateRect(hWnd, NULL, TRUE);
                    break;
                }
            }
            break;
         }

        case WM_DESTROY: // 窗口关闭
        {
            for(int i = 0; i < 3; i++) printf("在退出中： %s\n", buffer[i]);
            FILE *FP = fopen("Recently.txt", "w");
            fprintf(FP, "%s\n%s\n%s", buffer[0], buffer[1], buffer[2]);
            fclose(FP);
            PostQuitMessage(0);                    //设置为0表示程序正常结束
            return 0;
        }

    }
    return DefWindowProc(hWnd, message, wParam, lParam);

}

BOOL CALLBACK ModelDlgProc1(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int tmp_point[4][2];
    switch (message)
    {
        case WM_INITDIALOG:
        {
            for(int i = 0; i < 3; i++)
                tmp_point[i][0] = point[i].x, tmp_point[i][1] = point[i].y;
            //GetDlgItem(hDlg, IDC_EDIT1)->SetWindowText(0);
            break;
        }

        case WM_COMMAND:
        {
            switch (LOWORD(wParam))
            {
                case OK_B:
                {
                    BOOL k = TRUE, *test = &k;
                    tmp_point[0][0] = GetDlgItemInt(hDlg, IDC_EDIT1, test, TRUE);
                    tmp_point[1][0] = GetDlgItemInt(hDlg, IDC_EDIT2, test, TRUE);
                    tmp_point[2][0] = GetDlgItemInt(hDlg, IDC_EDIT3, test, TRUE);
                    tmp_point[3][0] = GetDlgItemInt(hDlg, IDC_EDIT4, test, TRUE);
                    tmp_point[0][1] = GetDlgItemInt(hDlg, IDC_EDIT5, test, TRUE);
                    tmp_point[1][1] = GetDlgItemInt(hDlg, IDC_EDIT6, test, TRUE);
                    tmp_point[2][1] = GetDlgItemInt(hDlg, IDC_EDIT7, test, TRUE);
                    tmp_point[3][1] = GetDlgItemInt(hDlg, IDC_EDIT8, test, TRUE);

                    printf("%d\n", k);
                    if(k)
                    {
                        nState = 6;
                        for(int i = 0; i < 4; i++)
                            point[i].x = tmp_point[i][1], point[i].y = tmp_point[i][0];
                        InvalidateRect(GetParent(hDlg), NULL, TRUE);      //表示刷新整个客户区(原来的要清空)
                    }
                    else
                    {
                        MessageBox (hDlg, TEXT ("修改失败！(请输入合法的整数)"), "提示", MB_ICONINFORMATION|MB_OK);
                        return TRUE;
                    }
                    EndDialog(hDlg, 0);
                    return TRUE;
                }

                case CANCEL_B:
                    EndDialog(hDlg, 0);
                    return TRUE;
            }

        }
        break;

        case WM_DESTROY:
        {
            EndDialog(hDlg, 0);
            return TRUE;
        }
    }
    return FALSE;
}

BOOL CALLBACK ModelDlgProc2(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int tmp_point[3][2];
    switch (message)
    {
        case WM_INITDIALOG:
        {
            for(int i = 0; i < 3; i++)
                tmp_point[i][0] = point2[i].x, tmp_point[i][1] = point2[i].y;
            //GetDlgItem(hDlg, IDC_EDIT1)->SetWindowText(0);
            break;
        }

        case WM_COMMAND:
        {
            switch (LOWORD(wParam))
            {
                case IDOK:
                {
                    BOOL k = TRUE, *test = &k;
                    tmp_point[0][0] = GetDlgItemInt(hDlg, IDC_T_EDIT1, test, TRUE);
                    tmp_point[1][0] = GetDlgItemInt(hDlg, IDC_T_EDIT2, test, TRUE);
                    tmp_point[2][0] = GetDlgItemInt(hDlg, IDC_T_EDIT3, test, TRUE);
                    tmp_point[0][1] = GetDlgItemInt(hDlg, IDC_T_EDIT4, test, TRUE);
                    tmp_point[1][1] = GetDlgItemInt(hDlg, IDC_T_EDIT5, test, TRUE);
                    tmp_point[2][1] = GetDlgItemInt(hDlg, IDC_T_EDIT6, test, TRUE);

                    if(k)
                    {
                        nState_triangle = 4;
                        for(int i = 0; i < 3; i++)
                            point2[i].x = tmp_point[i][1], point2[i].y = tmp_point[i][0];
                        InvalidateRect(GetParent(hDlg), NULL, TRUE);      //表示刷新整个客户区(原来的要清空)
                    }
                    else
                    {
                        MessageBox (hDlg, TEXT ("修改失败！(请输入合法的整数)"), "提示", MB_ICONINFORMATION|MB_OK);
                        return TRUE;
                    }
                    EndDialog(hDlg, 0);
                    return TRUE;
                }

                case IDCANCEL:
                    EndDialog(hDlg, 0);
                    return TRUE;
            }

        }
        break;

        case WM_DESTROY:
        {
            EndDialog(hDlg, 0);
            return TRUE;
        }
    }
    return FALSE;
}
