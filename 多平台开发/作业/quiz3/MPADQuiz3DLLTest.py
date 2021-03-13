#_*_coding:utf-8_*_
# above line indicates encoding is UTF-8

import wx
import ctypes
import sys
#sys.path.append('.')

DLLNAME=r"./MAPDQuiz3DLL.DLL" if sys.platform=="win32" else r"./MPADQuiz3SO.so"


class MyFrame(wx.Frame):

    def __init__(self):
        wx.Frame.__init__(self, None, wx.ID_ANY, 'DLL Example', size=(0, 0))
        panel = wx.Panel(self, wx.ID_ANY) 

        In00=wx.StaticText(panel, wx.ID_ANY, u"")
        In01=wx.StaticText(panel, wx.ID_ANY, u"")

        In1=wx.StaticText(panel, wx.ID_ANY, u"         输入x,a,b,c:")
        self.InText1 = wx.TextCtrl(panel, wx.ID_ANY, "10   3 2 1",  size=(350, -1))

        In2=wx.StaticText(panel, wx.ID_ANY, u"         输入字串:")
        self.InText2 = wx.TextCtrl(panel, wx.ID_ANY, "string is LOWERED by a dll deSIGned bY You",  size=(350, -1))

        self.InText1.SetInsertionPoint(0)

        Out1=wx.StaticText(panel, wx.ID_ANY, u"  函数f计算结果:")
        self.OutText1 = wx.StaticText(panel, wx.ID_ANY, " ", size=(350, -1))
        Out2=wx.StaticText(panel, wx.ID_ANY, u"  函数lower计算结果:")
        self.OutText2 = wx.StaticText(panel, wx.ID_ANY, " ", size=(350, -1))
        

        In02=wx.StaticText(panel, wx.ID_ANY, u"")
        Compute=wx.Button(panel, wx.ID_ANY, u"    计算    ", size=(-1, -1))
        
        self.Bind(wx.EVT_BUTTON, self.EvtCompute, Compute)

        In03=wx.StaticText(panel, wx.ID_ANY, u"")
        Exit=wx.Button(panel, wx.ID_ANY, u"   退出    ", size=(-1, -1))
        self.Bind(wx.EVT_BUTTON, self.EvtExit, Exit)

        sizer = wx.FlexGridSizer(cols=2, hgap=5, vgap=10)
        sizer.AddMany([In00,In01, In1, self.InText1,In2, self.InText2,Out1, \
                       self.OutText1,Out2, self.OutText2,In02, Compute,In03, Exit])
        panel.SetSizer(sizer)
        self.SetSize(650,300)
        self.Centre()
        

        
        self.cdll=ctypes.CDLL(DLLNAME)
        
        

    def EvtCompute(self, event):
        self.cdll.author.restype=ctypes.c_char_p
        author=str(self.cdll.author(1),encoding='ascii')+"  "+\
               str(self.cdll.author(2),encoding='gbk'if sys.platform=="win32" else 'utf-8')
               
        self.SetLabel(author)
        
        a=[int(x) for x in self.InText1.GetValue().split()]
        self.cdll.f.restype=ctypes.c_int
        ArrayType=ctypes.ARRAY(ctypes.c_int,3)
        self.cdll.f.argstype=[ctypes.c_int,ArrayType]
        result=self.cdll.f(a[0],ArrayType(*(a[1:])))
        self.OutText1.SetLabel(str(result))


        self.cdll.lower.restype = ctypes.c_char_p
        result=self.cdll.lower(self.InText2.GetValue().encode("ascii"))
        self.OutText2.SetLabel(result)


        
    def EvtExit(self, event): self.Destroy()

if __name__ == '__main__':
#    import os
#    os.chdir(r"your working directory")
    app = wx.App()
    frame = MyFrame()
    frame.Show()
    app.MainLoop()
