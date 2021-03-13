#_*_coding:utf-8_*_

import wx
import os
import os.path

is_check=[0,0,0]  #记录复选对话框按钮信息，以便于下次打开恢复
is_check_original=[0,0,0] #记录is_check改变前的情况，便于cancel时恢复
global which_radio,original#记录单选对话框按下了哪个按钮和改变前按下那个，类似于上面的解释
which_radio=0
original=0
global is_open #阻止无模式对话框重复打开
is_open=0
global chinese_or_english#用于判断
chinese_or_english=0

class ModalDialog(wx.Dialog):
    def __init__(self,parent):
        wx.Dialog.__init__(self, parent, -1, u"复选对话框",pos=(500,300),size=(500, 300))
        panel=wx.Panel(self)#创建画板
        self.okButton = wx.Button(panel, wx.ID_OK, u"确定", pos=(30, 200),size=(130,30))
        self.okButton.SetDefault()
        self.cancelButton = wx.Button(panel, wx.ID_CANCEL, u"取消",pos=(250, 200),size=(130,30))
        self.check1=wx.CheckBox(panel,201,u"复选位图1",pos=(30,50),size=(130,40))
        self.check2=wx.CheckBox(panel,202,u"复选位图2",pos=(30,100),size=(130,40))
        self.check3=wx.CheckBox(panel,203,u"复选位图3",pos=(30,150),size=(130,40))
        self.Bind(wx.EVT_CHECKBOX,self.OnCheckBoxs,id=201,id2=203)
        self.Bind(wx.EVT_BUTTON,self.OnOk,self.okButton)
        self.Bind(wx.EVT_BUTTON,self.OnCancel,self.cancelButton)
        
        if is_check[0]==1:
            self.check1.SetValue(True)
        if is_check[1]==1:
            self.check2.SetValue(True)
        if is_check[2]==1:
            self.check3.SetValue(True)
            
    def OnOk(self,evt):
        for i in range(0,3):
            is_check_original[i]=is_check[i]
        self.GetParent().Refresh()
        self.Destroy()
        
    def OnCancel(self,evt):
        for i in range(0,3):
            is_check[i]=is_check_original[i]
        self.Destroy()

    def OnCheckBoxs(self,evt):
        item=evt.GetId()
        tem=item-201
        is_check[tem]=(is_check[tem]+1)%2
        
class ModalessDialog(wx.Dialog):
    def __init__(self,parent):
        wx.Dialog.__init__(self, parent, -1, u"单选对话框",size=(400, 300),pos=(500,300))
        panel1=wx.Panel(self)
        self.okButton = wx.Button(panel1, wx.ID_OK, u"确认", pos=(30, 200),size=(130,40))
        self.okButton.SetDefault()
        self.cancelButton = wx.Button(panel1, wx.ID_CANCEL, u"取消",pos=(250, 200),size=(130,40))
        self.Bind(wx.EVT_BUTTON, self.OnOK, self.okButton)
        self.Bind(wx.EVT_BUTTON, self.OnCancel, self.cancelButton)
        
        self.radio1=wx.RadioButton(panel1,204,u"单选位图1",pos=(30,50),size=(130,40))
        self.radio2=wx.RadioButton(panel1,205,u"单选位图2",pos=(30,100),size=(130,40))
        self.radio3=wx.RadioButton(panel1,206,u"单选位图3",pos=(30,150),size=(130,40))
        self.Bind(wx.EVT_RADIOBUTTON,self.OnRadio,id=204,id2=206) 
        
        global which_radio,is_open
        is_open=1
        if which_radio==0:
            self.radio1.SetValue(True)
        elif which_radio==1:
            self.radio2.SetValue(True)
        else:
            self.radio3.SetValue(True)

    def OnOK(self, evt):
        global which_radio,original,is_open
        is_open=0
        original=which_radio
        self.GetParent().Refresh()
        self.Destroy()

    def OnCancel(self, evt):
        global which_radio,original,is_open
        is_open=0
        which_radio=original
        self.Destroy()

    def OnRadio(self,evt):
        item=evt.GetId()
        global which_radio,original
        original=which_radio
        which_radio=item-204
            
        

