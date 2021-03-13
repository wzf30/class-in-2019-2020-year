

def main():

    def setPosition(x, y, c):                                   #在棋盘的(x, y)处放上旗子
        screen.blit(c, (40 * x + 5, 40 * y + 15))
        pygame.display.update()
        if(c == white):                                         #在chessboard数组进行标记
            chessboard[x][y] = 1
            print("电脑走棋：", x, y)
            detect(x, y, 1)
        else:
            chessboard[x][y] = -1
            print("玩家走棋：", x, y)
            detect(x, y, -1)
            

    def getPosition(x, y, c):                               
        if(x >= 5 and x <= 605 and y >= 15 and y <= 615):
            x = (x-5)//40
            y = (y-15)//40
            if(chessboard[x][y] != 0):
                return False
            setPosition(x, y, c)
            return True
        else:
            return False

            
    def defendSearch():                                          #先检测是否需要防守
        num = 0                                                  #以五个为单位，检测到的最多的白棋子数
        Continue_flag = -1                                        #是否连续
        Block_flag = -1                                          #是否有一边被堵住
        freex = 0                                                #需要防守的位置
        freey = 0
        mark = [[0 for i in range(15)] for i in range(15)]
        for x in range(15):
            for y in range(15):
                if chessboard[x][y] == -1:                      #找到白棋
                    for i in range(8):                          #以(x, y)为起始位置，有8个方向
                        if(x + direct[i][0]*4 >= 15 or x + direct[i][0]*4 < 0 or y + direct[i][1]*4 >= 15 or y + direct[i][1]*4 < 0):  #检测是否超出棋盘界限
                            continue
                        black_num = 1
                        flag = -1
                        isContinue = -1
                        isBlock = -1
                        freex1 = 0
                        freey1 = 0
                        for j in range(1, 5):                            #检测之后的四个位置
                            tmpx = x + direct[i][0] * j
                            tmpy = y + direct[i][1] * j
                            if(chessboard[tmpx][tmpy] == 1):              #如果这个位置有白棋子
                                flag = 1
                                break
                            elif(chessboard[tmpx][tmpy] == 0):            #如果这个位置位空，则可以放置旗子
                                if(flag == -1):
                                    freex1 = tmpx
                                    freey1 = tmpy
                                    flag = 0
                                if(j >= 3):
                                    isContinue = 1                       #检测是否连续
                            else:
                                black_num += 1
                        if(flag == 1):
                            continue
                        if(x - direct[i][0] < 15 and x - direct[i][0] >= 0 and y - direct[i][1] < 15 and y - direct[i][1] >= 0 and chessboard[x - direct[i][0]][y - direct[i][1]] == 0):   #检测另一边有没有被堵
                            isBlock = 1
                        if(isContinue == -1 and black_num >= 3 and chessboard[x + direct[i][0]*3][y + direct[i][1]*3] == -1):
                            isContinue = 0
                            
                        if(black_num >= 3):
                            mark[freex1][freey1] += 1
                        if(black_num >= 3 and mark[freex1][freey1] >= 2):
                            return 4, freex1, freey1, 0, 0
                        if(num < black_num or (num == black_num and Continue_flag + Block_flag < isContinue + isBlock)):            #评估
                            num = black_num  
                            Continue_flag = isContinue
                            Block_flag = isBlock
                            freex = freex1
                            freey = freey1
        if(num == 4):
            return 4, freex, freey, Continue_flag, Block_flag
        if(num == 3 and (Block_flag > 0 or Continue_flag > 0)):
            return 4, freex, freey, Continue_flag, Block_flag
        return -1, 0, 0, 0, 0


    def attackSearch():
        num1, x1, y1, c, b = defendSearch()
        num = 0                                  #记录最多的连起来的旗子个数
        block_flag = 1                           #是否由一方被堵，没有为1
        freex = 0
        freey = 0
        for x in range(15):
            for y in range(15):
                if chessboard[x][y] == 1:
                    for i in range(8):                       #以(x, y)为起始位置，有8个方向
                        if(x + direct[i][0]*4 >= 15 or x + direct[i][0]*4 < 0 or y + direct[i][1]*4 >= 15 or y + direct[i][1]*4 < 0):    #检测是否超出棋盘界限
                            continue
                        white_num = 1
                        flag = -1
                        freex1 = 0
                        freey1 = 0
                        for j in range(1, 5):
                            tmpx = x + direct[i][0] * j
                            tmpy = y + direct[i][1] * j
                            if(chessboard[tmpx][tmpy] == -1):            #如果这个位置有黑棋子
                                flag = 1
                                break
                            elif(chessboard[tmpx][tmpy] == 0):
                                if(flag == -1):
                                    freex1 = tmpx
                                    freey1 = tmpy
                                    flag = 0
                            else:
                                white_num += 1
                        if(flag == 1):
                            continue
                        if(white_num >= num):
                            if(x - direct[i][0] < 15 and x - direct[i][0] >= 0 and y - direct[i][1] < 15 and y - direct[i][1] >= 0 and chessboard[x - direct[i][0]][y - direct[i][1]] == 0):#检测另一边有没有被堵
                                block_mark = 1
                            else:
                                block_mark = -1
                            
                            if(white_num > num):
                                num = white_num
                                block_flag = block_mark
                                freex = freex1
                                freey = freey1
                            elif(block_mark > 0):
                                block_flag = block_mark
                                freex = freex1
                                freey = freey1
        if(num == 4):
            setPosition(freex, freey, white)
            print("这一步是进攻")
        elif (num1 == 4):
            setPosition(x1, y1, white)
            print("这一步是防守")
        elif(num == 3 and block_flag >= 0):
            setPosition(freex, freey, white)
            print("这一步是进攻")
        elif(num1 == 3 and b > 0):
            setPosition(x1, y1, white)
            print("这一步是防守")
        elif(num >= 3):
            setPosition(freex, freey, white)
            print("这一步是进攻")
        else:                                                          
            if(num1 >= 3):
                print("这一步是防守")
                setPosition(x1, y1, white)
                return
            if(num > 0):
                setPosition(freex, freey, white)
                print("这一步是进攻")
                return
            
            for x in range(15):                                       #若是没有很好的位置，如一开始的时候
                for y in range(15):
                    if(chessboard[x][y] == -1):
                        for i in range(8):
                            print(i)
                            if(x + direct[i][0] < 15 and x + direct[i][0] >= 0 and y + direct[i][1] < 15 and y + direct[i][1] >= 0 and chessboard[x + direct[i][0]][y + direct[i][1]] == 0):
                                setPosition(x + direct[i][0], y + direct[i][1], white)
                                print("这一步是随机放置")
                                return 
                                
    def detect(x, y, mark):                                     #检测是否有一方赢了
            for i in range(4):
                k1 = 1
                k2 = 1
                while(x + direct[2*i][0]*k1 < 15 and x + direct[2*i][0]*k1 >= 0 and y + direct[2*i][1]*k1 < 15
                    and y + direct[2*i][1]*k1 >= 0 and chessboard[x + direct[2*i][0]*k1][y + direct[2*i][1]*k1] == mark):
                    k1 += 1
                while(x + direct[2*i+1][0]*k2 < 15 and x + direct[2*i+1][0]*k2 >= 0 and y + direct[2*i+1][1]*k2 < 15
                    and y + direct[2*i+1][1]*k2 >= 0 and chessboard[x + direct[2*i+1][0]*k2][y + direct[2*i+1][1]*k2] == mark):
                    k2 += 1
                if(k1 + k2 - 2 >= 4):
                    if(mark == 1):
                        print("机器人获胜")
                        result[0] = 1
                    else:
                        print("玩家获胜")
                        result[0] = -1
                    return True
            return False
            
    pos=[0 ,0]
    chessboard = [[0 for i in range(15)] for i in range(15)]
    direct = [[1, 0], [-1, 0], [0, 1], [0, -1], [1, 1], [-1, -1], [-1, 1], [1, -1]]
    result = [0]
    record = [0, 0]            

    pygame.init()
    pygame.display.set_caption('五子棋 汪子凡10185102153')
    screen = pygame.display.set_mode((600, 650))
    screen.fill([230, 213, 128])
    board = pygame.image.load(r'board.bmp').convert()
    black = pygame.image.load(r'black.bmp').convert()  # user is black
    black = pygame.transform.scale(black, (30, 30))
    white = pygame.image.load(r'white.bmp').convert()  # computer is white
    white = pygame.transform.scale(white, (30, 30))
    font = pygame.font.SysFont("arial", 50)
    success = font.render("You Success", False, (0, 0, 225))
    fail = font.render("You Lose", False, (0, 0, 225))
    text1 = font.render("Win: " + str(record[0]) + "   Lose: " + str(record[1]), False,(225, 225, 225))
    text2 = font.render("Click anywhere to continue...",  False,(0, 0, 255))
    screen.blit(text1, (10, 600))
    screen.blit(board, (0, 10))
    pygame.display.update()


    while True:
        for event in pygame.event.get():  # 获得事件
            if event.type == pygame.QUIT:
                pygame.quit()
                exit()
            else:
                pos[0], pos[1] = pygame.mouse.get_pos()  # 获得鼠标的位置
                if event.type == pygame.MOUSEBUTTONDOWN:
                    if(result[0]  != 0):
                        screen.fill([230, 213, 128])
                        screen.blit(board, (0, 10))
                        pygame.display.update()
                        chessboard = [[0 for i in range(15)] for i in range(15)]
                        result[0] = 0
                        text1 = font.render("Win: " + str(record[0]) + "   Lose: " + str(record[1]), False,(225, 225, 225))
                        screen.blit(text1, (10, 600))
                        pygame.display.update()
                        break
                    if(getPosition(pos[0], pos[1], black) == False):
                        break
                    if(result[0] == -1):
                        screen.blit(success,(200, 200))
                        screen.blit(text2,(25, 500))
                        pygame.display.update()
                        record[0] += 1
                        break
                    attackSearch()
                    if(result[0] == 1):
                        screen.blit(fail,(200, 200))
                        screen.blit(text2,(25, 500))
                        pygame.display.update()
                        record[1] += 1
                        break


import pygame
import sys
import time  # 声明 导入需要的模块
from pygame.locals import *
chessboard_b= []
chessboard_w= []
main()
