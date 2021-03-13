#!/usr/bin/env python
# coding: utf-8

# Top-down: 对于$a, b$,可以通过辗转相除的方式得到$(a, b)$,此过程可以通过递归来完成，当$b = 0$时，便递归完成。
# 
# Bottom-up: 递归的同时还需要算出相应的系数，若$Ex(a, b)$返回$x, y, d$, 使得 $x*a + y*b = (a, b) = d$；设$a = c*b + q$, 则$c = a//b, q = a\%b$ , 那么通过Ex_GCD$(b, a\%b)$得到的便是$x*b + y*(a\%b) = d$, 即$x*b + y*(a - a//b*b) = d$, 整理得 $y*a + (x - a//b)*b = d$, 可通过这个式子进行回溯
# 

# In[35]:


import random

def Ex_GCD(a, b):
    if(b == 0):
        return 1, 0, abs(a)
    x, y, d = Ex_GCD(b, a % b)
    return y, x - a // b * y, d

L = []
for i in range(2000):
    L.append(random.randint(-100000,100000))
print("a", " "*5, "b", " "*8, "a的系数", "b的系数","最大公约数")
for i in range(1000):
    x, y, d = Ex_GCD(L[2*i], L[2*i+1])
    print("%-7d %-10d %-7d %-7d %-7d" %(L[2*i], L[2*i+1], x, y, d))


# 

# In[ ]:




