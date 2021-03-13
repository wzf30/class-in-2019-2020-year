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
        if((!IsWindow(hDlgModeless) || !IsDialogMessage(hDlgModeless, &msg)) && (!TranslateAccelerator(hWnd, haccel, &msg)))
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
    wc.lpszMenuName     = MAKEINTRESOURCE(IDR_MENU_CH);               //����꽫��Դ��ID��ʾת��Ϊ�ַ�����ʾ������ֱ�����ַ���
    wc.lpszClassName    = TEXT("My1stWClass");  // Name of the window class

    return RegisterClass(&wc);
}

//***************************************************************************************

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)  // ʵ����ʼ��
{
    hWnd = CreateWindow(TEXT("My1stWClass"),     // Name of the window class
                             TEXT("2020MPADLab5: Windows��Դʹ��(2)  ���������ӷ� ѧ�ţ�10185102153"), // Title of the window
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
    static int id_bitmap[3] = {IDB_BITMAP1, IDB_BITMAP2, IDB_BITMAP3}, Bitmap_width, Bitmap_height;
    static int language_choose, cursor_choose, id_text;                    //��¼��Ӧ������ѡ�񣬹��ѡ���Լ���ʱ�ַ��������ʼλ��

    switch (message)
    {
       case WM_CREATE:                            //��ȡӦ�õľ��
         {
             hInst = ((LPCREATESTRUCT)lParam)-> hInstance;
             language_choose = ID_LANGUAGE_CH;
             id_text = IDS_CHINESE1;
             cursor_choose = 0;

             HBITMAP hbitmap = LoadBitmap(hInst, MAKEINTRESOURCE(id_bitmap[0]));
             BITMAP bitmap;
             GetObject(hbitmap, sizeof(BITMAP), &bitmap);          //��λͼ�й���Ϣ����bitmap
             Bitmap_width = bitmap.bmWidth;
             Bitmap_height = bitmap.bmHeight;
             DeleteObject(hbitmap);
             break;
         }

         case WM_PAINT:
         {
            hdc = BeginPaint(hWnd, &ps);
            LoadString(hInst, id_text + cursor_choose, buffer, 60);      //���ַ��������ȡ�ַ���
            TextOut(hdc, 10, 10, buffer, lstrlen(buffer));

            HDC hmemDC = CreateCompatibleDC(hdc);            //�ڴ��豸������
            HBITMAP hbitmap;
            for(int i = 0; i < 3; i++)                   //���μ��������ѡ�͵�ѡλͼ��ЩҪ��ʾ
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
            DeleteDC(hmemDC);                        //�ͷ���Դ
            EndPaint(hWnd, &ps);                    //���������������Ը���ϵͳ��Ҫ�ػ��ĵط��Ѿ��ػ�����getDC���У���һֱ�ظ�����
         }

         case WM_INITMENU:                       //��ʾ��ѡ����
         {
            hMenuMain = (HMENU)wParam;
            CheckMenuRadioItem(hMenuMain, ID_CUSSOR1, ID_CUSSOR3, cursor_choose + ID_CUSSOR1, MF_BYCOMMAND);
            CheckMenuRadioItem(hMenuMain, ID_LANGUAGE_CH, ID_LANGUAGE_EG, language_choose, MF_BYCOMMAND);
         }

         case WM_COMMAND:                                      //����COMMAND��Ϣ
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

                case ID_DIALOG_A:                                                //����ģʽ�Ի���
                    DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1),hWnd, ModelDlgProc);
                    break;
                case ID_DIALOG_B:
                    if(!IsWindow(hDlgModeless))                                  //������ģʽ�Ի���(��ֹ�ظ���)
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
                    dlgfile.lStructSize = sizeof(dlgfile);      //ȷ���ṹ�Ĵ�С
                    dlgfile.hwndOwner = hWnd;                 //ָ�����ĸ����ڣ����ΪNULL����ʾͨ�öԻ���
                    dlgfile.lpstrFile = szFile;                 //���ڱ����ļ�������·�����ļ���
                    dlgfile.lpstrFile[0] = '\0';
                    dlgfile.nMaxFile = sizeof(szFile);         //ָʾ����ṹ�Ĵ�С
                    dlgfile.lpstrFilter = ("All Files(*.*)\0*.*\0Python source(*.py)\0*.py\0C++ Files(*.cpp)\0*.cpp\0\0");
                    dlgfile.nFilterIndex = 1;
                    dlgfile.lpstrFileTitle = NULL;              //���ڱ����ļ���
                    dlgfile.nMaxFileTitle = 0;
                    dlgfile.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
                    dlgfile.lpstrTitle = ("��");
                    if(GetOpenFileName(&dlgfile))
                        MessageBox(hWnd, dlgfile.lpstrFile, "�ļ���", MB_ICONINFORMATION|MB_OK);
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
                    MessageBox (hWnd, TEXT ("2020MPADLab5(SDK)��Դ2\n���,�Ի���λͼ���ַ���\n\n����ѧ��: 10185102153 ����: ���ӷ�"),
                    "2020MPADlab5", MB_ICONINFORMATION|MB_OK);
                    break;
                case ID_FILE_EXIT:
                    PostQuitMessage(0);
                    return 0;
            }
            break;
         }


        case WM_DESTROY: // ���ڹر�
            PostQuitMessage(0);                    //����Ϊ0��ʾ������������
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

            SetFocus(GetDlgItem(hDlg, IDOK1));           //����Ĭ�Ͻ���
            return FALSE;
        }

        case WM_COMMAND:
        {
            switch (LOWORD(wParam))
            {
                case IDOK1:
                    for(int i = 0; i < 3; i++)
                        is_check[i] = (is_check[i] + tmp_check[i]) % 2;
                    InvalidateRect(GetParent(hDlg), NULL, TRUE);      //��ʾˢ�������ͻ���(ԭ����Ҫ���)
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
                        InvalidateRect(GetParent(hDlg), NULL, TRUE);      //��ʾˢ�������ͻ���(ԭ����Ҫ���)
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
