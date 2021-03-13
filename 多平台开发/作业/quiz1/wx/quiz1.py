#_*_coding:utf-8_*_ 
 
import wx 
class MyFrame(wx.Frame):
  def __init__(self):
    wx.Frame.__init__(self, None, -1, u"My Frame", size=(800, 500))
    # Create the menubar
    self.myMenu = wx.Menu()                     #将原来的MenuBar改成Menu

    # add a menu 
    menu = wx.Menu()

    # add an item to the menu, using \tKeyName automatically
    # creates an accelerator, the third param is some help text
    # that will show up in the statusbar
#    self.IdCommand = wx.NewId()
#    menu.Append(self.IdCommand, u"命令(&R)\tCtrl+R", "This the text in the Statusbar")
    # bind the menu event to an event handler
#    self.Bind(wx.EVT_MENU, self.OnCommand, id=self.IdCommand)

    self.IdCommand=menu.Append(-1, u"命令(&R)\tCtrl+R", "This the text in the Statusbar")
    self.Bind(wx.EVT_MENU, self.OnCommand, self.IdCommand)

    menu.AppendSeparator()

    menu.Append(wx.ID_EXIT, u"E&xit\tAlt-X", u"Exit this simple sample")
    # bind the menu event to an event handler
    self.Bind(wx.EVT_MENU, self.OnClose, id=wx.ID_EXIT)                     

    # and put the menu on the menubar
    self.myMenu.AppendSubMenu(menu, u"&File")             #将函数换成AppendSubMenu

    self.color = wx.Menu()
    # Radio items
    self.color.Append(201, u"White", u"", wx.ITEM_RADIO)
    self.color.Append(202, u"Gray", u"", wx.ITEM_RADIO)
    self.color.Append(203, u"Black", u"", wx.ITEM_RADIO)
#    self.Bind(wx.EVT_MENU, self.OnColor,id=201)
#    self.Bind(wx.EVT_MENU, self.OnColor,id=202)
#    self.Bind(wx.EVT_MENU, self.OnColor,id=203)
    self.Bind(wx.EVT_MENU_RANGE, self.OnColor,id=201,id2=203)

    self.myMenu.AppendSubMenu(self.color, u"&Color")     #将函数换成AppendSubMenu
    self.myMenu.Check(202,True)	                             #这里需要改变
    self.SetBackgroundColour(u"Gray")

    control = wx.Menu()
    # Check items
    control.Append(301, u"Enable", u"Enable/Disable BG change", wx.ITEM_CHECK)
    self.myMenu.AppendSubMenu(control, u"Con&trol")                              #将函数换成AppendSubMenu
    self.Bind(wx.EVT_MENU, self.OnControl,id=301)
    self.myMenu.Check(301,True)	                      #这里需要改变                  
    self.changeable = True

    # and another menu 
    menu = wx.Menu()

    IdAbout = menu.Append(-1, u"&About\tF1", u"Help tip")

    # bind the menu event to an event handler
    self.Bind(wx.EVT_MENU, self.OnHelp, IdAbout)

    # and put the menu on the menubar
    self.myMenu.AppendSubMenu(menu, u"&Help")                #将函数换成AppendSubMenu
    #self.SetMenuBar(self.menuBar)
	
    self.CreateStatusBar()
    self.Bind(wx.EVT_PAINT, self.OnPaint)

    self.Bind(wx.EVT_CONTEXT_MENU, self.OnContextMenu)          #这里需要添加事件

  def OnPaint(self, evt):
    dc=wx.PaintDC(self)
    # draw something in client area
    evt.Skip()

  def OnCommand(self, evt):
    wx.MessageBox(u"Sorry,运行命令 not implemented yet！",
           "Message", 
		   wx.OK | wx.ICON_EXCLAMATION, self)

  def OnColor(self, evt):
    item = self.myMenu.FindItemById(evt.GetId())          #这里需要改变        
#    text = item.GetText()
    text = item.GetItemLabel()
    wx.MessageBox(u"You selected item '%s'" % text,
           u"Color Menu", wx.OK | wx.ICON_INFORMATION, self)
    if self.changeable:
      self.SetBackgroundColour(text)
      self.Refresh()
    else:
      dc=wx.ClientDC(self)
      dc.SetTextForeground(u'red')
      dc.DrawText(u"不能改变BG!",100,50)
	 
  def OnControl(self, evt):
    self.changeable = evt.IsChecked()
#    self.GetMenuBar().Enable(self.IdCommand, self.changeable)
    self.myMenu.Enable(self.IdCommand.GetId(), self.changeable)              #这里需要改变        

  def OnHelp(self, evt):                                             #这里需要改变        
    wx.MessageBox(u"2020MPADQuiz1(WX)PopMenu\n\n作者学号： 10185102153 姓名： 汪子凡", "2020MPADQuiz1(WX)", wx.OK | wx.ICON_INFORMATION, self)
	
  def OnClose(self, evt):
    self.Close()

  def OnContextMenu(self, evt):                      #这里需要添加
      pos = evt.GetPosition()
      pos = frame.ScreenToClient(pos)                      
      frame.PopupMenu(self.myMenu, pos)

if __name__ == u'__main__':
  app = wx.App()
  frame = MyFrame()
  frame.Show(True)
  app.MainLoop()
