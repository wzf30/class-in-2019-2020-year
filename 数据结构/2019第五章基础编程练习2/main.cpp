/*
以下实现了所有必须要完成的函数功能：
1.运行程序，首先先选择不同的二叉树读入方式，有前序+附加两个标志位，前序序列+中序序列，后序序列+中序序列三种读入方式，然后输入相应格式；
2.选择不同的遍历方式，有前序，中序，后序，层次四种遍历方式，每种方式都有递归与非递归两种方式实现；
3.选择对这棵二叉树的不同询问，有节点个数，树的高度，是否为满树，是否为完全树，是否为左小右大的查找树5种询问；
4.最后将这棵二叉树看成森林，分别输出没棵树的前序遍历
*/
#include <stdio.h>
#include <stdlib.h>
#define MAXN 1024
char PreOrder[MAXN];//"ABDECGHI" 示例1的前序：“ABCDEFSGHIJKLMNO”
char inOrder[MAXN];//"DBEACHGI" 示例1的中序：“BCDASFHIJGEMNOLK”
char postOrder[MAXN];//"DEBHIGCA"
typedef struct binaryTreeNode//二叉树结构
{
    char data;//节点值
    struct binaryTreeNode *lchild;//左子节点
    struct binaryTreeNode *rchild;//右子节点
//根据需要加入其它字段
}BiNODE; //树的结构
typedef struct LRtagBiTree//左右标志表示树节点
{
    int ltag;
    char data;
    int rtag;
}LRBTree;
LRBTree LRtag[MAXN];

BiNODE* initial()//树节点初始化
{
    BiNODE* node;
    node= (BiNODE*)malloc(sizeof(BiNODE));
    node->lchild = NULL;
    node->rchild = NULL;
//node->level=0;
//node->parent='\0';
//node->data='\0';
return node;
}

BiNODE* BinaryTreeFromLRtag(int length)            //前序+附加两个标志位生成标准链式结构
{
    BiNODE *root = initial(), *p = root, *q; //创建根节点与工作指针p,q
    BiNODE* St[MAXN];                                              //建立栈
    int top = 0;                                                   //top指向栈顶的下一个元素
    for(int i = 0; i < length - 1; i++)
    {
        p->data = LRtag[i].data;
        if(LRtag[i].rtag == 0)                      //若有右子节点，进栈
            St[top++] = p;
        q = initial();
        if(LRtag[i].ltag == 0)
            p->lchild = q;
        else                                     //若没有左子节点，则下一个节点为栈顶元素的右子节点
        {
            p = St[--top];
            p->rchild = q;
        }
        p = q;
    }
    p->data = LRtag[length-1].data;
    return root;
}

//中序遍历的[L1, R1]对应着前序遍历的[L2，R2], 返回所表示树树的根节点
BiNODE* BiTreeFromInPre_recursion(char* inorder, int L1, int R1, char* preorder, int L2, int R2)
{
    if(L1 > R1) return NULL;
    BiNODE *root = initial();
    int i;
    for(i = L1; i <= R1; i++)
    {
        if(inorder[i] == preorder[L2])               //在中序遍历中找到根节点的下标i
            break;
    }
    root->data = inorder[i];
    root->lchild = BiTreeFromInPre_recursion(inorder, L1, i - 1, preorder, L2 + 1, L2 - L1 + i);   //分别对左右子树进行递归
    root->rchild = BiTreeFromInPre_recursion(inorder, i + 1, R1, preorder, L2 - L1 + i + 1, R2);
    return root;

}

BiNODE* BiTreeFromInPre(char* inorder, char* preorder, int length)//根据前序中序生成子树
{
    return BiTreeFromInPre_recursion(inorder, 0, length - 1, preorder, 0, length - 1);
}

BiNODE* BiTreeFromInPost_recursion(char* inorder, int L1, int R1, char *postorder, int L2, int R2)
{
    if(L1 > R1) return NULL;
    BiNODE *root = initial();
    int i;
    for(i = L1; L1 <= R1; i++)
    {
        if(inorder[i] == postorder[R2])
            break;
    }
    root->data = inorder[i];
    root->lchild = BiTreeFromInPost_recursion(inorder, L1, i - 1, postorder, L2, L2 - L1 + i - 1);
    root->rchild = BiTreeFromInPost_recursion(inorder, i + 1, R1, postorder, L2 - L1 + i, R2 - 1);
    return root;
}

BiNODE* BiTreeFromInPost(char* inorder, char* postorder, int length)//根据前序中序生成子树
{
    return BiTreeFromInPost_recursion(inorder, 0, length - 1, postorder, 0, length - 1);
}

void r_preorder(BiNODE* root)               //递归实现前序遍历
{
    if(root != NULL)
    {
        printf("%c ", root->data);
        r_preorder(root->lchild);
        r_preorder(root->rchild);
    }
}

