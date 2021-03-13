# -*- coding: utf8 -*- 

import wx
import psutil

class MyFrame(wx.Frame):
  def __init__(self):
    wx.Frame.__init__(self, None, -1, u"2020MPADLab2: Windows/Linux进程列表 作者学号: 10185102153 姓名:汪子凡", size = (1200, 800))
    panel = wx.Panel(self, -1)

    icon = wx.Icon(name = "icon1.ico", type = wx.BITMAP_TYPE_ICO)
    self.SetIcon(icon)
    
    for i in range(4):
      wx.StaticText(panel, -1, u"进程ID：   模块名：", pos = (i * 300, 10))
    cnt = 0
    ACCESS_DENIED=''
    for pid in sorted(psutil.pids()):
      try:
        p = psutil.Process(pid)
        pinfo = p.as_dict(ad_value=ACCESS_DENIED)
        wx.StaticText(panel, -1, u"%05x      %s"%(pid,pinfo['name']), pos = (cnt % 4 * 300, (cnt // 4 + 1) * 20 + 10))
        cnt += 1
      except psutil.NoSuchProcess: pass
                    
if __name__ == '__main__':
  app = wx.App()
  frame = MyFrame()
  frame.Show(True)
  app.MainLoop()

