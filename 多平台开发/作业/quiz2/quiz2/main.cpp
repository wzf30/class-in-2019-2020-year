#include <windows.h>
#include <stdio.h>
#define ID_EDIT    1

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);
WNDPROC OldList ;
static HWND hwndEdit, HWnd;
LRESULT CALLBACK ListProc (HWND, UINT, WPARAM, LPARAM) ;
TCHAR szAppName[] = TEXT ("EditDemo") ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
     HWND     hwnd ;
     MSG      msg ;
     WNDCLASS wndclass ;

     wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
     wndclass.lpfnWndProc   = WndProc ;
     wndclass.cbClsExtra    = 0 ;
     wndclass.cbWndExtra    = 0 ;
     wndclass.hInstance     = hInstance ;
     wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
     wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
     wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
     wndclass.lpszMenuName  = NULL ;
     wndclass.lpszClassName = szAppName ;

     if (!RegisterClass (&wndclass))
     {
          MessageBox (NULL, TEXT ("Failed to RegisterClass! "),
                      szAppName, MB_ICONERROR) ;
          return 0 ;
     }

     hwnd = CreateWindow (szAppName, szAppName,
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          NULL, NULL, hInstance, NULL) ;
    HWnd =  hwnd;
     ShowWindow (hwnd, iCmdShow) ;
     UpdateWindow (hwnd) ;

     while (GetMessage (&msg, NULL, 0, 0))
     {
          TranslateMessage (&msg) ;
          DispatchMessage (&msg) ;
     }
     return msg.wParam ;
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

     switch (message)
     {
     case WM_CREATE :
          hwndEdit = CreateWindow (TEXT ("edit"), NULL,                            //创建一个Edit控件
                         WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL |
                                   WS_BORDER | ES_LEFT | ES_MULTILINE |
                                   ES_AUTOHSCROLL | ES_AUTOVSCROLL,
                         0, 0, 0, 0, hwnd, (HMENU) ID_EDIT,
                         ((LPCREATESTRUCT) lParam) -> hInstance, NULL) ;
        OldList = (WNDPROC) SetWindowLong (hwndEdit, GWL_WNDPROC,
                                               (LPARAM) ListProc) ;
          return 0 ;

     case WM_SETFOCUS :
          SetFocus (hwndEdit) ;
          return 0 ;

     case WM_SIZE :
          MoveWindow (hwndEdit, 0, 0, LOWORD (lParam), HIWORD (lParam), TRUE) ;
          return 0 ;

     case WM_COMMAND :
          if (LOWORD (wParam) == ID_EDIT)
               if (HIWORD (wParam) == EN_ERRSPACE ||
                         HIWORD (wParam) == EN_MAXTEXT)

                    MessageBox (hwnd, TEXT ("Edit control out of space."),
                                szAppName, MB_OK | MB_ICONSTOP) ;

          return 0 ;

     case WM_DESTROY :
          PostQuitMessage (0) ;
          return 0 ;
     }
     return DefWindowProc (hwnd, message, wParam, lParam) ;
}

LRESULT CALLBACK ListProc (HWND hwnd, UINT message,
                           WPARAM wParam, LPARAM lParam)
{
    static int choose_Shift = 0, choose_Ctrl = 0;
    CHOOSEFONT cf;
    static LOGFONT lf;        // logical font structure
    static DWORD rgbCurrent;  // current text color
    static HFONT hfont;
    int iSelection;
    static LPTSTR Month[12] = {TEXT("January"), TEXT("February"), TEXT("March"),
                                      TEXT("April"), TEXT("May"), TEXT("June"),
                                      TEXT("July"), TEXT("August"), TEXT("September"),
                                      TEXT("October"), TEXT("November"), TEXT("December") };
    static LPTSTR m;
    switch (message)
    {
        case WM_KEYDOWN:
        {
            switch(LOWORD(wParam))
            {
                case VK_F1:
                case VK_F2:
                case VK_F3:
                case VK_F4:
                case VK_F5:
                case VK_F6:
                case VK_F7:
                case VK_F8:
                case VK_F9:
                case VK_F10:
                case VK_F11:
                case VK_F12:
                {
                    iSelection = LOWORD(wParam) - VK_F1;
                    if(iSelection == 0 && choose_Shift)
                    {
                        MessageBox(hwnd, TEXT ("2020MPADQuiz2(SDK)\nEdit\n\n作者学号:10185102153 姓名:汪子凡"),
                    TEXT("2020MPADlQuiz2"), MB_ICONINFORMATION|MB_OK);
                        break;
                    }
                    if(choose_Ctrl)
                    {
                        printf("%s\n", Month[iSelection]);
                        //GetDlgItemText(hwnd, 0, m, 100);
                        //printf("%s", m);
                        SetDlgItemText(HWnd, ID_EDIT, Month[iSelection]);
                    }
                    break;
                }
                case 17:
                    choose_Ctrl = 1;
                    break;
                case 16:
                    choose_Shift = 1;
                    break;
            }
            break;
        }
        case WM_KEYUP:
        {
            switch(LOWORD(wParam))
                case 17:
                    choose_Ctrl = 0;
                    break;
                case 16:
                    choose_Shift = 0;
                    break;
            break;
        }
        case WM_RBUTTONUP:
        {
        ZeroMemory(&cf, sizeof(cf));
        cf.lStructSize = sizeof (cf);
        cf.hwndOwner = hwnd;
        cf.lpLogFont = &lf;
        cf.rgbColors = rgbCurrent;
        cf.Flags = CF_SCREENFONTS | CF_EFFECTS;

        if (ChooseFont(&cf)==TRUE)
        {
            //hfont = CreateFontIndirect(cf.lpLogFont);
            //rgbCurrent = cf.rgbColors;
            //InvalidateRect(hWnd, NULL, TRUE);
            return TRUE;
        }
        break;
        }
    }
    return CallWindowProc (OldList, hwnd, message, wParam, lParam) ;
}
