#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <math.h>
#include <vector>
#include <algorithm>
const double INF = 10000.0;
using namespace std;

double cost[1000][1000];
double info[1000][2];
int N = 1000;

double CalLen(int a , int b)
{
    return sqrt((info[a][0] - info[b][0]) * (info[a][0] - info[b][0]) + (info[a][1] - info[b][1]) * (info[a][1] - info[b][1]));
}

void readTxt_info(string file)
{
    ifstream infile;
    infile.open(file.data()); //将文件流对象与文件连接起来
    assert(infile.is_open()); //若失败,则输出错误消息,并终止程序运行

    int c;
    double x , y;
    while (!infile.eof())
    {
        infile >> c;
        infile >> info[c][0];
        infile >> info[c][1];
    }
    infile.close(); //关闭文件输入流
}

void readText_cost(string file)
{
    ifstream infile;
    infile.open(file.data()); //将文件流对象与文件连接起来
    assert(infile.is_open()); //若失败,则输出错误消息,并终止程序运行

    int a , b;
    while (!infile.eof())
    {
        infile >> a >> b;
        cost[a][b] = CalLen(a, b);
        cost[b][a] = cost[a][b];
        //cout << a << " " << b << " " << cost[a][b] << endl;
    }
    infile.close(); //关闭文件输入流
}

void initial() //最后要改一下，改成两个参数file1和file2
{
    readTxt_info("city.txt");
    /*for(int i = 0; i < 1000; i ++)
        cout << info[i][0] << " " << info[i][1] << endl;
    */
    //memset(cost, INF, sizeof(cost));
    for (int i = 0; i < 1000; i++)
    {
        for (int j = 0; j < 1000; j++)
        {
            cost[i][j] = INF;
            //cout << cost[i][j] << " ";
        }

        //cout << endl;
    }
    /*for (int i = 0; i < 1000; i++)
        cout << dist[i][0] << " " << dist[i][1] << endl;
    */
    readText_cost("link.txt");
}

int Dijkstra(int v, int u) //v到其他点的最短距离
{
    double pre[1000], dist[1000], s[1000];
    int flag = 0;
    for (int i = 0; i < N; i++)
    {
        dist[i] = cost[v][i];
        s[i] = 0;
        if (dist[i] < INF)
            pre[i] = v;
        else
            pre[i] = 0;
    }
    s[v] = 1;
    pre[v] = -1;
    int min, k;
    for (int i = 0; i < N; i++)
    {
        min = INF;
        k = -1;
        for (int j = 0; j < N; j++)
            if (s[j] == 0)
                if (dist[j] != 0 && dist[j] < min)
                {

                    min = dist[j];
                    k = j;
                }
        if (k == -1)
        {
            flag = -1;
            break;
        }
        if (k == u)
            break;
        s[k] = 1;
        for (int j = 0; j < N; j++)
            if (s[j] == 0 && cost[k][j] < INF)
                if (dist[k] + cost[k][j] < dist[j])
                {
                    dist[j] = dist[k] + cost[k][j];
                    pre[j] = k;
                }
    }
    if(flag != -1)
    {
        cout << "length:" << dist[u];
        cout << ",path:" << u << "-";
        int u_ = u;
        while (pre[u_] != v)
        {
            cout << pre[u_] << '-';
            u_ = pre[u_];
        }
        cout << v << endl;
    }
    else
        cout << "length: INF, path: null" << endl;
}

struct node
{
    int val;
    double g;
    double h;
    int pre;
    vector<int> sub;
    int isOpen;
};

int cmp(const node* a , const node* b)
{
    return a -> g + a -> h < b -> g +  b -> h;
}

void AStar(int v , int u)
{
    vector<node *> Open;
    node NODE[1000];
    for(int i = 0; i < 1000; i ++)
    {
        NODE[i].val = i;
        NODE[i].pre = -1;
        NODE[i].h = CalLen(i, u); //
    }
    //NODE[v].h = cost[v][u];
    NODE[v].pre = -2;
    Open.push_back(&NODE[v]);
    while(!Open.empty())
    {
        node* n = Open[0];
        Open.erase(Open.begin());

        n -> isOpen = 1;
        if(n->val == u) break;     //
        for(int i = 0; i < 1000; i ++)
        {
            if (cost[n->val][i] < INF && i != n->val)
            {
                if (NODE[i].isOpen != 1)
                {
                    if (NODE[i].pre == -1) //新拓展的结点
                    {
                        n->sub.push_back(i);
                        NODE[i].pre = n->val;
                        NODE[i].g = cost[n->val][i] + NODE[n->val].g;
                        //NODE[i].h = cost[i][u];
                        Open.push_back(&NODE[i]);
                    }
                    else //在Open表中还未被拓展
                    {
                        if (NODE[i].g + NODE[i].h > cost[n->val][i] + NODE[n->val].g + NODE[i].h)
                        {
                            NODE[i].g = cost[n->val][i] + NODE[n->val].g;
                            //NODE[i].h = cost[i][u];
                            n->sub.push_back(i);
                            vector<int>::iterator it;
                            for (it = NODE[NODE[i].pre].sub.begin(); it != NODE[NODE[i].pre].sub.end(); ++it)
                                if (*it == i)
                                {
                                    NODE[NODE[i].pre].sub.erase(it);
                                    break;
                                }
                            NODE[i].pre = n->val;
                        }
                    }
                }
                /*else //已经被拓展
                {
                    if (NODE[i].g + NODE[i].h > cost[n->val][i] + NODE[n->val].g + cost[i][u])
                    {
                        NODE[i].g = cost[n->val][i] + NODE[n->val].g;
                        NODE[i].h = cost[i][u];
                        n->sub.push_back(i);
                        vector<int>::iterator it;
                        for (it = NODE[NODE[i].pre].sub.begin(); it != NODE[NODE[i].pre].sub.end(); ++it)
                            if (*it == i)
                            {
                                NODE[NODE[i].pre].sub.erase(it);
                                break;
                            }
                        NODE[i].pre = n->val;
                        Open.push_back(&NODE[i]);
                    }
                }*/
            }
        }
        sort(Open.begin(),Open.end(),cmp);
    }
    cout << "length:" << NODE[u].g;
    cout << ", path:";
    int q = u;
    while(q != v)
    {
        cout << q << "-";
        q = NODE[q].pre;
    }
    cout << v << endl;
}


int main()
{
    initial();
    Dijkstra(123,321);
    AStar(123,321);
    Dijkstra(1, 856);
    AStar(1, 856);
    Dijkstra(2, 856);
    AStar(2, 856);
    Dijkstra(100, 856);
    AStar(100, 856);
}
