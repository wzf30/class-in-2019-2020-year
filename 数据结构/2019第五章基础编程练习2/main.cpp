/*
����ʵ�������б���Ҫ��ɵĺ������ܣ�
1.���г���������ѡ��ͬ�Ķ��������뷽ʽ����ǰ��+����������־λ��ǰ������+�������У���������+�����������ֶ��뷽ʽ��Ȼ��������Ӧ��ʽ��
2.ѡ��ͬ�ı�����ʽ����ǰ�����򣬺��򣬲�����ֱ�����ʽ��ÿ�ַ�ʽ���еݹ���ǵݹ����ַ�ʽʵ�֣�
3.ѡ�����ö������Ĳ�ͬѯ�ʣ��нڵ���������ĸ߶ȣ��Ƿ�Ϊ�������Ƿ�Ϊ��ȫ�����Ƿ�Ϊ��С�Ҵ�Ĳ�����5��ѯ�ʣ�
4.�����ö���������ɭ�֣��ֱ����û������ǰ�����
*/
#include <stdio.h>
#include <stdlib.h>
#define MAXN 1024
char PreOrder[MAXN];//"ABDECGHI" ʾ��1��ǰ�򣺡�ABCDEFSGHIJKLMNO��
char inOrder[MAXN];//"DBEACHGI" ʾ��1�����򣺡�BCDASFHIJGEMNOLK��
char postOrder[MAXN];//"DEBHIGCA"
typedef struct binaryTreeNode//�������ṹ
{
    char data;//�ڵ�ֵ
    struct binaryTreeNode *lchild;//���ӽڵ�
    struct binaryTreeNode *rchild;//���ӽڵ�
//������Ҫ���������ֶ�
}BiNODE; //���Ľṹ
typedef struct LRtagBiTree//���ұ�־��ʾ���ڵ�
{
    int ltag;
    char data;
    int rtag;
}LRBTree;
LRBTree LRtag[MAXN];

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

BiNODE* BinaryTreeFromLRtag(int length)            //ǰ��+����������־λ���ɱ�׼��ʽ�ṹ
{
    BiNODE *root = initial(), *p = root, *q; //�������ڵ��빤��ָ��p,q
    BiNODE* St[MAXN];                                              //����ջ
    int top = 0;                                                   //topָ��ջ������һ��Ԫ��
    for(int i = 0; i < length - 1; i++)
    {
        p->data = LRtag[i].data;
        if(LRtag[i].rtag == 0)                      //�������ӽڵ㣬��ջ
            St[top++] = p;
        q = initial();
        if(LRtag[i].ltag == 0)
            p->lchild = q;
        else                                     //��û�����ӽڵ㣬����һ���ڵ�Ϊջ��Ԫ�ص����ӽڵ�
        {
            p = St[--top];
            p->rchild = q;
        }
        p = q;
    }
    p->data = LRtag[length-1].data;
    return root;
}

//���������[L1, R1]��Ӧ��ǰ�������[L2��R2], ��������ʾ�����ĸ��ڵ�
BiNODE* BiTreeFromInPre_recursion(char* inorder, int L1, int R1, char* preorder, int L2, int R2)
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

BiNODE* BiTreeFromInPre(char* inorder, char* preorder, int length)//����ǰ��������������
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

BiNODE* BiTreeFromInPost(char* inorder, char* postorder, int length)//����ǰ��������������
{
    return BiTreeFromInPost_recursion(inorder, 0, length - 1, postorder, 0, length - 1);
}

void r_preorder(BiNODE* root)               //�ݹ�ʵ��ǰ�����
{
    if(root != NULL)
    {
        printf("%c ", root->data);
        r_preorder(root->lchild);
        r_preorder(root->rchild);
    }
}

void non_r_preorder(BiNODE* root)               //�ǵݹ�ʵ��ǰ�����
{
    BiNODE* st[MAXN], *p = root;
    int top = 0;
    while(top != 0 || p != NULL)
    {
        while(p != NULL)
        {
            printf("%c ", p->data);
            st[top++] = p;              //�ڵ���ջ
            p = p->lchild;              //ָ���Һ���
        }
        if(top != 0)
        {
            p = st[--top];
            p = p->rchild;
        }
    }
}