class MyFrame(wx.Frame):
    def __init__(self):
        path=os.path.dirname(__file__)
        os.chdir(path)
        wx.Frame.__init__(self,None,-1,u"LAB6",size=(800,500))
        self.menuBar=wx.MenuBar()
        self.icon = wx.Icon('icon1.ico', wx.BITMAP_TYPE_ICO)#设置图标
        self.SetIcon(self.icon) 
        self.SetCursor(wx.StockCursor(wx.CURSOR_ARROW))#设置光标
        
        image = wx.Image(name="bitmap1.bmp", type=wx.BITMAP_TYPE_BMP)
        self.bmp1=image.ConvertToBitmap()#设用于复选对话框的位图1
        
        image=wx.Image(name="bitmap2.bmp",type=wx.BITMAP_TYPE_BMP) #设用于复选对话框的位图2
        self.bmp2=image.ConvertToBitmap()
        
        image=wx.Image(name="bitmap3.bmp",type=wx.BITMAP_TYPE_BMP)#设用于复选对话框的位图3
        self.bmp3=image.ConvertToBitmap()
        
        self.x, self.y = self.bmp1.GetWidth(), self.bmp1.GetHeight()
        self.Bind(wx.EVT_PAINT, self.OnPaint)
        
        self.which_cursor=0
        self.CUrsor1=[u"当前光标是:箭头",u"当前光标是:十字",u"当前光标是:我的光标"]#设置用于显示“用那个光标”的静态文本
        self.CUrsor2=[u"The current cursor is:ARROW",u"The current cursor is:CROSS",u"The current cursor is:MY CURSOR"]
        self.CURSORS=[]
        self.CURSORS=self.CUrsor1
        self.which_cursor=0
        
        #以下是菜单项制作
        self.file=wx.Menu();
        self.file.Append(100,u"退出(X)",u"")
        self.Bind(wx.EVT_MENU,self.OnExit,id=100)
        self.menuBar.Append(self.file,u"文件(F)")
        
        self.dialog=wx.Menu()
        self.dialog.Append(101,u"&模式对话框(M)\tCtrl-A",u"")
        self.dialog.Append(102,u"&无模式对话框(L)\tCtrl-B",u"")
        self.dialog.Append(103,u"&文件对话框(F)\tCtrl-C",u"")
        self.Bind(wx.EVT_MENU_RANGE,self.OnDialog,id=101,id2=103)
        self.menuBar.Append(self.dialog,u"对话框(D)")
        
        self.cursor=wx.Menu()
        self.cursor.Append(104,u"&光标1(箭头)\tCtrl-1",u"",wx.ITEM_RADIO)
        self.cursor.Append(105,u"&光标2(十字)\tCtrl-2",u"",wx.ITEM_RADIO)
        self.cursor.Append(106,u"&光标3(自定义)\tCtrl-3",u"",wx.ITEM_RADIO)
        self.Bind(wx.EVT_MENU_RANGE,self.OnCursor,id=104,id2=106)
        self.menuBar.Append(self.cursor,u"光标类型(C)")
        self.menuBar.Check(104,True)
        
        self.language=wx.Menu()
        self.language.Append(107,u"&中(Chinese)\tCtrl-Shift-C",u"",wx.ITEM_RADIO)
        self.language.Append(108,u"&English(英)\tCtrl-Shift-D",u"",wx.ITEM_RADIO)
        self.Bind(wx.EVT_MENU_RANGE,self.OnLanguage,id=107,id2=108)
        self.menuBar.Append(self.language,u"语言(L)")
        self.menuBar.Check(107,True)
        
        self.about=wx.Menu()
        self.about.Append(109,u"&程序信息(I)\tF1",u"")
        self.Bind(wx.EVT_MENU,self.OnMessage,id=109)
        self.menuBar.Append(self.about,u"关于(A)")
        
        #以下是用于中英文菜单切换的字符串
        self.menuchinese=[u"退出(X)",u"&模式对话框(M)\tCtrl-A",u"&无模式对话框(L)\tCtrl-B",
                           u"&文件对话框(F)\tCtrl-C",u"&光标1(箭头)\tCtrl-1",u"&光标2(十字)\tCtrl-2",
                           u"&光标3(自定义)\tCtrl-3",u"&中(Chinese)\tCtrl-Shift-C",u"&English(英)\tCtrl-Shift-D",
                           u"&程序信息(I)\tF1"]
        self.menuenglish=[u"Exit",u"&Modal Dialog...\tCtrl-A",u"&Modaless Dialog...\tCtrl-B",
                         u"&File Dialog...\tCtrl-C",u"&Cursor1(ARROW)\tCtrl-1",u"&Cursor2(CROSS)\tCtrl-2",
                         u"&Cursor3(UserDefined)\tCtrl-3",u"&中(C)\tCtrl-Shift-C",u"&English(英)\tCtrl-Shift-D",
                         u"&Program Information\tF1"]
        self.menunow=[]  #用于记录当前所用菜单的菜单项是menuchinese 还是menuenglish
        self.menubarchinese=[u"文件(F)",u"对话框(D)",u"光标类型(C)",u"语言(L)",u"关于(A)"]
        self.menubarenglish=[u"File",u"Dialog",u"Cursor",u"Language",u"About"]
        self.menubarnow=[] #用于记录当前所用菜单的父菜单项是menubarchinese 还是menubarenglish
       
       
        self.SetMenuBar(self.menuBar)
        
    def OnPaint(self,evt):
        global which_radio 
        dc = wx.PaintDC(self)
        dc.SetFont(wx.Font(20, wx.SWISS, wx.NORMAL, wx.BOLD, False))
        if which_radio==0:
             dc.DrawBitmap(self.bmp1, self.x*3, self.y)
        elif which_radio==1:
             dc.DrawBitmap(self.bmp2, self.x*3, self.y)
        else:
             dc.DrawBitmap(self.bmp3, self.x*3, self.y)
        if is_check[0]:
             dc.DrawBitmap(self.bmp1, self.x, self.y)
        if is_check[1]:
             dc.DrawBitmap(self.bmp2, self.x, self.y*4)
        if is_check[2]:
             dc.DrawBitmap(self.bmp3, self.x, self.y*7)
        dc.DrawText(self.CURSORS[self.which_cursor],200,200)
            
                     
    def OnDialog(self,evt):
        global chinese_or_english
        item=evt.GetId()
        flag=0
        if item==101:
            dlg = ModalDialog(self)
            dlg.ShowModal()
        if item==102:
            global is_open
            if is_open==0:
                dlg1=ModalessDialog(self)
                dlg1.Show()
        if item==103:
            wil=u"All Files(*.*)|*.*|C++ files(*.cpp)|*.cpp|Python source(*.py)|*.py"
            if chinese_or_english==0:
                Dlg=wx.FileDialog(parent=None,message=u"文件对话框",wildcard=wil,style=wx.OPEN)
                result=Dlg.ShowModal()
            else:
                Dlg=wx.FileDialog(parent=None,message=u"File Dialog",wildcard=wil,style=wx.OPEN)
                result=Dlg.ShowModal()
            if result==wx.ID_OK:
                if chinese_or_english==0:
                    text=u"文件路径:\n"+Dlg.GetPath()
                else:
                    text="File Path:\n"+Dlg.GetPath()
                flag=1;
                
            Dlg.Destroy() 
            if flag==1:
                wx.MessageBox(text,u"文件路径",wx.OK,self)
                
    def OnCursor(self,evt):
         item=evt.GetId()
         self.which_cursor=item-104
         if item==104:
             frame.SetCursor(wx.StockCursor(wx.CURSOR_ARROW))
             self.which_cursor=0
         if item==105:
             frame.SetCursor(wx.StockCursor(wx.CURSOR_CROSS))
             self.which_cursor=1
         if item==106:
             Cs=wx.Cursor(cursorName="cursor1.cur",type=wx.BITMAP_TYPE_CUR)
             frame.SetCursor(Cs) 
             self.which_cursor=2
         self.Refresh()
             
    def OnLanguage(self,evt):
        global chinese_or_english
        item=evt.GetId()
        if item==107:
            chinese_or_english=0
            self.menunow=self.menuchinese
            self.menubarnow=self.menubarchinese
            self.CURSORS=self.CUrsor1
            self.SetTitle(u"LAB6(第六次)")
            
        if item==108:
            chinese_or_english=1
            self.menunow=self.menuenglish
            self.menubarnow=self.menubarenglish
            self.CURSORS=self.CUrsor2
            self.SetTitle(u"LAB6(six)")
        for i in range(0,5):
            self.menuBar.SetLabelTop(i,self.menubarnow[i])
        self.file.SetLabel(100,self.menunow[0])
        for i in range(101,104):
            tem=i-100
            self.dialog.SetLabel(i,self.menunow[tem])
        for i in range(104,107):
            tem=i-100
            self.cursor.SetLabel(i,self.menunow[tem])
        for i in range(107,109):
            tem=i-100
            self.language.SetLabel(i,self.menunow[tem])
        self.about.SetLabel(109,self.menunow[9])
        self.Refresh()

               
    
    def OnMessage(self,evt):
        wx.MessageBox(u"学号：10175102206\n姓名：张欣竹","17Lab6",wx.OK | wx.ICON_INFORMATION,self)
        
    def OnExit(self,evt):
        self.Close()
        
        
    
if __name__ == '__main__':
    app = wx.App() 
    frame = MyFrame()
    frame.Show(True)
    app.MainLoop()