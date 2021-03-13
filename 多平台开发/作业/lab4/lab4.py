# -*- coding: utf8 -*- 

import wx


class MyFrame(wx.Frame):
  def __init__(self):
    wx.Frame.__init__(self, None, -1, u"2020MPADLab2: Windows/Linux进程列表 作者学号: 10185102153 姓名:汪子凡", size = (1000, 700))      #调用基类的构造函数

    self.menuBar = wx.MenuBar()             #创建一个菜单栏
    panel = wx.Panel(self, -1)              #建立一个面板，上面可以放各种部件
    self.choose_icon = 0

    self.showMessage = []                   #创建一个静态文本框，来打印4个显示消息
    self.showMessage.append(wx.StaticText(panel, -1, "", pos = (400, 400)))
    self.showMessage.append(wx.StaticText(panel, -1, "", pos = (600, 400)))
    self.showMessage.append(wx.StaticText(panel, -1, "", pos = (400, 600)))
    self.showMessage.append(wx.StaticText(panel, -1, "", pos = (600, 600)))
    
    self.showIcon = wx.StaticText(panel, -1, "当前显示的是：图标1", pos = (100, 100))    #用此文本框来显示当前用了哪个图标
    self.showIcon.SetFont(wx.Font(10, wx.SWISS, wx.NORMAL, wx.BOLD, False))              #这一行可以改变文本框的字体
    icon = wx.Icon(name = "icon1.ico", type = wx.BITMAP_TYPE_ICO)                        #一开始先加载入图标1
    self.SetIcon(icon)
    
    
    #接下来创建四个菜单标题(popmenu)，先用wx.Menu()创建，然后用Append方法添加菜单项，并将每个菜单项的选中与一个操作进行bind，最后将该菜单标题添加到菜单栏中
    Pop_File = wx.Menu()
    Pop_File.Append(wx.ID_EXIT, u"&Exit\tCtrl+Shift+Delete", u"退出程序")   #\t后面直接定义了加速键
    self.Bind(wx.EVT_MENU, self.OnClose, id = wx.ID_EXIT)               
    self.menuBar.Append(Pop_File, u"&File")                     #将该菜单标题添加到菜单栏
    
    Pop_Icon = wx.Menu()
    Pop_Icon.Append(100, u"图标1(&X)\tCtrl+X", u"选择图标1", wx.ITEM_RADIO)       #注意最后一个参数，表示这三个图标只能选择其中一个
    Pop_Icon.Append(101, u"图标2(&Y)\tCtrl+Y", u"选择图标2", wx.ITEM_RADIO)
    Pop_Icon.Append(102, u"图标3(&Z)\tCtrl+Z", u"选择图标3", wx.ITEM_RADIO)
    self.Bind(wx.EVT_MENU_RANGE,self.OnIcon,id=100,id2=102)                       #注意这个绑定方法，可以将3种不同的图标选中响应到一个去
    self.menuBar.Append(Pop_Icon, u"图标(I&)")                   
    self.menuBar.Check(100, True)                                                 #一开始是默认选择图标1状态 

    Pop_Show = wx.Menu()
    Pop_Show.Append(110, u"显示(&1)\tCtrl+1", u"显示信息1", wx.ITEM_CHECK)        #注意最后一个参数，表示这三个图标可以多选
    Pop_Show.Append(111, u"显示(&2)\tCtrl+2", u"显示信息2", wx.ITEM_CHECK)
    Pop_Show.Append(112, u"显示(&3)\tCtrl+3", u"显示信息3", wx.ITEM_CHECK)
    Pop_Show.Append(113, u"显示(&4)\tCtrl+4", u"显示信息4", wx.ITEM_CHECK)
    self.Bind(wx.EVT_MENU_RANGE,self.OnShow,id=110,id2=113)
    self.menuBar.Append(Pop_Show, u"显示(&D)")

    Pop_About = wx.Menu()
    Pop_About.Append(114, u"程序信息(&I)\tF1")
    self.Bind(wx.EVT_MENU, self.OnMessage, id = 114)
    self.menuBar.Append(Pop_About, u"关于(&A)") 

    self.SetMenuBar(self.menuBar)                                                #添加菜单栏到面板




  
  def OnClose(self, event):
    self.Close()

  def OnIcon(self, event):                                                                    #与SDK方法不同，wx点击菜单项会自动选中,点击已经选中(单选)的，则不会进入这个函数
    print(1)                                                                                  #在对话框里的控件都是点击即选中
    IconName = ["icon1.ico", "icon2.ico", "icon3.ico"]
    contents = [u"当前显示的是：图标1",u"当前显示的是：图标2",u"当前显示的是：图标3"]
    iSelection = event.GetId() - 100                                                       #确定要改变到哪一个图标
    returns = wx.MessageBox(u"确定要修改吗？","确认",wx.YES_NO|wx.ICON_QUESTION)           #进行消息框确认
    if returns==wx.YES:
      self.choose_icon = iSelection
      icon = wx.Icon(name = IconName[iSelection], type = wx.BITMAP_TYPE_ICO)               #改变图标并改变输出的文字
      self.showIcon.SetLabel(contents[iSelection])
      self.SetIcon(icon) 
      if(iSelection == 2):                                                                #选中第三个图标后要将第三个菜单标题灰化 
        self.GetMenuBar().EnableTop(2,False)
      else:
        self.GetMenuBar().EnableTop(2,True)
    self.menuBar.Check(self.choose_icon + 100, True)

  def OnShow(self, event):
    ShowName=[u"显示1",u"显示2",u"显示3",u"显示4"]
    iSelection = event.GetId() - 110
    if event.IsChecked():
      self.showMessage[iSelection].SetLabel(ShowName[iSelection])
    else:
      self.showMessage[iSelection].SetLabel("")
    #self.Refresh()  

  def OnMessage(self, event):
    wx.MessageBox(u"2020MPADLab3: Windows资源使用(1)\n图标、菜单、动态菜单、加速键、消息框\n作者学号：10185102153 姓名：汪子凡\n", "2020MPADLab4", wx.OK | wx.ICON_INFORMATION,self)

    
      
if __name__ == '__main__':
  app = wx.App()                #创建一个应用         
  frame = MyFrame()             #创建一个窗口
  frame.Show(True)
  app.MainLoop()