void r_inorder(BiNODE* root)             //�ݹ�ʵ���������
{
    if(root != NULL)
    {
        r_inorder(root->lchild);
        printf("%c ", root->data);
        r_inorder(root->rchild);
    }
}

void non_r_inorder(BiNODE* root)             //�ǵݹ�ʵ���������
{
    BiNODE* st[MAXN], *p = root;
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
            printf("%c ", p->data);
            p = p->rchild;
        }
    }
}

void r_postorder(BiNODE* root)                //�ݹ�ʵ�ֺ������
{
    if(root != NULL)
    {
        r_postorder(root->lchild);
        r_postorder(root->rchild);
        printf("%c ", root->data);
    }
}

void non_r_postorder(BiNODE* root)           //�ǵݹ�������
{
    BiNODE *s[MAXN], *p;
    int mark[MAXN], top = -1;           //markΪ0��ʾ�˽ڵ�û��չ���ӽڵ㣬1��ʾ�Ѿ�չ��
    s[++top] = root;
    mark[top] = 0;                        //���ڵ���ջ��topָ��ջ��
    while(top >= 0)
    {
        p = s[top];
        if(mark[top] == 0 && (p->lchild || p->rchild))  //��������ڵ����Ӻ��ӣ����ӽڵ�û��չ����չ���ӽڵ�
        {
            mark[top] = 1;
            if(p->rchild)                                    //�����ڵ��ӽڵ���ջ
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
        if((!s[top]->lchild && !s[top]->rchild) || mark[top] == 1) //����ջ��Ԫ�ص��ӽڵ��Ѿ�չ������û���ӽڵ�
            printf("%c ", s[top--]->data);
    }
}

void levelorder(BiNODE* root)
{
    BiNODE* q[MAXN], *p;
    q[0] = root;
    int head = 0, tail = 1;        //headָ����ף�headָ���β��һ��
    while(head != tail)
    {
        p = q[head++];
        if(p->lchild) q[tail++] = p->lchild;
        if(p->rchild) q[tail++] = p->rchild;
        printf("%c ", p->data);
    }
}

int get_node(BiNODE* root)                   //���һ�����Ľڵ�
{
    if(root == NULL) return 0;
    return 1 + get_node(root->lchild) + get_node(root->rchild);
}

int get_height(BiNODE* root)                  //���һ�����ĸ߶ȣ��߶ȴ�1��ʼ����������Ҫ��1
{
    if(root == NULL) return 0;
    return 1 + (get_height(root->lchild) > get_height(root->rchild) ? get_height(root->lchild) : get_height(root->rchild));
}

int judge_fulltree(BiNODE* root)                        //�ж�һ�����Ƿ�Ϊ����
{
    int height = get_height(root) - 1, node = get_node(root);
    if((1<<(height + 1)) -1 == node) return 1;
    else return -1;
}

int judge_completetree(BiNODE* root)                      //�ж�һ�����Ƿ�Ϊ��ȫ������
{
    int flag = 0;
    BiNODE* q[MAXN], *p;
    q[0] = root;
    int head = 0, tail = 1;             //headָ����ף�headָ���β��һ��;
    while(head != tail)
    {
        p = q[head++];
        if(flag == 1 && (p->lchild || p->rchild)) //�Ѿ���Ǻ�֮���ֳ����������ӻ����Һ��ӵĽڵ�
        {
            flag = -1;
            break;
        }
        if(!p->lchild || !p->rchild) //������һ��û������ӻ����Ҷ��ӵĽڵ㣬���ñ�־λ
            flag = 1;
        if(p->lchild) q[tail++] = p->lchild;
        if(p->rchild) q[tail++] = p->rchild;
    }
    if(flag == -1) return 0;
    return 1;
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

void binary_tree_to_forest(BiNODE *root)
{
    int cnt = 0;
    printf("��%d������ǰ�����Ϊ��%c ", ++cnt, root->data);
    non_r_preorder(root->lchild);
    putchar('\n');
    while(root->rchild)
    {
        root = root->rchild;
        printf("��%d������ǰ�����Ϊ��%c ", ++cnt, root->data);
        non_r_preorder(root->lchild);
        putchar('\n');
    }
}
int main()
{
    int cas, n;
    BiNODE* root;
    printf("��ѡ��������Ķ��뷽ʽ��\n1. ǰ��+����������־λ\n2. ǰ������+��������\n3. ��������+��������\n");
    scanf("%d", &cas);
    if(cas == 1)
    {
        printf("���ڵ�һ���������Ľڵ���n�������n�нڵ����Ϣ\n");
        scanf("%d", &n);
        for(int i = 0; i < n; i++)
            scanf("%d %c %d", &LRtag[i].ltag, &LRtag[i].data, &LRtag[i].rtag);
        root =  BinaryTreeFromLRtag(n);
    }
    else if(cas == 2)
    {
        printf("���ڵ�һ���������Ľڵ���n���ڶ�������ǰ�����У�������������������\n");
        scanf("%d", &n);
        for(int i = 0; i < n; i++)
            scanf(" %c", &PreOrder[i]);
        for(int i = 0; i < n; i++)
            scanf(" %c", &inOrder[i]);
        root = BiTreeFromInPre(inOrder, PreOrder, n);
    }
    else
    {
        printf("���ڵ�һ���������Ľڵ���n,�ڶ��������������У������������������\n");
        scanf("%d", &n);
        for(int i = 0; i < n; i++)
            scanf(" %c", &inOrder[i]);
        for(int i = 0; i < n; i++)
            scanf(" %c", &postOrder[i]);
        root = BiTreeFromInPost(inOrder, postOrder, n);
    }

    while(1)
    {
        printf("\n��ѡ����Ӧ�ı�����ʽ��1.ǰ����� 2.������� 3.������� 4.��α���\n�������������˳�\n");
        scanf("%d", &cas);
        if(cas == 1)
        {
            printf("�ݹ�ǰ�������   ");
            r_preorder(root); printf("\n�ǵݹ�ǰ������� ");
            non_r_preorder(root); putchar('\n');
        }
        else if(cas == 2)
        {
            printf("�ݹ����������   ");
            r_inorder(root); printf("\n�ǵݹ���������� ");
            non_r_inorder(root); putchar('\n');
        }
        else if(cas == 3)
        {
            printf("�ݹ���������   ");
            r_postorder(root); printf("\n�ǵݹ��������� ");
            non_r_postorder(root); putchar('\n');
        }
        else if(cas == 4)
        {
            printf("��α����� ");
            levelorder(root); putchar('\n');
        }
        else break;
    }

    while(1)
    {
        printf("\n��ѡ�������ѯ�ʲ�����\n1.���Ľڵ��� 2.���ĸ߶�\n 3.�Ƿ�Ϊ���� 4.�Ƿ�Ϊ��ȫ������\n");
        printf("5.�ж�һ�ö������Ƿ��������ϵĽ���ֵ��С�ڸ����������ϵĽ���ֵ�����ڸ�\n�������������˳�\n");
        scanf("%d", &cas);
        if(cas == 1)
            printf("������Ľڵ���Ϊ��%d\n", get_node(root));
        else if(cas == 2)
            printf("������ĸ߶�Ϊ��%d\n", get_height(root) - 1);
        else if(cas == 3)
        {
            if(judge_fulltree(root) > 0) printf("�����������\n");
            else printf("�������������\n");
        }
        else if(cas == 4)
        {
            if(judge_completetree(root) > 0) printf("���������ȫ������\n");
            else printf("�����������ȫ������\n");
        }
        else if(cas == 5)
        {
            if(judge_findtree(root) > 0) printf("��һ����С�Ҵ�Ĳ�����\n");
            else printf("����һ����С�Ҵ�Ĳ�����\n");
        }
        else break;
    }

    printf("��ö���������ת��Ϊɭ�֣�\n");
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
