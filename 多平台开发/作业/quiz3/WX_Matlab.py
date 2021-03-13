#_*_coding:utf-8_*_
import wx

import matplotlib
from matplotlib.figure import Figure
from matplotlib.backends.backend_wxagg import FigureCanvasWxAgg as FigureCanvas
#from numpy import arange, sin, pi
from numpy import *
import ctypes
import sys

DLLNAME=r"./LabTest.DLL" if sys.platform=="win32" else r"./LabTest.so"


class myFrame(wx.Frame):
  def __init__(self, parent=None, id=-1,pos=wx.DefaultPosition,title='matplot', style=wx.DEFAULT_FRAME_STYLE ):
     wx.Frame.__init__(self, parent, id, title, pos=(10,10), style=style)

     self.cdll=ctypes.CDLL(DLLNAME)
     
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
