#include <windows.h>
#include <stdio.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,UINT wParam,LONG lParam);
BOOL InitWindowsClass(HINSTANCE hInstance);
BOOL InitWindows(HINSTANCE hInstance,int nCmdShow);
HWND hWnd;
BOOL InitWindowsClass(HINSTANCE hInstance)//初始化窗口类
{
		WNDCLASS WndClass;
		WndClass.cbClsExtra=0;
		WndClass.cbWndExtra=0;
		WndClass.hbrBackground=(HBRUSH)(GetStockObject(WHITE_BRUSH));
		WndClass.hCursor=LoadCursor(NULL,IDC_ARROW);
		 WndClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
		WndClass.hInstance=hInstance;
		WndClass.lpfnWndProc=WndProc;
		WndClass.lpszClassName= TEXT("WinText");
		WndClass.lpszMenuName=MAKEINTRESOURCE(IDR_MENU1); ;
		WndClass.style=CS_HREDRAW|CS_VREDRAW;
		return RegisterClass(&WndClass);
}

BOOL InitWindows(HINSTANCE hInstance,int nCmdShow) //初始化窗口
{
		hWnd=CreateWindow(TEXT("WinText"),  //生成窗口
						TEXT("2020MPADlab9(SDK) 汪子凡"),
						WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT,
						0,
						CW_USEDEFAULT,
						0,
						NULL,
						NULL,
						hInstance,
						NULL);
		if(!hWnd)
			return FALSE;
		ShowWindow(hWnd,nCmdShow);//显示窗口
		UpdateWindow(hWnd);
		return TRUE;
}
//主函数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow)
{
	MSG Message;
	if(!InitWindowsClass(hInstance))		return FALSE;
	if(!InitWindows(hInstance,nCmdShow))	return FALSE;

    HACCEL haccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));

	while(GetMessage(&Message,0,0,0))		//消息循环
		{
		     if(!TranslateAccelerator(hWnd, haccel, &Message))
		    TranslateMessage(&Message);
		 DispatchMessage(&Message);
        }
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,UINT wParam,LONG lParam)
{ static long nXChar,nCaps,nYChar;
  int pointx,pointy,i,j, Len;
  HDC hDC;   					//定义指向设备环境的句柄
  TEXTMETRIC tm;				//存放字体各种属性的结构体变量
  PAINTSTRUCT PtStr; 			//指向包含绘图信息的结构体变量
  static LPCTSTR textbuf[4]={TEXT("故人西辞黄鹤楼"), TEXT("烟花三月下扬州"),TEXT("孤帆远影碧空尽"), TEXT("唯见长江天际流")};
    CHOOSEFONT cf;            // common dialog box structure
    static LOGFONT lf;        // logical font structure
    static DWORD rgbCurrent;  // current text color
    static HFONT hfont;

  switch(iMessage)  					//处理消息
  {	case WM_CREATE:						//处理窗口创建消息
	 hDC=GetDC(hWnd) ;   				//获取当前设备表句柄
	 //GetTextMetrics(hDC,&tm); 			//获取字体信息
	 //nXChar=tm.tmAveCharWidth;  			//获取字符宽度
     //nYChar=tm.tmHeight+tm.tmExternalLeading;	//字符高度
	 //nCaps=(tm.tmPitchAndFamily&1?3:2)*nXChar/2;	//字间距

	 ReleaseDC(hWnd,hDC); return 0;		//释放当前设备句柄

	case WM_PAINT: 						//处理重画消息
	hDC=BeginPaint(hWnd,&PtStr); 		//开始绘图
    SelectObject(hDC, hfont);
    SetTextColor(hDC,rgbCurrent);
    GetTextMetrics(hDC,&tm); 			//重新获取字体信息
	 nXChar=tm.tmAveCharWidth;  			//获取字符宽度
     nYChar=tm.tmHeight+tm.tmExternalLeading;	//字符高度
	 nCaps=(tm.tmPitchAndFamily&1?3:2)*nXChar/2;	//字间距
     if(lstrlen(textbuf[0]) == 7) Len = 1;          //ANSI
     else Len = 2;                              //UNICODE
     //printf("%d %d\n", lstrlen(textbuf[0]), Len);
	 for(i=4;i>0;i--)
	 {for(j=0;j<7;j++)					//输出文本
        {  pointx=100+i*nXChar*5;	pointy=50+j*(nYChar+nCaps);
	       TextOut(hDC,pointx,pointy,textbuf[4-i]+j*Len, Len);
	    }
	 }
     EndPaint(hWnd,&PtStr); return 0; 		//结束绘图
    case WM_RBUTTONUP:
    {
        ZeroMemory(&cf, sizeof(cf));
        cf.lStructSize = sizeof (cf);
        cf.hwndOwner = hWnd;
        cf.lpLogFont = &lf;
        cf.rgbColors = rgbCurrent;
        cf.Flags = CF_SCREENFONTS | CF_EFFECTS;

        if (ChooseFont(&cf)==TRUE)
        {
            hfont = CreateFontIndirect(cf.lpLogFont);
            rgbCurrent = cf.rgbColors;
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;
    }
    case WM_COMMAND:
        switch(LOWORD(wParam))
        {
            case IDC_EXIT:
                PostQuitMessage(0);	return TRUE;
            case IDC_INFORMATION:
                MessageBox(hWnd, TEXT ("2020MPADlab9(SDK)\n文字输出\n\n作者学号:10185102153 姓名:汪子凡"),
                    TEXT("2020MPADlab9"), MB_ICONINFORMATION|MB_OK);
                return TRUE;
        }
        break;
	case WM_DESTROY: //结束应用程序
	 PostQuitMessage(0);	return TRUE;
	default:
		return(DefWindowProc(hWnd,iMessage,wParam,lParam));
  }
  return FALSE;
}
