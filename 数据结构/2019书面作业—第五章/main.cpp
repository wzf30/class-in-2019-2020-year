/*功能介绍：
  1.首先可以通过输入端选择输入方式，1为括号表示法，2位双亲表示法，3位前序 + 层次表示法, 然后输入相应的树的表达方式；
  2.将输入数据读入到inputTree数组(通过convert函数),inputTree数组存有data与parent两个成员，并得到结点数n与最高层次m；
  3.建树，将inputTree数组转化为链式表达（通过creatTreeFromParent函数）,每个结点含有data, child的指针数组,指向parent的指针以及孩子个数num与层次lev；
  4.通过相应的函数进行前序,后序与层次遍历(前序与后序有递归与非递归两种方法);
  5.输出这棵树的结点总个数，叶子结点的情况(通过get_leaf_node函数)与这棵树的最高层次(通过get_level函数)
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
// 树的次树：最大MAXM次，树最多有MAXN个结点
#define MAXN 100
#define MAXM 10

typedef struct nodeParent
{
    char data;
    int parent;
}NODE_PARENT;

typedef struct linkNode
{
    char data;                           //节点数据
    struct linkNode *child[MAXM];        //子节点的地址
    struct linkNode *parent;             //父节点的地址
    int lev;                             //节点的层次(根节点为0）
    int num;                             //子节点的个数
}LINK_NODE;

LINK_NODE *addr_NODE[MAXN];                 //储存链式存储一棵树的每个结点的指针

int AddChild(LINK_NODE *parent,LINK_NODE *child,int m)     //m次树中，parent下插入一个新的子结点child
{
    if(parent == NULL || child == NULL || parent -> num == m) return -1;       //如果插入子节点失败
    parent -> child[parent -> num] = child;
    parent -> num++;
    child -> lev = parent -> lev + 1;
    child -> parent = parent;
    return 1;
}

LINK_NODE *Node_initail(int m)  //m次树结点初始化，根据结点类型，完成结点的初始化操作
{
    LINK_NODE *a = (LINK_NODE*)malloc(sizeof(LINK_NODE));
    for(int i = 0; i < m; i++)
        a -> child[i] = NULL;
    a -> num = 0;
    return a;
}

LINK_NODE *creatTreeFromParent(NODE_PARENT inputTree[],int m,int n)    //根据inputTree建树成链式存储，用一个数组addr_NODE来记录每个结点的指针，
{                                                                      //m表示次数，n表示节点个数
    int i;
    LINK_NODE *root, *p, *q;

    if(n < 1)  return NULL;                       //若为空树
    root = Node_initail(m);                       //建立根节点
    root -> data = inputTree[0].data;
    root -> lev = 0;
    addr_NODE[0] = root;

    for(i = 1; i < n; i++)
    {
        q = Node_initail(m);
        q -> data = inputTree[i].data;
        addr_NODE[i] = q;
        p = addr_NODE[inputTree[i].parent];          //找到子节点q的父节点p
        if(AddChild(p, q, m) < 0)
            printf("建树失败！\n");
    }

    return root;
}

void r_preorder(LINK_NODE *t, int m) //递归前序遍历，t:根，m次数
{
    int i;
    if(t !=  NULL)
    {
        printf("%c ", t -> data);
        for(i = 0; i < t -> num; i++)
            r_preorder(t -> child[i], m);
    }
}

void r_postorder(LINK_NODE *t, int m)       //递归后序遍历，t:根，m次数
{
    int i;
    if(t != NULL)
    {
        for(i = 0; i < t -> num; i++)
            r_postorder(t -> child[i], m);
        printf("%c ", t -> data);
    }
}

void hierachicalorder(LINK_NODE *t, int m)         //非递归层次遍历
{
    LINK_NODE *q[MAXN], *p;
    int head = 0, tail = 1, i;
    q[0] = t;
    while(head < tail)
    {
        p = q[head++];
        printf("%c ", p -> data);
        for(i = 0; i < p -> num; i++)
            q[tail++] = p -> child[i];
    }
}

void preorderNoRecursion(LINK_NODE *t, int m)   //递归前序遍历，t:根，m次数
{
    LINK_NODE* s[MAXN];
    int top, i;
    if(t == NULL) return;
    s[0] = t;
    top = 1;                 //根节点入栈，top指向栈顶的下一个位置
    while(top > 0)
    {
        t = s[--top];
        printf("%c ", t -> data);
        for(i = t -> num - 1; i >= 0; i--)
            s[top++] = t -> child[i];
    }
}

void postorderNoRecursion(LINK_NODE *t, int m)    //非递归后序遍历，t:根，m次数
{
    LINK_NODE *s[MAXN], *p;
    int mark[MAXN], top = -1, j;
    s[++top] = t;
    mark[top] = 0;                        //根节点入栈，top指向栈顶
    while(top >= 0)
    {
        p = s[top];
        if(mark[top] == 0 && p -> num > 0)
        {
            mark[top] = 1;
            for(j = p -> num - 1; j >= 0; j--)
            {
                s[++top] = p -> child[j];
                mark[top] = 0;
            }
        }
        if(s[top] -> num == 0 || mark[top] == 1)
            printf("%c ", s[top--] -> data);
    }
}

void convert1(NODE_PARENT inputTree[], int *a, int *b)       //处理括号表示法 a来记录最大次数，b来记录节点数
{

    int s[MAXN], num[MAXN], top = -1, m = 0, n = 0;                 //top记录当前栈顶元素,栈顶元素为父亲节点
    char ch;                                       //num记录当前父节点的子节点个数
    scanf(" %c", &ch);
    inputTree[n].data = ch; inputTree[n++].parent = -1;

    while((ch = getchar()) != '\n')
    {
        if(isalpha(ch))
        {
            inputTree[n].data = ch;
            inputTree[n++].parent = s[top];
            num[top]++;
        }
        else
        {
            switch(ch)
            {
                case '(': s[++top] = n - 1;
                          num[top] = 0;
                          break;
                case ')': m = m > num[top] ? m : num[top];
                          top--;
            }
        }
    }
    *a = m; *b = n;
}

void convert2(NODE_PARENT inputTree[], int *a, int *b)       //处理双亲表示法 a来记录最大次数，b来记录节点数
{
    scanf("%d", b);
    int num[MAXN];
    int mx = 0, i;
    for(i = 0; i < *b; i++)
    {
        scanf("%d %c", &inputTree[i].parent, &inputTree[i].data);
        num[i] = 0;
        if(inputTree[i].parent >= 0) num[inputTree[i].parent]++;          //记录父节点的孩子数
    }
    for(int i = 0; i < *b; i++) mx = mx > num[i] ? mx : num[i];
    *a = mx;
}

void convert3(NODE_PARENT inputTree[], int *a, int *b)       //处理前序+层号表示法 a来记录最大次数，b来记录节点数
{
    scanf("%d", b);
    int i, tmp, s1[MAXN], s2[MAXN], num[MAXN], top = 0, mx = 0; //s1记录上个节点的序号，s2记录层次，num记录父节点的孩子个数
    scanf("%d %c", &tmp, &inputTree[0].data);                  //top指向栈顶
    inputTree[0].parent = -1;
    s1[top] = 0;
    s2[top] = tmp;
    num[top] = 0;
    for(i = 1; i < *b; i++)
    {
        scanf("%d %c", &tmp, &inputTree[i].data);
        while(s2[top] >= tmp)                                    //如果上个节点的层次不比当前节点低，则出栈直到找到层次比当前低的节点
        {
            mx = mx > num[top] ? mx : num[top];
            top--;
        }
        num[top]++;
        inputTree[i].parent = s1[top];
        s1[++top] = i;
        s2[top] = tmp;
        num[top] = 0;
    }
    while(top >= 0)
    {
        mx = mx > num[top] ? mx : num[top];
        top--;
    }
    *a = mx;
}

void get_leaf_node(int m, int n)            //获得叶子结点的信息
{
    int i, cnt = 0;
    printf("这棵%d次树的叶子结点为： ", m);
    for(i = 0; i < n; i++)
    {
        if(addr_NODE[i] -> num == 0)
        {
            printf("%c ", addr_NODE[i] -> data);
            cnt++;
        }
    }
    printf("\n叶子结点的总个数为%d\n", cnt);
}

void get_mx_level(int m, int n)                  //获得最大层次的信息
{
    int i, mx = -1;
    for(i = 0; i < n; i++)
        mx = mx > addr_NODE[i] -> lev ? mx : addr_NODE[i] -> lev;
    printf("这棵%d次树的最大层次为%d\n", m, mx);
}
int main()
{
    int m, n, tag;
    LINK_NODE *root;
    NODE_PARENT inputTree[MAXN];            //将输入数据（每一个节点的data与parent）读入到inputTree数组，建树
    while(true)
    {
        printf("请先选择输入的类型：1.括号表示 2.双亲表示 3.前序 + 层号表示\n输入其他数字跳出循环\n");
        scanf("%d", &tag);
        printf("请输入：\n");                     //第2与第3种表示法要首先给定节点个数
        switch(tag)
        {
            case 1: convert1(inputTree, &m, &n); break;
            case 2: convert2(inputTree, &m, &n); break;
            case 3: convert3(inputTree, &m, &n); break;
            default: tag = -1;
        }
        if(tag == -1) break;

        root = creatTreeFromParent(inputTree, m, n);            //根节点的parent为-1

        printf("前序遍历(递归):   ");     r_preorder(root, m);
        printf("\n前序遍历(非递归): ");    preorderNoRecursion(root, m);
        printf("\n\n后序遍历(递归):   ");  r_postorder(root, m);
        printf("\n后序遍历(非递归): ");      postorderNoRecursion(root, m);
        printf("\n\n层次遍历: ");       hierachicalorder(root, m);
        printf("\n\n");

        printf("这棵%d次树的节点总数为%d\n", m, n);
        get_leaf_node(m, n);
        get_mx_level(m, n);
        putchar('\n');
    }
    return 1;
}
