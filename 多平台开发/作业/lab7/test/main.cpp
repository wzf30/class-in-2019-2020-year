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
    HACCEL haccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));           //Load�����ټ�

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
                             TEXT("2020MPADLab7: ����ͼ�� ���������ӷ� ѧ�ţ�10185102153"), // Title of the window
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

    ShowWindow(hWnd, nCmdShow);                                    //����VM_SHOWWINDOW��Ϣ��VM_SIZE��Ϣ
    UpdateWindow(hWnd);                                            //����VM_PAINT��Ϣ

    return TRUE;
}

//***************************************************************************************

// ���ڹ��̺���

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;                         //�豸�����ľ��
    PAINTSTRUCT ps;
    static HINSTANCE hInst;
    static HMENU hMenuMain;

    static LPCTSTR showMesg[8] = {"Beizer���߻��ƣ�",
                                "1.��������������ȷ���˵�P0�������϶����ſ����ȷ���˵�P3",
                                "2.�ٴΰ�������������϶����ſ����ȷ�����Ƶ�P1(ֱ��P0P1����������)",
                                "3.�ٴΰ�������������϶����ſ����ȷ�����Ƶ�P2(ֱ��P2P3����������)",
                                "�����λ��ƣ�",
                                "1.��������������ȷ������P0�������϶����ſ����ȷ���˵�P1",
                                "2.�ٴΰ�������������϶����ſ����ȷ�����Ƶ�P2",
                                "������ɣ��������»��ƻ��߱���"};
    static int choose = 0;
    static BOOL fErase=FALSE;
    int xPos,yPos;

    switch (message)
    {
        case WM_CREATE:                            //��ȡӦ�õľ��
        {
             hInst = ((LPCREATESTRUCT)lParam)-> hInstance;

             FILE *fp = fopen("Recently.txt", "r");                            //�������������ļ�
             fscanf(fp, "%s %s %s", buffer[0], buffer[1], buffer[2]);
             fclose(fp);
             DrawMenuBar(hWnd);
             break;
       }

       case WM_PAINT:
       {
            hdc = BeginPaint(hWnd, &ps);
            if(choose == 0)                 //���ǻ�Beizer����
            {
                TextOut(hdc, 10, 10, showMesg[0], lstrlen(showMesg[0]));
                TextOut(hdc, 10, 30, showMesg[1], lstrlen(showMesg[1]));
                TextOut(hdc, 10, 50, showMesg[2], lstrlen(showMesg[2]));
                TextOut(hdc, 10, 70, showMesg[3], lstrlen(showMesg[3]));
                if(nState == 2)                                             //����ֻ����˵�һ��
                {
                    MoveToEx(hdc,point[0].x,point[0].y,NULL);
                    LineTo(hdc,point[3].x,point[3].y);
                }
                else if(nState > 2)                                           //����ɵ�һ��֮��
                {
                    TextOut(hdc, 10, 100, showMesg[7], lstrlen(showMesg[7]));
                    PolyBezier(hdc,point,4);
                }
            }
            else                             //���ǻ�������
            {
                TextOut(hdc, 10, 10, showMesg[4], lstrlen(showMesg[4]));
                TextOut(hdc, 10, 30, showMesg[5], lstrlen(showMesg[5]));
                TextOut(hdc, 10, 50, showMesg[6], lstrlen(showMesg[6]));
                if(nState_triangle == 2)                                   //����ֻ����˵�һ��
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
            EndPaint(hWnd, &ps);                    //���������������Ը���ϵͳ��Ҫ�ػ��ĵط��Ѿ��ػ�����getDC���У���һֱ�ظ�����

            break;
        }

        case WM_INITMENU:                       //��ʾ��ѡ����
        {
            hMenuMain = (HMENU)wParam;                           //��ʾ�������λ�������
            CheckMenuRadioItem(hMenuMain, ID_CHANGE_BEIZER, ID_CHANGE_TRIANGLE, choose + 40007, MF_BYCOMMAND);

            ModifyMenu(hMenuMain, ID_RECENTFILES_1, MF_BYCOMMAND, ID_RECENTFILES_1, buffer[0]);
            ModifyMenu(hMenuMain, ID_RECENTFILES_2, MF_BYCOMMAND, ID_RECENTFILES_2, buffer[1]);
            ModifyMenu(hMenuMain, ID_RECENTFILES_3, MF_BYCOMMAND, ID_RECENTFILES_3, buffer[2]);
            break;
        }

        case WM_LBUTTONDOWN:
            xPos = (signed short)LOWORD(lParam);   yPos = (signed short)HIWORD(lParam);            //ǿ������ת����֤������ϲ����bug
            SetCapture(hWnd);
            hdc = GetDC(hWnd);
            SetROP2(hdc,R2_NOTXORPEN);
            if(choose == 0)
                switch (nState)
                {
                    case 0:
                        point[0].x = xPos;  point[0].y = yPos;                 //��ȡ�˵�P0
                        nState = 1;
                        break;
                    case 2:
                        MoveToEx(hdc,point[0].x,point[0].y,NULL);            //����ԭ����ֱ��
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
                        nState = 6;                                      //״̬��������nstateΪ6
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
                        nState_triangle = 4;                                  //״̬�������� nState_triangleΪ4
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
                if(choose == 0)                              //bizer����
                {
                    switch (nState)
                    {
                        case 1:
                            if(fErase)                  //��һ�λ��ߵ�ʱ��fErase����0��û���߿��Բ�
                            {
                                MoveToEx(hdc,point[0].x,point[0].y,NULL);
                                LineTo(hdc,point[3].x,point[3].y);
                            }
                            point[3].x=xPos;    point[3].y=yPos;                        //����
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
                            if(fErase)                  //��һ�λ��ߵ�ʱ��fErase����0��û���߿��Բ�
                            {
                                MoveToEx(hdc,point2[0].x,point2[0].y,NULL);
                                LineTo(hdc,point2[1].x,point2[1].y);
                            }
                            point2[1].x = xPos;    point2[1].y = yPos;                        //����
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


         case WM_COMMAND:                                      //����COMMAND��Ϣ
         {
            HMENU hMenuMain = GetMenu(hWnd);
            switch(LOWORD(wParam))
            {
                case ID_CHANGE_BEIZER:
                case ID_CHANGE_TRIANGLE:                      //������������
                {
                    int iSelection = LOWORD(wParam) - 40007;
                    if(iSelection == choose)
                        break;
                    if(MessageBox (hWnd, TEXT("ȷ��Ҫ�޸Ļ�ͼ������"), TEXT("ȷ��"),  MB_YESNO|MB_ICONQUESTION ) == IDYES )   //��Ϣ�����ȷ��
                    {
                        CheckMenuRadioItem(hMenuMain,ID_CHANGE_BEIZER, ID_CHANGE_TRIANGLE, LOWORD(wParam), MF_BYCOMMAND);
                        choose = iSelection;
                        fErase = false;  nState = nState_triangle = 0;
                        InvalidateRect(hWnd, NULL, true);
                    }
                    break;
                }

                case ID_SETTINGS_REPAINT:                        //���»���
                    if(MessageBox (hWnd, TEXT("ȷ��Ҫ���»�����"), TEXT("ȷ��"),  MB_YESNO|MB_ICONQUESTION ) == IDYES )
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
                    MessageBox (hWnd, TEXT ("������ҵ���Ի�����������(����껭������������),ʵ���˻������ļ������½��򿪹���(��txt��ʽ)"),
                    "2020MPADlab7", MB_ICONINFORMATION|MB_OK);
                    break;

                case ID_HELP_ABOUT:
                    MessageBox (hWnd, TEXT ("2020MPADLab7 ��ͼ\n\n����ѧ��: 10185102153 ����: ���ӷ�"),
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
                    dlgfile.lStructSize = sizeof(dlgfile);      //ȷ���ṹ�Ĵ�С
                    dlgfile.hwndOwner = hWnd;                 //ָ�����ĸ����ڣ����ΪNULL����ʾͨ�öԻ���
                    dlgfile.lpstrFile = szFile;                 //���ڱ����ļ�������·�����ļ���
                    dlgfile.lpstrFile[0] = '\0';
                    dlgfile.nMaxFile = sizeof(szFile);         //ָʾ����ṹ�Ĵ�С
                    dlgfile.lpstrFilter = ("txt source(*.txt)\0*.txt\0");
                    dlgfile.nFilterIndex = 1;
                    dlgfile.lpstrFileTitle = NULL;              //���ڱ����ļ���
                    dlgfile.nMaxFileTitle = 0;
                    dlgfile.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

                    int tmp_array[4][2], tmp_choose;
                    if(LOWORD(wParam) == ID_FILE_OPEN)
                    {
                        dlgfile.lpstrTitle = ("��");
                        if(GetOpenFileName(&dlgfile))
                        {
                            FILE *fp = fopen(dlgfile.lpstrFile, "r");               //���ļ����ȶ�ȡ�ļ���Ϣ
                            if(fscanf(fp, "%d %d %d %d %d %d %d %d %d", &tmp_choose, &tmp_array[0][0], &tmp_array[0][1], &tmp_array[1][0],
                            &tmp_array[1][1], &tmp_array[2][0], &tmp_array[2][1], &tmp_array[3][0], &tmp_array[3][1]) == 9)
                            {
                                sprintf(myfile, "%s", dlgfile.lpstrFile);                //���ĵ�ǰ��myfile
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
                            else                                              //�����ļ�����Ϣ��ȡ��ʽ����ȷ
                                MessageBox (hWnd, TEXT ("��ʧ��"), "��ʾ", MB_ICONINFORMATION|MB_OK);
                            fclose(fp);
                        }
                        else                                                 //�����ļ�û�д�
                             MessageBox (hWnd, TEXT ("��ʧ��"), "��ʾ", MB_ICONINFORMATION|MB_OK);
                    }

                    else if(LOWORD(wParam) == ID_FILE_NEW)
                    {
                        dlgfile.lpstrTitle = ("�½�");
                        if(GetSaveFileName(&dlgfile))
                        {
                            sprintf(myfile, "%s", dlgfile.lpstrFile);
                            char b[]=".txt", *pa = myfile;            //�����ļ�ʱ��Ҫ����.txt��׺
                            while(*pa) pa++;
                            sprintf(pa, "%s", b);
                            printf("%s", myfile);
                            FILE *fp = fopen(myfile, "w");
                            fclose(fp);
                            choose = 0;  nState = 0;  fErase = FALSE;           //Ĭ���½����ļ�Ϊbizer����
                        }
                    }

                    else if(LOWORD(wParam) == ID_FILE_SAVE && myfile[0] != '\0')     //���Ǳ����ļ�
                    {
                        if(nState < 6 && nState_triangle < 4)
                        {
                            MessageBox (hWnd, TEXT ("����ʧ�ܣ�ͼ��δ�������"), "��ʾ", MB_ICONINFORMATION|MB_OK);
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
                            char b[]=".txt", *pa = myfile;                //Ҫ�Ӻ�׺
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
                            MessageBox (hWnd, TEXT ("��ʧ��"), "��ʾ", MB_ICONINFORMATION|MB_OK);
                        fclose(fp);
                    }
                    else
                        MessageBox (hWnd, TEXT ("��ʧ��"), "��ʾ", MB_ICONINFORMATION|MB_OK);
                    InvalidateRect(hWnd, NULL, TRUE);
                    break;
                }
            }
            break;
         }

        case WM_DESTROY: // ���ڹر�
        {
            for(int i = 0; i < 3; i++) printf("���˳��У� %s\n", buffer[i]);
            FILE *FP = fopen("Recently.txt", "w");
            fprintf(FP, "%s\n%s\n%s", buffer[0], buffer[1], buffer[2]);
            fclose(FP);
            PostQuitMessage(0);                    //����Ϊ0��ʾ������������
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
                        InvalidateRect(GetParent(hDlg), NULL, TRUE);      //��ʾˢ�������ͻ���(ԭ����Ҫ���)
                    }
                    else
                    {
                        MessageBox (hDlg, TEXT ("�޸�ʧ�ܣ�(������Ϸ�������)"), "��ʾ", MB_ICONINFORMATION|MB_OK);
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
                        InvalidateRect(GetParent(hDlg), NULL, TRUE);      //��ʾˢ�������ͻ���(ԭ����Ҫ���)
                    }
                    else
                    {
                        MessageBox (hDlg, TEXT ("�޸�ʧ�ܣ�(������Ϸ�������)"), "��ʾ", MB_ICONINFORMATION|MB_OK);
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
