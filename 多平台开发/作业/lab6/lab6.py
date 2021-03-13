# -*- coding: utf8 -*-

import wx
import os
from String import StringTable                                 #字符串资源

class ModalDialog(wx.Dialog):                                  #定义一个模式对话框类
    def __init__(self, parent):
        wx.Dialog.__init__(self, parent, -1, u"复选对话框",pos=(500,300),size=(500, 300))            #调用基类的构造函数
        panel = wx.Panel(self)                                                                       #创建画板
        self.okButton = wx.Button(panel, wx.ID_OK, u"确定", pos=(30, 200),size=(130,30))             #创建button控件
        self.okButton.SetDefault()                                                                   #设置输入焦点                                          
        self.cancelButton = wx.Button(panel, wx.ID_CANCEL, u"取消",pos=(250, 200),size=(130,30))
        self.check = []
        self.check.append(wx.CheckBox(panel,500,u"复选位图1",pos=(30,50),size=(130,40)))             #创建CheckBox控件(用来复选)
        self.check.append(wx.CheckBox(panel,501,u"复选位图2",pos=(30,100),size=(130,40)))
        self.check.append(wx.CheckBox(panel,502,u"复选位图3",pos=(30,150),size=(130,40)))
        self.Bind(wx.EVT_CHECKBOX, self.OnCheckBoxs, id=500,id2=502)                                  #绑定，注意控件所对应的函数
        self.Bind(wx.EVT_BUTTON, self.OnOk, self.okButton)
        self.Bind(wx.EVT_BUTTON, self.OnCancel, self.cancelButton)
        
        self.tmp = self.GetParent().choose_bitmap1[:]                                                 #对复选按钮按照原来状态初始化
        for i in range(3):
            if(self.tmp[i] == 1):
                self.check[i].SetValue(True)

    def OnCheckBoxs(self, evt):
            iSelection = evt.GetId() - 500
            self.tmp[iSelection] ^= 1

    def OnCancel(self, evt):                                                                      #cancel的话之前改变是无效的                                                           
            self.Destroy()

    def OnOk(self, evt):
            for i in range(3):
                self.GetParent().choose_bitmap1[i] = self.tmp[i]
            self.GetParent().Refresh()                                                            #刷新客户区
            self.Destroy()


class ModalessDialog(wx.Dialog):                          #定义一个无模式对话框类
    def __init__(self,parent):
        wx.Dialog.__init__(self, parent, -1, u"单选对话框",size=(400, 300),pos=(500,300))      #调用基类的构造函数
        panel = wx.Panel(self)                                                                 #创建画板
        self.okButton = wx.Button(panel, wx.ID_OK, u"确认", pos=(30, 200),size=(130,40))       
        self.okButton.SetDefault()
        self.cancelButton = wx.Button(panel, wx.ID_CANCEL, u"取消",pos=(250, 200),size=(130,40))
        self.Bind(wx.EVT_BUTTON, self.OnOK, self.okButton)
        self.Bind(wx.EVT_BUTTON, self.OnCancel, self.cancelButton)
        self.radio = []
        self.radio.append(wx.RadioButton(panel, 600, u"单选位图1",pos=(30,50),size=(130,40)))         #创建RadioButton控件(用来单选)
        self.radio.append(wx.RadioButton(panel, 601, u"单选位图2",pos=(30,100),size=(130,40)))
        self.radio.append(wx.RadioButton(panel, 602, u"单选位图3",pos=(30,150),size=(130,40)))
        self.Bind(wx.EVT_RADIOBUTTON, self.OnRadio, id=600, id2= 602)                                 #绑定，注意控件所对应的函数

        self.tmp = self.GetParent().choose_bitmap2                                                  #对单选按钮按照原来状态初始化
        self.radio[self.tmp].SetValue(True)

    def OnRadio(self, evt):
            iSelection = evt.GetId() - 600
            self.tmp = iSelection

    def OnCancel(self, evt):                                                              #cancel的话之前改变是无效的         
            self.GetParent().choose_open = 0
            self.Destroy()

    def OnOK(self, evt):
            self.GetParent().choose_open = 0
            self.GetParent().choose_bitmap2 = self.tmp
            self.GetParent().Refresh()
            self.Destroy()
        