void non_r_preorder(BiNODE* root)               //非递归实现前序遍历
{
    BiNODE* st[MAXN], *p = root;
    int top = 0;
    while(top != 0 || p != NULL)
    {
        while(p != NULL)
        {
            printf("%c ", p->data);
            st[top++] = p;              //节点入栈
            p = p->lchild;              //指向右孩子
        }
        if(top != 0)
        {
            p = st[--top];
            p = p->rchild;
        }
    }
}

void r_inorder(BiNODE* root)             //递归实现中序遍历
{
    if(root != NULL)
    {
        r_inorder(root->lchild);
        printf("%c ", root->data);
        r_inorder(root->rchild);
    }
}

void non_r_inorder(BiNODE* root)             //非递归实现中序遍历
{
    BiNODE* st[MAXN], *p = root;
    int top = 0;
    while(top != 0 || p != NULL)
    {
        while(p != NULL)
        {
            st[top++] = p;              //节点入栈
            p = p->lchild;              //指向右孩子
        }
        if(top != 0)
        {
            p = st[--top];                    //取出栈顶元素
            printf("%c ", p->data);
            p = p->rchild;
        }
    }
}

void r_postorder(BiNODE* root)                //递归实现后序遍历
{
    if(root != NULL)
    {
        r_postorder(root->lchild);
        r_postorder(root->rchild);
        printf("%c ", root->data);
    }
}

void non_r_postorder(BiNODE* root)           //非递归后序遍历
{
    BiNODE *s[MAXN], *p;
    int mark[MAXN], top = -1;           //mark为0表示此节点没有展开子节点，1表示已经展开
    s[++top] = root;
    mark[top] = 0;                        //根节点入栈，top指向栈顶
    while(top >= 0)
    {
        p = s[top];
        if(mark[top] == 0 && (p->lchild || p->rchild))  //若是这个节点有子孩子，且子节点没有展开，展开子节点
        {
            mark[top] = 1;
            if(p->rchild)                                    //将存在的子节点入栈
            {
                s[++top] = p->rchild;
                mark[top] = 0;
            }
            if(p->lchild)
            {
                s[++top] = p->lchild;
                mark[top] = 0;
            }
        }
        if((!s[top]->lchild && !s[top]->rchild) || mark[top] == 1) //若是栈顶元素的子节点已经展开或者没有子节点
            printf("%c ", s[top--]->data);
    }
}

void levelorder(BiNODE* root)
{
    BiNODE* q[MAXN], *p;
    q[0] = root;
    int head = 0, tail = 1;        //head指向队首，head指向队尾下一个
    while(head != tail)
    {
        p = q[head++];
        if(p->lchild) q[tail++] = p->lchild;
        if(p->rchild) q[tail++] = p->rchild;
        printf("%c ", p->data);
    }
}

int get_node(BiNODE* root)                   //获得一棵树的节点
{
    if(root == NULL) return 0;
    return 1 + get_node(root->lchild) + get_node(root->rchild);
}

int get_height(BiNODE* root)                  //获得一棵树的高度，高度从1开始计数，最后答案要减1
{
    if(root == NULL) return 0;
    return 1 + (get_height(root->lchild) > get_height(root->rchild) ? get_height(root->lchild) : get_height(root->rchild));
}

int judge_fulltree(BiNODE* root)                        //判断一棵树是否为满树
{
    int height = get_height(root) - 1, node = get_node(root);
    if((1<<(height + 1)) -1 == node) return 1;
    else return -1;
}

int judge_completetree(BiNODE* root)                      //判断一棵树是否为完全二叉树
{
    int flag = 0;
    BiNODE* q[MAXN], *p;
    q[0] = root;
    int head = 0, tail = 1;             //head指向队首，head指向队尾下一个;
    while(head != tail)
    {
        p = q[head++];
        if(flag == 1 && (p->lchild || p->rchild)) //已经标记好之后又出现了有左孩子或者右孩子的节点
        {
            flag = -1;
            break;
        }
        if(!p->lchild || !p->rchild) //遇到第一个没有左儿子或者右儿子的节点，设置标志位
            flag = 1;
        if(p->lchild) q[tail++] = p->lchild;
        if(p->rchild) q[tail++] = p->rchild;
    }
    if(flag == -1) return 0;
    return 1;
}

int judge_findtree(BiNODE* root)                     //判断一棵树是否为查找树
{
    char ans[MAXN];                      //记录中序遍历的结果
    int cnt = 0;
    BiNODE* st[MAXN], *p = root;         //进行中序遍历
    int top = 0;
    while(top != 0 || p != NULL)
    {
        while(p != NULL)
        {
            st[top++] = p;              //节点入栈
            p = p->lchild;              //指向右孩子
        }
        if(top != 0)
        {
            p = st[--top];                    //取出栈顶元素
            ans[cnt++] = p->data;
            p = p->rchild;
        }
    }
    for(int i = 1; i < cnt; i++)
    {
        if(ans[i] <= ans[i - 1])
            return 0;
    }
    return 1;
}

