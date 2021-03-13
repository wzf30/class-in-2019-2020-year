#### **创建资源部分**

  需要建立一个图标资源(大小图标)， 三个显示的位图资源，一个自定义的鼠标资源，两个对话框资源(一个为复选一个为多选)，加速键的资源，字符串资源以及菜单资源。



  建立菜单资源的时候，由于有中英文的切换，可以有两种方式：①建立两个菜单(比如IDR_LANGUAGE_CH和LDR_LANGUAGE_EG)，在用户进行切换时(如中文切换英文)：

```c++
SetMenu(hWnd, LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENU_CH)));
DestroyMenu(hMenuMain);                       //销毁原来的菜单资源
```

这种方式的时候为了之后处理WM_COMMAND的简便，可以将两个菜单的对应控件用一样的ID

②创建一个菜单，通过以下函数在对应的字符串表里进行修改：

```c++
ModifyMenu(HMenuMain, oldID, newID, "facetxt");
```

   

  建立字符串资源的时候，每个字符串都有中英文两种格式。



  建立资源的时候还要注意就是取ID名字的时候要能准确表示其内容，ID40001这样的表示在写代码的时候不知道它表示了什么内容；还有注意不要混淆，比如光标资源的ID名和菜单更换光标的菜单项的ID，是两个不同的ID，是在WM_COMMAND中switch到了更换光标的菜单项的ID然后去Load光标资源ID。

  

####   编写代码部分

  在注册窗口类的时候加载入菜单，光标资源，由于这次作业使用了无模式对话框，所以需要增加全局变量：

```c++
HWND hDlgModeless = 0;            //0表示无模式对话框没有打开
```

  需要修改消息循环：

```c++
if((!IsWindow(hDlgModeless) || !IsDialogMessage(hDlgModeless, &msg)) && (!TranslateAccelerator(hWnd, haccel, &msg)))
```

  无模式消息框和模式消息框需要定义自己的回调函数，要注意它们的创建方法不同。

```c++
BOOL CALLBACK ModelDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK ModelessDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
```

  在WM_create消息中，需要获取位图资源的长宽(后面加载位图资源的时候需要用到)。

  在菜单里进行位图显示的时候，为了方便起见(效率可能不是最优)，可以调用InvalidRect函数对整个客户区进行刷新重画，这时候需要静态变量去记录应该print出来的位图和文字，然后在WM_PAINT消息中print出来(其中注意对位图显示的四个步骤)。对于文字的显示则需要学会字符串表里字符串的load过程。

  而记录了应该显示的位图之后，在每次打开复选对话框或者单选对话框的时候，要进行相应的选中，同时也要记录用户的改变操作，而这个改变操作不能直接对静态变量的值修改，因为用户可能会点取消，这样的话这些操作就是无效的，当用户点击确定的时候这些变化才可以对静态变量进行改变。

  通用对话框可按照视频里老师的讲解创建。

  语言的选择也需要用静态变量记录下来，语言改变时，这个静态变量也需要改变，同时也要将相应的字符串表给改变，还有菜单上的check标志也需要重新打印(如选中哪个光标)