class MyFrame(wx.Frame):
    def __init__(self):
        wx.Frame.__init__(self, None, -1, u"2020MPADLab6: Windows资源使用(2)  姓名：汪子凡 学号：10185102153", size = (1000, 700))      #调用基类的构造函数

        self.choose_language = 0           #0表示中文，1表示英文
        self.choose_cursor = 0             #0表示箭头，1表示十字，2表示自定义
        self.choose_bitmap1 = [0, 0, 0]    #0表示不选，1表示选
        self.choose_bitmap2 = 0
        self.choose_open = 0               #0表示无模式对话框未打开，1表示已经打开，避免重复打开

        self.Bind(wx.EVT_PAINT, self.OnPaint)                       #将paint事件与函数OnPaint绑定

        #创建一个中文菜单栏
        self.CH_menuBar = wx.MenuBar()           
        #接下来创建四个菜单标题(popmenu)，先用wx.Menu()创建，然后用Append方法添加菜单项，并将每个菜单项的选中与一个操作进行bind，最后将该菜单标题添加到菜单栏中
        CH_Pop_File = wx.Menu()
        CH_Pop_File.Append(wx.ID_EXIT, u"&Exit\tCtrl+Shift+Delete", u"退出程序")   #\t后面直接定义了加速键
        self.Bind(wx.EVT_MENU, self.OnClose, id = wx.ID_EXIT)               
        self.CH_menuBar.Append(CH_Pop_File, u"&File")                     #将该菜单标题添加到菜单栏
    
        CH_Pop_Dialog = wx.Menu()
        CH_Pop_Dialog.Append(100, u"模式对话框(&M)...\tCtrl+A", u"打开模式对话框")      
        CH_Pop_Dialog.Append(101, u"无模式对话框(&L)...\tCtrl+B", u"打开无模式对话框")
        CH_Pop_Dialog.Append(102, u"文件对话框(&F)...\tCtrl+C", u"打开通用对话框")
        self.Bind(wx.EVT_MENU_RANGE,self.OnDialog,id=100,id2=102)                       #注意这个绑定方法
        self.CH_menuBar.Append(CH_Pop_Dialog, u"对话框(&I)")                   

        CH_Pop_Cursor = wx.Menu()
        CH_Pop_Cursor.Append(200, u"光标1(箭头)\tCtrl+1", u"使用箭头光标", wx.ITEM_RADIO)        #注意最后一个参数，表示这三个光标是单选
        CH_Pop_Cursor.Append(201, u"光标2(十字)\tCtrl+2", u"使用十字光标", wx.ITEM_RADIO)
        CH_Pop_Cursor.Append(202, u"光标3(自定义)\tCtrl+3", u"使用自定义光标", wx.ITEM_RADIO)
        self.Bind(wx.EVT_MENU_RANGE,self.CursorChange,id=200,id2=202)
        self.CH_menuBar.Append(CH_Pop_Cursor, u"光标类型(&C)")

        CH_Pop_Language = wx.Menu()
        CH_Pop_Language.Append(300, u"中(Chinses)\tCtrl+Shift+C", u"切换至中文", wx.ITEM_RADIO)        #注意最后一个参数，表示这两个语言是单选
        CH_Pop_Language.Append(301, u"英(English)\tCtrl+Shift+D", u"切换至英文", wx.ITEM_RADIO)
        self.Bind(wx.EVT_MENU_RANGE,self.ChangeLanguage,id=300,id2=301)
        self.CH_menuBar.Append(CH_Pop_Language, u"语言(&L)")

        CH_Pop_About = wx.Menu()
        CH_Pop_About.Append(400, u"程序信息(&I)\tF1")
        self.Bind(wx.EVT_MENU, self.OnMessage, id = 400)
        self.CH_menuBar.Append(CH_Pop_About, u"关于(&A)") 

        self.SetMenuBar(self.CH_menuBar)


        #创建一个英文菜单栏
        self.EG_menuBar = wx.MenuBar()           
        #接下来创建四个菜单标题(popmenu)，先用wx.Menu()创建，然后用Append方法添加菜单项，并将每个菜单项的选中与一个操作进行bind，最后将该菜单标题添加到菜单栏中
        EG_Pop_File = wx.Menu()
        EG_Pop_File.Append(wx.ID_EXIT, u"&Exit", u"退出程序")   #\t后面直接定义了加速键           
        self.EG_menuBar.Append(EG_Pop_File, u"&File")                     #将该菜单标题添加到菜单栏
    
        EG_Pop_Dialog = wx.Menu()
        EG_Pop_Dialog.Append(100, u"Modal Dialog...\tCtrl+A", u"打开模式对话框")      
        EG_Pop_Dialog.Append(101, u"Modaless Dialog...\tCtrl+B", u"打开无模式对话框")
        EG_Pop_Dialog.Append(102, u"File Dialog...\tCtrl+C", u"打开通用对话框")
        #self.Bind(wx.EVT_MENU_RANGE,self.OnDialog,id=100,id2=102)                      #由于中英文菜单对应的菜单项赋予了同一个ID，所以不需要重复绑定
        self.EG_menuBar.Append(EG_Pop_Dialog, u"&Dialog")                   

        EG_Pop_Cursor = wx.Menu()
        EG_Pop_Cursor.Append(200, u"Cursor1(ARROW)\tCtrl+1", u"使用箭头光标", wx.ITEM_RADIO)       
        EG_Pop_Cursor.Append(201, u"Cursor2(CROSS)\tCtrl+2", u"使用十字光标", wx.ITEM_RADIO)
        EG_Pop_Cursor.Append(202, u"Cursor3(UserDefined)\tCtrl+3", u"使用自定义光标", wx.ITEM_RADIO)
        self.EG_menuBar.Append(EG_Pop_Cursor, u"&Cursor")

        EG_Pop_Language = wx.Menu()
        EG_Pop_Language.Append(300, u"Chinese(中)\tCtrl+Shift+C", u"切换至中文", wx.ITEM_RADIO)        
        EG_Pop_Language.Append(301, u"English(英)\tCtrl+Shift+D", u"切换至英文", wx.ITEM_RADIO)
        self.EG_menuBar.Append(EG_Pop_Language, u"Language")

        EG_Pop_About = wx.Menu()
        EG_Pop_About.Append(400, u"Program Information\tF1")
        self.EG_menuBar.Append(EG_Pop_About, u"&About")

        #创建位图资源
        self.bmpList = []
        self.bmpList.append(wx.Image(name = "bitmap1.bmp", type = wx.BITMAP_TYPE_BMP).ConvertToBitmap())
        self.bmpList.append(wx.Image(name = "bitmap2.bmp", type = wx.BITMAP_TYPE_BMP).ConvertToBitmap())
        self.bmpList.append(wx.Image(name = "bitmap3.bmp", type = wx.BITMAP_TYPE_BMP).ConvertToBitmap())

        icon = wx.Icon(name = "icon1.ico", type = wx.BITMAP_TYPE_ICO)                        #创建icon资源
        self.SetIcon(icon)

    
    def OnPaint(self,evt):
        dc = wx.PaintDC(self)
        for i in range(3):
            if(self.choose_bitmap1[i] == 1):
                dc.DrawBitmap(self.bmpList[i] , 400, 200 * i + 50)               #省略了MemDC的简便方式加载位图资源             
        dc.DrawBitmap(self.bmpList[self.choose_bitmap2] , 600, 50)
        dc.SetFont(wx.Font(15, wx.SWISS, wx.NORMAL, wx.BOLD, False))
        dc.DrawText(StringTable[self.choose_language][self.choose_cursor], 25,50)           #显示光标信息

    def OnClose(self,evt):
            self.Close()

    def CursorChange(self, evt):
        iSelection = evt.GetId() - 200
        if(iSelection == self.choose_cursor):                                         #若是光标不用切换
            return
        if(iSelection == 0):
            self.choose_cursor = 0
            frame.SetCursor(wx.Cursor(wx.CURSOR_ARROW))                               #注意加载光标资源的方法
        elif(iSelection == 1):
            self.choose_cursor = 1
            frame.SetCursor(wx.Cursor(wx.CURSOR_CROSS))
        else:
            self.choose_cursor = 2
            MyCursor=wx.Cursor(cursorName="cursor1.cur",type=wx.BITMAP_TYPE_CUR)
            frame.SetCursor(MyCursor)
        self.Refresh()

    def ChangeLanguage(self, evt):
        iSelection = evt.GetId() - 300
        if(iSelection == self.choose_language):                              #若是语言不需要切换
            return
        if(iSelection == 0):
            self.choose_language = 0
            self.SetMenuBar(self.CH_menuBar)                              #注意改变菜单资源的方法
            self.CH_menuBar.Check(300, True)                              #注意需要将对应的语言单选和光标单选恢复初始值
            self.CH_menuBar.Check(self.choose_cursor + 200, True)
            self.SetTitle(u"2020MPADLab6: Windows资源使用(2)  姓名：汪子凡 学号：10185102153")       #改变标题
            self.Refresh()                                               #重新刷新客户区，以便重新加载光标的显示文字切换
        else:
            self.choose_language = 1
            self.SetMenuBar(self.EG_menuBar)
            self.EG_menuBar.Check(301, True)
            self.EG_menuBar.Check(self.choose_cursor + 200, True)
            self.SetTitle(u"2020MPADLab6: Windows Resources(2)  Name：汪子凡  ID：10185102153")
            self.Refresh()
        

    def OnDialog(self, evt):
        iSelection = evt.GetId() - 100
        if(iSelection == 0 ):
            dlg = ModalDialog(self)
            dlg.ShowModal()                                      #创建模式对话框，使用showModal()方法
        elif(iSelection == 1 and self.choose_open == 0):
            self.choose_open = 1
            dlg1 = ModalessDialog(self)   
            dlg1.Show()                                          #创建无模式对话框，使用show()方法
        else:
            wil=u"All Files(*.*)|*.*|C++ files(*.cpp)|*.cpp|Python source(*.py)|*.py"              #创建通用对话框
            if(self.choose_language ==0):
                Dlg=wx.FileDialog(None, u"", os.getcwd(), wil, style = wx.FD_OPEN)
            else:
                Dlg=wx.FileDialog(None, u"", os.getcwd(), wil, style = wx.FD_OPEN)
            if(Dlg.ShowModal() == wx.ID_OK):
                flag = 0
                i = 0
                for line in open(Dlg.GetPath(),"r"):
                    tmp = line.strip().split(' ')
                    print(tmp)
                Dlg.Destroy()
                if(self.choose_language == 0):
                    wx.MessageBox(text, StringTable[self.choose_language][3], wx.OK, self)
                    
                    
            else:
                Dlg.Destroy()
            
    def OnMessage(self,evt):
        wx.MessageBox(u"2020MPADLab6(WX)资源2\n光标,对话框，位图，字符串\n\n作者学号: 10185102153 姓名: 汪子凡", "2020MPADLab6(WX)" ,wx.OK | wx.ICON_INFORMATION, self)        
            
        
if __name__ == '__main__':
  app = wx.App()                #创建一个应用         
  frame = MyFrame()             #创建一个窗口
  frame.Show(True)
  app.MainLoop()
