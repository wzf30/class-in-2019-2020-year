#include <stdio.h>
#include <stdlib.h>
#define MAXN 1024

int PreOrder[MAXN];//"ABDECGHI" ʾ��1��ǰ�򣺡�ABCDEFSGHIJKLMNO��
int inOrder[MAXN];//"DBEACHGI" ʾ��1�����򣺡�BCDASFHIJGEMNOLK��

typedef struct binaryTreeNode//�������ṹ
{
    int data;//�ڵ�ֵ
    struct binaryTreeNode *lchild;//���ӽڵ�
    struct binaryTreeNode *rchild;//���ӽڵ�
    int balance_degree;
//������Ҫ���������ֶ�
}BiNODE; //���Ľṹ

BiNODE* initial()//���ڵ��ʼ��
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

//���������[L1, R1]��Ӧ��ǰ�������[L2��R2], ��������ʾ�����ĸ��ڵ�
BiNODE* BiTreeFromInPre_recursion(int* inorder, int L1, int R1, int* preorder, int L2, int R2)
{
    if(L1 > R1) return NULL;
    BiNODE *root = initial();
    int i;
    for(i = L1; i <= R1; i++)
    {
        if(inorder[i] == preorder[L2])               //������������ҵ����ڵ���±�i
            break;
    }
    root->data = inorder[i];
    root->lchild = BiTreeFromInPre_recursion(inorder, L1, i - 1, preorder, L2 + 1, L2 - L1 + i);   //�ֱ�������������еݹ�
    root->rchild = BiTreeFromInPre_recursion(inorder, i + 1, R1, preorder, L2 - L1 + i + 1, R2);
    return root;

}

BiNODE* BiTreeFromInPre(int* inorder, int* preorder, int length)//����ǰ��������������
{
    return BiTreeFromInPre_recursion(inorder, 0, length - 1, preorder, 0, length - 1);
}

int get_height(BiNODE* root)                  //���һ�����ĸ߶ȣ��߶ȴ�1��ʼ����������Ҫ��1
{
    if(root == NULL) return 0;
    return 1 + (get_height(root->lchild) > get_height(root->rchild) ? get_height(root->lchild) : get_height(root->rchild));
}

int judge_findtree(BiNODE* root)                     //�ж�һ�����Ƿ�Ϊ������
{
    char ans[MAXN];                      //��¼��������Ľ��
    int cnt = 0;
    BiNODE* st[MAXN], *p = root;         //�����������
    int top = 0;
    while(top != 0 || p != NULL)
    {
        while(p != NULL)
        {
            st[top++] = p;              //�ڵ���ջ
            p = p->lchild;              //ָ���Һ���
        }
        if(top != 0)
        {
            p = st[--top];                    //ȡ��ջ��Ԫ��
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
// **************************************����Ϊ��һ����ҵ����(����������)*******************************

bool judge_balancetree(BiNODE* root)
{
    if(root == NULL) return true;
    if(abs(root->balance_degree) > 1) return false;
    return judge_balancetree(root->lchild) && judge_balancetree(root->rchild);
}

int trace(BiNODE* root)   //ͨ���������١���Ҷ�ӽ���һ��·���������鿴�������еĽ��
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
    printf("���ڵ�һ���������Ľڵ���n���ڶ�������ǰ�����У�������������������\n");
    scanf("%d", &n);
    for(int i = 0; i < n; i++)
        scanf(" %d", &PreOrder[i]);
    for(int i = 0; i < n; i++)
        scanf(" %d", &inOrder[i]);
    root = BiTreeFromInPre(inOrder, PreOrder, n);

    set_balance_degree(root);
//*******************************����Ϊ��һ����ҵ�Ĵ���************************************************

    if(judge_findtree(root)) printf("\n������ǲ�����\n");
    else printf("��������ǲ�����\n");
    if(judge_balancetree(root)) printf("�������ƽ����\n");
    else printf("���������ƽ����\n");

    printf("������ĸ߶�Ϊ��%d\n", trace(root));   //��ƽ���������������ĸ߶�
}


/*
7
1 2 4 7 5 3 6
7 4 2 5 1 6 3        ƽ���������ǲ�����

7
4 2 1 3 5 6 7
1 2 3 4 5 6 7        ������������ƽ����

7
4 2 1 3 6 5 7
1 2 3 4 5 6 7
*/
