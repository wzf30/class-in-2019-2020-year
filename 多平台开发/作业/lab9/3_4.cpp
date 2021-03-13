#include<windows.h>
#include<stdlib.h>
#include<string.h>
long WINAPI WndProc(HWND hWnd,UINT iMessage,UINT wParam,LONG lParam);
BOOL InitWindowsClass(HINSTANCE hInstance);
BOOL InitWindows(HINSTANCE hInstance,int nCmdShow);

BOOL InitWindowsClass(HINSTANCE hInstance)//��ʼ��������
{
		WNDCLASS WndClass;
		WndClass.cbClsExtra=0;
		WndClass.cbWndExtra=0;
		WndClass.hbrBackground=(HBRUSH)(GetStockObject(WHITE_BRUSH));
		WndClass.hCursor=LoadCursor(NULL,IDC_ARROW);
		WndClass.hIcon=LoadIcon(NULL,"END");
		WndClass.hInstance=hInstance;
		WndClass.lpfnWndProc=WndProc;
		WndClass.lpszClassName="WinText";
		WndClass.lpszMenuName=NULL;
		WndClass.style=CS_HREDRAW|CS_VREDRAW;
		return RegisterClass(&WndClass);
}

BOOL InitWindows(HINSTANCE hInstance,int nCmdShow) //��ʼ������
{
	HWND hWnd;
		hWnd=CreateWindow("WinText",  //���ɴ���
						"�ı���ʾʾ������",
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
	while(GetMessage(&Message,0,0,0))		//��Ϣѭ��
		{TranslateMessage(&Message);
		 DispatchMessage(&Message);	
        }
	return Message.wParam;
}

long WINAPI WndProc(HWND hWnd,UINT iMessage,UINT wParam,LONG lParam)
{ static long nXChar,nCaps,nYChar;
  int pointx,pointy,i,j;
  HDC hDC;   					//����ָ���豸�����ľ��	
  TEXTMETRIC tm;				//�������������ԵĽṹ�����
  PAINTSTRUCT PtStr; 			//ָ�������ͼ��Ϣ�Ľṹ�����
  char *textbuf[4]={{"�������ǻƺ�¥"}, {"�̻�����������"},{"�·�ԶӰ�̿վ�"}, {"Ψ�����������"}};
  switch(iMessage)  					//������Ϣ
  {	case WM_CREATE:						//�����ڴ�����Ϣ
	 hDC=GetDC(hWnd) ;   				//��ȡ��ǰ�豸����
	 GetTextMetrics(hDC,&tm); 			//��ȡ������Ϣ
	 nXChar=tm.tmAveCharWidth;  			//��ȡ�ַ����
     nYChar=tm.tmHeight+tm.tmExternalLeading;	//�ַ��߶�
	 nCaps=(tm.tmPitchAndFamily&1?3:2)*nXChar/2;	//�ּ��
	 ReleaseDC(hWnd,hDC); return 0;		//�ͷŵ�ǰ�豸���
	case WM_PAINT: 						//�����ػ���Ϣ
	hDC=BeginPaint(hWnd,&PtStr); 		//��ʼ��ͼ
	 for(i=4;i>0;i--)  
	 {for(j=0;j<7;j++)					//����ı�
        {  pointx=100+i*nXChar*5;	pointy=50+j*(nYChar+nCaps);
	       TextOut(hDC,pointx,pointy,textbuf[4-i]+j*2,2); 
	    } 
	 }
     EndPaint(hWnd,&PtStr); return 0; 		//������ͼ
	case WM_DESTROY: //����Ӧ�ó���
	 PostQuitMessage(0);	return 0;
	default:	
		return(DefWindowProc(hWnd,iMessage,wParam,lParam));
  }
}
