import wx
import psutil
class MyFrame(wx.Frame):
  def __init__(self):
    wx.Frame.__init__(self, None, -1, "My 1st WX Program", size=(800, 600))
    panel = wx.Panel(self, -1)
    '''panel.Bind(wx.EVT_MOTION, self.OnMove)
    wx.StaticText(panel, -1, "Pos:", pos=(10, 12))
    self.posCtrl = wx.TextCtrl(panel, -1, "", pos=(40, 10))
    hello=wx.StaticText(panel, -1, "", pos=(100, 120))
    hello.SetFont(wx.Font(72, wx.SWISS, wx.NORMAL, wx.BOLD, False))
    hello.SetLabel("Hello!")'''

    ACCESS_DENIED = ''
    i=0
    for pid in sorted(psutil.pids()):
      try:
          p = psutil.Process(pid)
          pinfo = p.as_dict(ad_value=ACCESS_DENIED)
          wx.StaticText(panel,-1,u"%05x      %s" % (pid, pinfo['name']),pos=(0,200+i*20))
          i=i+2
      except psutil.NoSuchProcess: pass

  def OnMove(self, event):
    pos = event.GetPosition()
    self.posCtrl.SetValue("%s, %s" % (pos.x, pos.y))

if __name__ == '__main__':
  app = wx.App()
  frame = MyFrame()
  frame.Show(True)
  app.MainLoop()
