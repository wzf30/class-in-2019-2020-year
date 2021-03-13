#include <windows.h>
#include <stdio.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,UINT wParam,LONG lParam);
BOOL InitWindowsClass(HINSTANCE hInstance);
BOOL InitWindows(HINSTANCE hInstance,int nCmdShow);
HWND hWnd;
BOOL InitWindowsClass(HINSTANCE hInstance)//��ʼ��������
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

BOOL InitWindows(HINSTANCE hInstance,int nCmdShow) //��ʼ������
{
		hWnd=CreateWindow(TEXT("WinText"),  //���ɴ���
						TEXT("2020MPADlab9(SDK) ���ӷ�"),
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
		ShowWindow(hWnd,nCmdShow);//��ʾ����
		UpdateWindow(hWnd);
		return TRUE;
}
//������
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow)
{
	MSG Message;
	if(!InitWindowsClass(hInstance))		return FALSE;
	if(!InitWindows(hInstance,nCmdShow))	return FALSE;

    HACCEL haccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));

	while(GetMessage(&Message,0,0,0))		//��Ϣѭ��
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
  HDC hDC;   					//����ָ���豸�����ľ��
  TEXTMETRIC tm;				//�������������ԵĽṹ�����
  PAINTSTRUCT PtStr; 			//ָ�������ͼ��Ϣ�Ľṹ�����
  static LPCTSTR textbuf[4]={TEXT("�������ǻƺ�¥"), TEXT("�̻�����������"),TEXT("�·�ԶӰ�̿վ�"), TEXT("Ψ�����������")};
    CHOOSEFONT cf;            // common dialog box structure
    static LOGFONT lf;        // logical font structure
    static DWORD rgbCurrent;  // current text color
    static HFONT hfont;

  switch(iMessage)  					//������Ϣ
  {	case WM_CREATE:						//�����ڴ�����Ϣ
	 hDC=GetDC(hWnd) ;   				//��ȡ��ǰ�豸����
	 //GetTextMetrics(hDC,&tm); 			//��ȡ������Ϣ
	 //nXChar=tm.tmAveCharWidth;  			//��ȡ�ַ����
     //nYChar=tm.tmHeight+tm.tmExternalLeading;	//�ַ��߶�
	 //nCaps=(tm.tmPitchAndFamily&1?3:2)*nXChar/2;	//�ּ��

	 ReleaseDC(hWnd,hDC); return 0;		//�ͷŵ�ǰ�豸���

	case WM_PAINT: 						//�����ػ���Ϣ
	hDC=BeginPaint(hWnd,&PtStr); 		//��ʼ��ͼ
    SelectObject(hDC, hfont);
    SetTextColor(hDC,rgbCurrent);
    GetTextMetrics(hDC,&tm); 			//���»�ȡ������Ϣ
	 nXChar=tm.tmAveCharWidth;  			//��ȡ�ַ����
     nYChar=tm.tmHeight+tm.tmExternalLeading;	//�ַ��߶�
	 nCaps=(tm.tmPitchAndFamily&1?3:2)*nXChar/2;	//�ּ��
     if(lstrlen(textbuf[0]) == 7) Len = 1;          //ANSI
     else Len = 2;                              //UNICODE
     //printf("%d %d\n", lstrlen(textbuf[0]), Len);
	 for(i=4;i>0;i--)
	 {for(j=0;j<7;j++)					//����ı�
        {  pointx=100+i*nXChar*5;	pointy=50+j*(nYChar+nCaps);
	       TextOut(hDC,pointx,pointy,textbuf[4-i]+j*Len, Len);
	    }
	 }
     EndPaint(hWnd,&PtStr); return 0; 		//������ͼ
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
                MessageBox(hWnd, TEXT ("2020MPADlab9(SDK)\n�������\n\n����ѧ��:10185102153 ����:���ӷ�"),
                    TEXT("2020MPADlab9"), MB_ICONINFORMATION|MB_OK);
                return TRUE;
        }
        break;
	case WM_DESTROY: //����Ӧ�ó���
	 PostQuitMessage(0);	return TRUE;
	default:
		return(DefWindowProc(hWnd,iMessage,wParam,lParam));
  }
  return FALSE;
}
