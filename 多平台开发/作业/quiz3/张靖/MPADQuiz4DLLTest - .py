#_*_coding:utf-8_*_
import wx
from ctypes import *
from sympy import *
import sys
import matplotlib
from matplotlib.figure import Figure
from matplotlib.backends.backend_wxagg import FigureCanvasWxAgg as FigureCanvas
#from numpy import arange, sin, pi
from numpy import *
DLLNAME=r"./MAPDQuiz4DLL.DLL" if sys.platform=="win32" else r"./MPADQuiz4SO.so"

class myFrame(wx.Frame):
  def __init__(self, parent=None, id=-1,pos=wx.DefaultPosition,title='matplot', style=wx.DEFAULT_FRAME_STYLE ):
     wx.Frame.__init__(self, parent, id, title, pos=(10,10), style=style)
     self.SetBackgroundColour('Gray')
     menuFile = wx.Menu()
     menuFile.Append(2, "E&xit")

     menuBar = wx.MenuBar()
     menuBar.Append(menuFile,'&File')
     self.SetMenuBar(menuBar)

     self.Bind(wx.EVT_MENU,  self.OnQuit, id=2)
     self.Bind(wx.EVT_CLOSE, self.OnQuit, id=2)
     self.Bind(wx.EVT_SIZE,  self.OnSize)
     self.Bind(wx.EVT_PAINT, self.OnPaint)    

     self.figure = Figure(figsize=(15,7.5))
     self.figure.patch.set_facecolor('gray')
     self.axes = self.figure.add_subplot(111)
     self.axes.patch.set_facecolor('gray')
     self.axes.axis("off")
     

     t = arange(0.0,3.0,0.01); s = t*2
     self.axes.plot(t,s,color='r',lw=3.0)

     In=wx.StaticText(self, wx.ID_ANY, u"       输入: ")
     font=self.GetFont()
     font.PointSize+=10
     self.InText = wx.TextCtrl(self, wx.ID_ANY, \
                               "$t = arange(0.0,3.0,0.01); s = t*2; \n"
                               "self.axes.plot(t,s,color=self.lineColor[self.lineColorIdx],lw=3.0)", \
                               size=(800, 100),style=wx.TE_MULTILINE)
     self.InText.SetFont(font)
     self.InText.SetInsertionPoint(0)
     Compute=wx.Button(self, wx.ID_ANY, u"显示", size=(50, -1))
     SORT=wx.Button(self, wx.ID_ANY, u"排序", size=(50, -1),pos=(900,0))
     author=wx.Button(self, wx.ID_ANY, u"显示作者", size=(50, -1),pos=(1000,0))
     simple=wx.Button(self, wx.ID_ANY, u"展开", size=(50, -1),pos=(1100,0))
     Fenjie=wx.Button(self, wx.ID_ANY, u"化简", size=(50, -1),pos=(1150,0))
     self.Bind(wx.EVT_BUTTON, self.huajian, Fenjie)
     self.Bind(wx.EVT_BUTTON, self.EvtSORT, SORT)
     self.Bind(wx.EVT_BUTTON, self.zhankai, simple)
     self.Bind(wx.EVT_BUTTON, self.show, author)
     sizer1 = wx.BoxSizer(wx.HORIZONTAL)
     sizer1.AddMany([In,self.InText, Compute])        
     self.Bind(wx.EVT_BUTTON, self.EvtCompute, Compute)
     self.sizer = wx.FlexGridSizer(rows=2, cols=1, hgap=1, vgap=1)
     self.sizer.Add(sizer1)
     self.canvas = FigureCanvas(self, -1, self.figure)
     self.sizer.Add(self.canvas,flag=wx.EXPAND)
     self.SetSizer(self.sizer)
     self.Fit()
     self.lineColor=['red','green','blue','cyan','magenta','yellow','black','white','#FF00FF']
     self.lineColorIdx=0
     self.cdll=CDLL(DLLNAME)
     self.cdll.author.restype=c_char_p
     
  def show(self,event):
    author=str(self.cdll.author(1),encoding='ascii')+"  "+\
    str(self.cdll.author(2),encoding='gbk'if sys.platform=="win32" else 'utf-8')
    self.SetLabel(author) 
  def EvtCompute(self, event):
    formula=self.InText.GetValue()
    if formula.startswith('$'):
      self.axes.clear()
      formula=formula[1:]
    self.axes.patch.set_facecolor('gray')
    self.axes.axis("off")
    self.lineColorIdx=(self.lineColorIdx+1)%len(self.lineColor)
    exec(formula)
    self.canvas.draw()
    
  def OnPaint(self, event):
    event.Skip()
    
  def EvtSORT(self,event):
    formula=self.InText.GetValue()
    judge=formula[len(formula)-1]
    formula2=formula[:len(formula)-2]
    a=[int(x) for x in formula2.split()]
    c=[i for i in range(0,len(a))]
    print(a)
    ArrayType=c_int*len(a)
    b=ArrayType(*a)
    qsort =self.cdll.iSort
    qsort.restype = None
    qsort.argtypes = [ArrayType, c_int, c_int]
    print(judge)
    if(judge=="a"):
      
      qsort(b,len(a),1)
    if(judge=="b"):
      qsort(b,len(a),-1)
    for i in range(0,len(a)):
      c[i]=b[i]
    self.axes.clear()
    self.axes.axis("off")
    x=[i for i in range(0,len(a))]
    self.axes.plot(x,c,color=self.lineColor[self.lineColorIdx+1],lw=3.0)
    self.axes.plot(x,a,color=self.lineColor[self.lineColorIdx],lw=3.0)
    self.canvas.draw()
  def zhankai(self,event):
    x,y,a=symbols("x y a")
    formula=self.InText.GetValue()
    print(expand(formula))
    str=latex(expand(formula))
    self.axes.clear()
    self.axes.axis("off")
    self.axes.text(0.5,0.9,'$'+str+'$',size=30)
    self.canvas.draw()
  def huajian(self,event):
    x,y,a=symbols("x y a")
    formula=self.InText.GetValue()
    str=latex(simplify(formula))
    self.axes.clear()
    self.axes.axis("off")
    self.axes.text(0.5,0.9,'$'+str+'$',size=30)
    self.canvas.draw()
  def OnSize(self, event):
    self.Refresh()
    event.Skip()


  def OnQuit(self, event):
      self.Close()


class App(wx.App):
  def __init__(self,f):
    wx.App.__init__(self,f)

  def OnInit(self):
    frame = myFrame(parent=None, title='MATLAB style frame')
    frame.Show()
    self.SetTopWindow(frame)
    return True


app = App(False)
app.MainLoop()
