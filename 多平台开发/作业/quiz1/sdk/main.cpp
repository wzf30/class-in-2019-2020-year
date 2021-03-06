#include <windows.h>
#include "resource.h"
#include <stdio.h>

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

HINSTANCE hInst ;
TCHAR     szAppName[] = TEXT ("PopMenu") ;

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
     wndclass.hIcon         = LoadIcon (NULL, szAppName) ;
     wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
     wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
     wndclass.lpszMenuName  = MAKEINTRESOURCE(POPMENU) ;
     wndclass.lpszClassName = szAppName ;

     if (!RegisterClass (&wndclass))
     {
          MessageBox (NULL, TEXT ("Failed to RegisterClass! "),
                      szAppName, MB_ICONERROR) ;
          return 0 ;
     }

     hInst = hInstance ;

     hwnd = CreateWindow (szAppName, TEXT ("Popup Menu Demonstration"),
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          NULL, NULL, hInstance, NULL) ;

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
     static HMENU hMenu ;
     static int   idColor [5] = { WHITE_BRUSH,  LTGRAY_BRUSH, GRAY_BRUSH,
                                  DKGRAY_BRUSH, BLACK_BRUSH } ;
     static int   iSelection = IDM_BKGND_WHITE;
     static RECT rec;
     POINT        point, point_client;

     switch (message)
     {
     case WM_CREATE:
          hMenu = LoadMenu (hInst, MAKEINTRESOURCE(POPMENU) ) ;
          hMenu = GetSubMenu (hMenu, 0) ;
          GetClientRect(hwnd, &rec);
          CheckMenuRadioItem(hMenu, IDM_BKGND_WHITE, IDM_BKGND_BLACK, iSelection, MF_BYCOMMAND);
          return 0 ;

     case WM_RBUTTONUP:
          point.x = point_client.x = LOWORD (lParam) ;
          point.y = point_client.y = HIWORD (lParam) ;
          ClientToScreen (hwnd, &point) ;
          if( point_client.x * 2 < rec.right && point_client.y * 2 < rec.bottom)
          {
              HMENU tmp =  GetSubMenu(hMenu, 0);
              TrackPopupMenu (tmp, TPM_RIGHTBUTTON, point.x, point.y, 0, hwnd, NULL) ;
          }
          else if(point_client.x * 2 > rec.right && point_client.y * 2 < rec.bottom)
          {
              HMENU tmp =  GetSubMenu(hMenu, 1);
              TrackPopupMenu (tmp, TPM_RIGHTBUTTON, point.x, point.y, 0, hwnd, NULL) ;
          }
          else if(point_client.x * 2 < rec.right)
          {
              HMENU tmp =  GetSubMenu(hMenu, 2);
              TrackPopupMenu (tmp, TPM_RIGHTBUTTON, point.x, point.y, 0, hwnd, NULL) ;
          }
          else
          {
              HMENU tmp =  GetSubMenu(hMenu, 3);
              TrackPopupMenu (tmp, TPM_RIGHTBUTTON, point.x, point.y, 0, hwnd, NULL) ;
          }
          return 0 ;

     case WM_INITMENU:                       //????????????
     {
            CheckMenuRadioItem((HMENU)wParam, IDM_BKGND_WHITE, IDM_BKGND_BLACK, iSelection, MF_BYCOMMAND);
     }


     case WM_COMMAND:
          switch (LOWORD (wParam))
          {
          case IDM_FILE_NEW:
          case IDM_FILE_OPEN:
          case IDM_FILE_SAVE:
          case IDM_FILE_SAVE_AS:
          case IDM_EDIT_UNDO:
          case IDM_EDIT_CUT:
          case IDM_EDIT_COPY:
          case IDM_EDIT_PASTE:
          case IDM_EDIT_CLEAR:
               MessageBeep (0) ;
               return 0 ;

          case IDM_BKGND_WHITE:         // Note: Logic below
          case IDM_BKGND_LTGRAY:        //   assumes that IDM_WHITE
          case IDM_BKGND_GRAY:          //   through IDM_BLACK are
          case IDM_BKGND_DKGRAY:        //   consecutive numbers in
          case IDM_BKGND_BLACK:         //   the order shown here.
         {

               //CheckMenuItem (hMenu, iSelection, MF_UNCHECKED) ;
               iSelection = LOWORD (wParam) ;
               //CheckMenuItem (hMenu, iSelection, MF_CHECKED) ;
                CheckMenuRadioItem(hMenu, IDM_BKGND_WHITE, IDM_BKGND_BLACK, iSelection, MF_BYCOMMAND);
               SetClassLong (hwnd, GCL_HBRBACKGROUND, (LONG)
                    GetStockObject
                         (idColor [LOWORD (wParam) - IDM_BKGND_WHITE])) ;

               InvalidateRect (hwnd, NULL, TRUE) ;
               DrawMenuBar(hwnd);
               return 0 ;
         }
          case IDM_APP_ABOUT:
               MessageBox (hwnd, TEXT ("Popup Menu Demonstration Program\n")
                                 TEXT (" "),
                           szAppName, MB_ICONINFORMATION | MB_OK) ;
               return 0 ;

          case IDM_APP_EXIT:
               SendMessage (hwnd, WM_CLOSE, 0, 0) ;
               return 0 ;

          case IDM_APP_HELP:
               MessageBox (hwnd, TEXT ("2020MPADQuiz1(SDK)PopMenu\n\n??????????10185102153 ????????????")
                                 TEXT (""),
                           szAppName, MB_ICONINFORMATION | MB_OK) ;
          }
          break ;
     case WM_DESTROY:
          PostQuitMessage (0) ;
          return 0 ;
     }
     return DefWindowProc (hwnd, message, wParam, lParam) ;
}
