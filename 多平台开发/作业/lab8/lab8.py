# -*- coding: utf8 -*-

import wx
import os





class MyFrame(wx.Frame):
    def __init__(self):
        wx.Frame.__init__(self, None, -1, u"2020MPADLab8: 绘制图形(2)  姓名：汪子凡 学号：10185102153", size = (1000, 700))      #调用基类的构造函数


        self.Bind(wx.EVT_PAINT, self.OnPaint)                       #将paint事件与函数OnPaint绑定

        #创建一个菜单栏
        self.menuBar = wx.MenuBar()           
        #接下来创建菜单标题(popmenu)，先用wx.Menu()创建，然后用Append方法添加菜单项，并将每个菜单项的选中与一个操作进行bind，最后将该菜单标题添加到菜单栏中
        Pop_File = wx.Menu()
        Pop_File.Append(100, u"&New\tCtrl+N", u"c创建新文件")   #\t后面直接定义了加速键
        self.Bind(wx.EVT_MENU, self.OnNew, id = 100)
        Pop_File.Append(101, u"&Open\tCtrl+O", u"打开新文件")
        self.Bind(wx.EVT_MENU, self.OnOpen, id = 101)
        Pop_File.Append(102, u"&Save\tCtrl+S", u"保存文件")
        self.Bind(wx.EVT_MENU, self.OnSave, id = 102)
        Pop_File.Append(103, u"&Save as...\tCtrl+Shift+S", u"另存为")
        self.Bind(wx.EVT_MENU, self.OnSave_as, id = 103)
        Pop_Recently = wx.Menu()
        Pop_Recently.Append(104, " ", u" ")
        Pop_Recently.Append(105, " ", u" ")
        Pop_Recently.Append(106, " ", u" ")
        self.Bind(wx.EVT_MENU, self.OnRecently, id = 104, id2 = 106)
        Pop_File. AppendSubMenu(Pop_Recently, u"&Recently File")    #recently files要创建二级菜单
        self.menuBar.Append(Pop_File, u"&File")                     #将该菜单标题添加到菜单栏

        Pop_Settings = wx.Menu()
        Pop_Change = wx.Menu()
        Pop_Change.Append(107, u"Beizer", u"Beizer", wx.ITEM_RADIO)
        Pop_Change.Append(108, u"Triangle", u"Triangle", wx.ITEM_RADIO)
        self.Bind(wx.EVT_MENU, self.OnChoose, id = 107, id2 = 108)
        Pop_Settings. AppendSubMenu(Pop_Change, u"&Change")
        Pop_Settings.Append(109, u"Repaint", u"重新绘制")
        self.Bind(wx.EVT_MENU, self.OnRepaint, id = 109)
        self.menuBar.Append(Pop_Settings, u"Settings")

        Pop_Help = wx.Menu()
        Pop_Help.Append(110, u"Lab7 help", u"Lab7 help")
        self.Bind(wx.EVT_MENU, self.OnHelp, id = 110)
        Pop_Help.Append(111, u"About", u"About")
        self.Bind(wx.EVT_MENU, self.OnAbout, id = 111)
        self.menuBar.Append(Pop_Help, u"Help")
        
        self.SetMenuBar(self.menuBar)

        icon = wx.Icon(name = "icon1.ico", type = wx.BITMAP_TYPE_ICO)                        #创建icon资源
        self.SetIcon(icon)

        self.choose = 0
        self.state = 0
        self.fErase = False
        self.filename = None
        self.point = [[0, 0] for i in range(4)]
        self.Bind(wx.EVT_LEFT_DOWN, self.OnMouseLeftDown)
        self.Bind(wx.EVT_LEFT_UP, self.OnMouseLeftUp)
        self.Bind(wx.EVT_MOTION, self.OnMouseMove)

        self.showMesg= ["Beizer曲线绘制：",
                                "1.按下鼠标左键，即确定端点P0，可以拖动，放开左键确定端点P3",
                                "2.再次按下左键，可以拖动，放开左键确定控制点P1(直线P0P1与曲线相切)",
                                "3.再次按下左键，可以拖动，放开左键确定控制点P2(直线P2P3与曲线相切)",
                                "三角形绘制：",
                                "1.按下鼠标左键，即确定顶点P0，可以拖动，放开左键确定端点P1",
                                "2.再次按下左键，可以拖动，放开左键确定控制点P2",
                                "绘制完成！可以重新绘制或者保存",
                                "当前还没有打开任何文件,可以打开一个txt文件或者新建一个"]
        self.buffer = []
        for line in open("data.txt","r"):
                self.buffer.append(line.strip())
        for i in  range(3):
            self.menuBar.FindItemById(104 + i).SetText(self.buffer[i])
        

    def OnPaint(self,evt):
        dc = wx.PaintDC(self)
        dc.SetPen(wx.Pen("black", 5))
        if(self.choose == 0):
            dc.DrawText(self.showMesg[0], 10, 10)
            dc.DrawText(self.showMesg[1], 10, 30)
            dc.DrawText(self.showMesg[2], 10, 50)
            dc.DrawText(self.showMesg[3], 10, 70)
            if(self.state == 2):
                dc.DrawLine(self.point[0][0], self.point[0][1], self.point[3][0], self.point[3][1])
            elif(self.state > 2):
                dc.DrawText(self.showMesg[7], 10, 100)
                dc.DrawSpline(self.point)
        else:
            dc.DrawText(self.showMesg[4], 10, 10)
            dc.DrawText(self.showMesg[5], 10, 30)
            dc.DrawText(self.showMesg[6], 10, 50)
            if(self.state == 2):
                dc.DrawLine(self.point[0][0], self.point[0][1], self.point[1][0], self.point[1][1])
            elif(self.state > 2):
                dc.DrawText(self.showMesg[7], 10, 100)
                dc.DrawLine(self.point[0][0], self.point[0][1], self.point[2][0], self.point[2][1])
                dc.DrawLine(self.point[2][0], self.point[2][1], self.point[1][0], self.point[1][1])
                dc.DrawLine(self.point[0][0], self.point[0][1], self.point[1][0], self.point[1][1])
        if(self.filename == None):                                       #显示现在的绘图地址
            dc.DrawText(self.showMesg[8], 10, 600)
        else:
            dc.DrawText(self.filename, 10, 600)
        self.menuBar.Check(self.choose + 107, True)                       #显示菜单的绘图格式

    def OnMouseLeftDown(self, evt):
        p = evt.GetPosition()
        dc = wx.ClientDC(self)
        dc.SetPen(wx.Pen("black", 5))
        dc.SetLogicalFunction(wx.INVERT)                #设置绘图模式    
        if(self.choose == 0):
            if(self.state == 0):
               self.point[0][0] = p.x
               self.point[0][1] = p.y
               self.state = 1
            elif(self.state == 2):
                dc.DrawLine(self.point[0][0], self.point[0][1], self.point[3][0], self.point[3][1])
                self.point[1][0] = p.x
                self.point[1][1] = p.y
                self.point[2][0] = p.x
                self.point[2][1] = p.y
                dc.DrawSpline(self.point)
                self.state = 3
                self.fErase = True
            elif(self.state == 4):
                dc.DrawSpline(self.point)
                self.point[2][0] = p.x
                self.point[2][1] = p.y
                dc.DrawSpline(self.point)
                self.state = 5
                self.fErase = True
        else:
            if(self.state == 0):
                self.point[0][0] = p.x
                self.point[0][1] = p.y
                self.state = 1
            elif(self.state == 2):
                self.point[2][0] = p.x
                self.point[2][1] = p.y
                dc.DrawLine(self.point[0][0], self.point[0][1], self.point[2][0], self.point[2][1])
                dc.DrawLine(self.point[1][0], self.point[1][1], self.point[2][0], self.point[2][1])
                self.state = 3
                self.fErase = True
                
                
    def OnMouseLeftUp(self, evt):
        p = evt.GetPosition()
        self.fErase = False
        dc = wx.ClientDC(self)
        dc.SetPen(wx.Pen("black", 5))
        dc.SetLogicalFunction(wx.INVERT)                #设置绘图模式
        if(self.choose == 0):
            if(self.state == 1):
                self.point[3][0] = p.x
                self.point[3][1] = p.y
                self.state = 2
            elif(self.state == 3):
                self.point[1][0] = p.x
                self.point[1][1] = p.y
                self.state = 4
            elif(self.state == 5):
                self.point[2][0] = p.x
                self.point[2][1] = p.y
                self.state = 6
                self.Refresh()
        else:
            if(self.state == 1):
                self.point[1][0] = p.x
                self.point[1][1] = p.y
                self.state = 2
            elif(self.state == 3):
                self.point[2][0] = p.x
                self.point[2][1] = p.y
                self.state = 4
                self.Refresh()

    def OnMouseMove(self, evt):
        p = evt.GetPosition()
        dc = wx.ClientDC(self)
        dc.SetPen(wx.Pen("black", 5))
        dc.SetLogicalFunction(wx.INVERT)
        if evt.LeftIsDown():
            if(self.choose == 0):
                if(self.state == 1):
                    if(self.fErase):
                        dc.DrawLine(self.point[0][0], self.point[0][1], self.point[3][0], self.point[3][1])
                    self.point[3][0] = p.x
                    self.point[3][1] = p.y
                    dc.DrawLine(self.point[0][0], self.point[0][1], self.point[3][0], self.point[3][1])
                elif(self.state == 3):
                    if(self.fErase):
                        dc.DrawSpline(self.point)
                    self.point[1][0] = p.x
                    self.point[1][1] = p.y
                    self.point[2][0] = p.x
                    self.point[2][1] = p.y
                    dc.DrawSpline(self.point)
                elif(self.state == 5):
                    if(self.fErase):
                        dc.DrawSpline(self.point)
                    self.point[2][0] = p.x
                    self.point[2][1] = p.y
                    dc.DrawSpline(self.point)
            else:
                if(self.state == 1):
                    if(self.fErase):
                        dc.DrawLine(self.point[0][0], self.point[0][1], self.point[1][0], self.point[1][1])
                    self.point[1][0] = p.x
                    self.point[1][1] = p.y
                    dc.DrawLine(self.point[0][0], self.point[0][1], self.point[1][0], self.point[1][1])
                elif(self.state == 3):
                    if(self.fErase):
                        dc.DrawLine(self.point[0][0], self.point[0][1], self.point[2][0], self.point[2][1])
                        dc.DrawLine(self.point[1][0], self.point[1][1], self.point[2][0], self.point[2][1])
                    self.point[2][0] = p.x
                    self.point[2][1] = p.y
                    dc.DrawLine(self.point[0][0], self.point[0][1], self.point[2][0], self.point[2][1])
                    dc.DrawLine(self.point[1][0], self.point[1][1], self.point[2][0], self.point[2][1])
                
            self.fErase = True

    def OnNew(self,evt):
        wil=u"TXT source(*.txt)|*.txt"              #创建通用对话框
        Dlg=wx.FileDialog(None, u"新建", os.getcwd(), wil, style = wx.FD_OPEN)
        if(Dlg.ShowModal() == wx.ID_OK):
            self.filename = Dlg.GetPath()
            if(self.filename[-4:-1] != ".tx"):
                self.filename += ".txt"
            print(self.filename)
            self.Refresh()
            self.state = 0
            self.Erase = False
            self.choose = 0
            open(self.filename, "w")
            

    def ModifyMenu(self):
        tmp = self.buffer[0]
        self.buffer[0] = self.filename
        self.menuBar.FindItemById(104).SetText(self.buffer[0])
        if(tmp == self.buffer[0]):
            return
        for i in range(1, 3):
            if(self.buffer[i] == self.buffer[0]):
                self.buffer[i] = tmp
                self.menuBar.FindItemById(104 + i).SetText(self.buffer[i])
                break
            tmp1 = tmp
            tmp = self.buffer[i]
            self.buffer[i] = tmp1
            self.menuBar.FindItemById(104 + i).SetText(self.buffer[i])
        with open("data.txt",'w') as f:
            for i in range(3):
                f.write(str(self.buffer[i]) + "\n")

    def Open(self, tmp):
        self.choose = int(tmp[0][0])
        for i in range(4):
            self.point[i][0] = int(tmp[i+1][0])
            self.point[i][1] = int(tmp[i+1][1])
        self.state = 10
        fErase = True
        self.ModifyMenu()
        self.Refresh()
                
    def OnOpen(self,evt):
        wil=u"Txt source(*.txt)|*.txt"              #创建通用对话框
        Dlg=wx.FileDialog(None, u"打开", os.getcwd(), wil, style = wx.FD_OPEN)
        
        if(Dlg.ShowModal() == wx.ID_OK):
            flag = 0                                           #判断打开文件是否数据格式正确
            tmp = []
            for line in open(Dlg.GetPath(),"r"):
                tmp.append(line.strip().split(' '))
            if(len(tmp) == 5 and len(tmp[0]) == 1 and tmp[0][0].isdigit()):
                for i in range(1, 5):
                    if(len(tmp[i]) == 2 and tmp[i][0].isdigit() and tmp[i][1].isdigit()):
                        if(i == 4):
                            flag = 1
            if(flag == 0):
                wx.MessageBox(u"读取文件失败", "提示" ,wx.OK | wx.ICON_INFORMATION, self)
                
            else:                                              #读取数据
                self.filename = Dlg.GetPath()
                self.Open(tmp)
                self.Refresh()
            Dlg.Destroy()
        else:
            Dlg.Destroy()


    def OnSave(self,evt):
        if((self.choose == 0 and self.state < 6) or (self.choose == 1 and self.state < 4)):
           wx.MessageBox(u"当前没有绘制完成", "提示" ,wx.OK | wx.ICON_INFORMATION, self)
        elif(self.filename != None):
            self.save()
        else:
            self.OnSave_as(evt)

    def OnSave_as(self,evt):
        if((self.choose == 0 and self.state < 6) or (self.choose == 1 and self.state < 4)):
           wx.MessageBox(u"当前没有绘制完成", "提示" ,wx.OK | wx.ICON_INFORMATION, self)
           return
        wil=u"Txt source(*.txt)|*.txt"              #创建通用对话框
        Dlg=wx.FileDialog(None, u"另存为", os.getcwd(), wil, style = wx.FD_SAVE)
        if(Dlg.ShowModal() == wx.ID_OK):
            self.filename = Dlg.GetPath()
            if(self.filename[-4:-1] != ".tx"):
                self.filename += ".txt"
            print(self.filename)
            self.save()
            Dlg.Destroy()
        else:
            Dlg.Destroy()

    def save(self):
        with open(self.filename,'w') as f:
            f.write(str(self.choose) + "\n")
            for i in range(4):
                f.write(str(self.point[i][0]) + " " + str(self.point[i][1]) + "\n")
        self.ModifyMenu()
        self.Refresh()
        

    def OnRecently(self,evt):
        tmp_file = self.buffer[evt.GetId() - 104]
        flag = 0                                           #判断打开文件是否数据格式正确
        tmp = []
        for line in open(tmp_file,"r"):
            tmp.append(line.strip().split(' '))
        if(len(tmp) == 5 and len(tmp[0]) == 1 and tmp[0][0].isdigit()):
            for i in range(1, 5):
                if(len(tmp[i]) == 2 and tmp[i][0].isdigit() and tmp[i][1].isdigit()):
                    if(i == 4):
                        flag = 1
        if(flag == 0):
            wx.MessageBox(u"读取文件失败", "提示" ,wx.OK | wx.ICON_INFORMATION, self)
                
        else:                                              #读取数据
            self.filename = tmp_file
            self.Open(tmp)
            self.Refresh()

    def OnChoose(self,evt):
         i = evt.GetId()  - 107
         if(i != self.choose):
             self.choose = i
             self.state = 0
             self.fErase = False
             self.Refresh()

    def OnRepaint(self,evt):
        self.state = 0
        self.fErase = False
        self.Refresh()

    def OnHelp(self,evt):
        wx.MessageBox(u"本次作业实现了基本的文件保存新建打开功能(以txt格式)", "2020MPADlab8" ,wx.OK | wx.ICON_INFORMATION, self) 

    def OnAbout(self,evt):
        wx.MessageBox(u"2020MPADLab8 绘图\n\n作者学号: 10185102153 姓名: 汪子凡", "2020MPADlab8" ,wx.OK | wx.ICON_INFORMATION, self)

if __name__ == '__main__':
  app = wx.App()                #创建一个应用         
  frame = MyFrame()             #创建一个窗口
  frame.Show(True)
  app.MainLoop()