void binary_tree_to_forest(BiNODE *root)
{
    int cnt = 0;
    printf("第%d棵树的前序遍历为：%c ", ++cnt, root->data);
    non_r_preorder(root->lchild);
    putchar('\n');
    while(root->rchild)
    {
        root = root->rchild;
        printf("第%d棵树的前序遍历为：%c ", ++cnt, root->data);
        non_r_preorder(root->lchild);
        putchar('\n');
    }
}
int main()
{
    int cas, n;
    BiNODE* root;
    printf("请选择二叉树的读入方式：\n1. 前序+附加两个标志位\n2. 前序序列+中序序列\n3. 后序序列+中序序列\n");
    scanf("%d", &cas);
    if(cas == 1)
    {
        printf("请在第一行输入树的节点数n，再输出n行节点的信息\n");
        scanf("%d", &n);
        for(int i = 0; i < n; i++)
            scanf("%d %c %d", &LRtag[i].ltag, &LRtag[i].data, &LRtag[i].rtag);
        root =  BinaryTreeFromLRtag(n);
    }
    else if(cas == 2)
    {
        printf("请在第一行输入树的节点数n，第二行输入前序序列，第三行输入中序序列\n");
        scanf("%d", &n);
        for(int i = 0; i < n; i++)
            scanf(" %c", &PreOrder[i]);
        for(int i = 0; i < n; i++)
            scanf(" %c", &inOrder[i]);
        root = BiTreeFromInPre(inOrder, PreOrder, n);
    }
    else
    {
        printf("请在第一行输入树的节点数n,第二行输入中序序列，第三行输入后序序列\n");
        scanf("%d", &n);
        for(int i = 0; i < n; i++)
            scanf(" %c", &inOrder[i]);
        for(int i = 0; i < n; i++)
            scanf(" %c", &postOrder[i]);
        root = BiTreeFromInPost(inOrder, postOrder, n);
    }

    while(1)
    {
        printf("\n请选择相应的遍历方式：1.前序遍历 2.中序遍历 3.后序遍历 4.层次遍历\n输入其他数字退出\n");
        scanf("%d", &cas);
        if(cas == 1)
        {
            printf("递归前序遍历：   ");
            r_preorder(root); printf("\n非递归前序遍历： ");
            non_r_preorder(root); putchar('\n');
        }
        else if(cas == 2)
        {
            printf("递归中序遍历：   ");
            r_inorder(root); printf("\n非递归中序遍历： ");
            non_r_inorder(root); putchar('\n');
        }
        else if(cas == 3)
        {
            printf("递归后序遍历：   ");
            r_postorder(root); printf("\n非递归后序遍历： ");
            non_r_postorder(root); putchar('\n');
        }
        else if(cas == 4)
        {
            printf("层次遍历： ");
            levelorder(root); putchar('\n');
        }
        else break;
    }

    while(1)
    {
        printf("\n请选择对树的询问操作：\n1.树的节点数 2.树的高度\n 3.是否为满树 4.是否为完全二叉树\n");
        printf("5.判断一棵二叉树是否左子树上的结点的值都小于根，右子树上的结点的值都大于根\n输入其他数字退出\n");
        scanf("%d", &cas);
        if(cas == 1)
            printf("这棵树的节点数为：%d\n", get_node(root));
        else if(cas == 2)
            printf("这棵树的高度为：%d\n", get_height(root) - 1);
        else if(cas == 3)
        {
            if(judge_fulltree(root) > 0) printf("这棵树是满树\n");
            else printf("这棵树不是满树\n");
        }
        else if(cas == 4)
        {
            if(judge_completetree(root) > 0) printf("这棵树是完全二叉树\n");
            else printf("这棵树不是完全二叉树\n");
        }
        else if(cas == 5)
        {
            if(judge_findtree(root) > 0) printf("是一棵左小右大的查找树\n");
            else printf("不是一棵左小右大的查找树\n");
        }
        else break;
    }

    printf("这棵二叉树可以转化为森林：\n");
    binary_tree_to_forest(root);
}


/*
13
0 A 0
0 B 0
1 C 1
0 D 0
1 E 1
1 F 0
1 G 1
0 H 1
0 I 0
0 J 0
1 K 1
1 L 1
1 M 1

13
A B C D E F G H I J K L M
C B E D F G A K J L I M H

13
C B E D F G A K J L I M H
C E G F D B K L J M I H A
*/

/*
7
0 A 0
0 B 0
1 D 1
1 E 1
0 C 0
1 F 1
1 G 1

7
A B D E C F G
D B E A F C G

7
D B E A F C G
D E B F G C A
*/

/*
6
0 D 0
0 B 0
1 A 1
1 C 1
0 F 1
1 E 1

6
D B A C F E
A B C D E F

6
A B C D E F
A C B E F D
*/
