#include <stdio.h>
#include <stdlib.h>
#define MAXN 1024

int PreOrder[MAXN];//"ABDECGHI" 示例1的前序：“ABCDEFSGHIJKLMNO”
int inOrder[MAXN];//"DBEACHGI" 示例1的中序：“BCDASFHIJGEMNOLK”

typedef struct binaryTreeNode//二叉树结构
{
    int data;//节点值
    struct binaryTreeNode *lchild;//左子节点
    struct binaryTreeNode *rchild;//右子节点
    int balance_degree;
//根据需要加入其它字段
}BiNODE; //树的结构

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

//中序遍历的[L1, R1]对应着前序遍历的[L2，R2], 返回所表示树树的根节点
BiNODE* BiTreeFromInPre_recursion(int* inorder, int L1, int R1, int* preorder, int L2, int R2)
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

BiNODE* BiTreeFromInPre(int* inorder, int* preorder, int length)//根据前序中序生成子树
{
    return BiTreeFromInPre_recursion(inorder, 0, length - 1, preorder, 0, length - 1);
}

int get_height(BiNODE* root)                  //获得一棵树的高度，高度从1开始计数，最后答案要减1
{
    if(root == NULL) return 0;
    return 1 + (get_height(root->lchild) > get_height(root->rchild) ? get_height(root->lchild) : get_height(root->rchild));
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

void set_balance_degree(BiNODE* root)
{
    if(root == NULL) return;
    root->balance_degree = get_height(root->rchild) - get_height(root->lchild);
    set_balance_degree(root->lchild);
    set_balance_degree(root->rchild);
}
// **************************************以上为上一次作业代码(建立二叉树)*******************************

bool judge_balancetree(BiNODE* root)
{
    if(root == NULL) return true;
    if(abs(root->balance_degree) > 1) return false;
    return judge_balancetree(root->lchild) && judge_balancetree(root->rchild);
}

int trace(BiNODE* root)   //通过仅“跟踪”到叶子结点的一条路径，而不查看树中所有的结点
{
    BiNODE* p = root;
    int cnt = -1;
    while(p)
    {
        if(p->balance_degree >= 0) p = p->rchild;
        else p = p->lchild;
        cnt++;
    }
    return cnt;
}

int main()
{
    int n;
    BiNODE* root;
    printf("请在第一行输入树的节点数n，第二行输入前序序列，第三行输入中序序列\n");
    scanf("%d", &n);
    for(int i = 0; i < n; i++)
        scanf(" %d", &PreOrder[i]);
    for(int i = 0; i < n; i++)
        scanf(" %d", &inOrder[i]);
    root = BiTreeFromInPre(inOrder, PreOrder, n);

    set_balance_degree(root);
//*******************************以上为上一次作业的代码************************************************

    if(judge_findtree(root)) printf("\n这棵树是查找树\n");
    else printf("这棵树不是查找树\n");
    if(judge_balancetree(root)) printf("这棵树是平衡树\n");
    else printf("这棵树不是平衡树\n");

    printf("这棵树的高度为：%d\n", trace(root));   //用平衡度来查找这棵树的高度
}


/*
7
1 2 4 7 5 3 6
7 4 2 5 1 6 3        平衡树，不是查找树

7
4 2 1 3 5 6 7
1 2 3 4 5 6 7        查找树，不是平衡树

7
4 2 1 3 6 5 7
1 2 3 4 5 6 7
*/
