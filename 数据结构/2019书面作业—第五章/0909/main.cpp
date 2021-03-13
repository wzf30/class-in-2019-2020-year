/*���ܽ��ܣ�
  1.���ȿ���ͨ�������ѡ�����뷽ʽ��1Ϊ���ű�ʾ����2λ˫�ױ�ʾ����3λǰ�� + ��α�ʾ��, Ȼ��������Ӧ�����ı�﷽ʽ��
  2.���������ݶ��뵽inputTree����(ͨ��convert����),inputTree�������data��parent������Ա�����õ������n����߲��m��
  3.��������inputTree����ת��Ϊ��ʽ��ͨ��creatTreeFromParent������,ÿ����㺬��data, child��ָ������,ָ��parent��ָ���Լ����Ӹ���num����lev��
  4.ͨ����Ӧ�ĺ�������ǰ��,�������α���(ǰ��������еݹ���ǵݹ����ַ���);
  5.���������Ľ���ܸ�����Ҷ�ӽ������(ͨ��get_leaf_node����)�����������߲��(ͨ��get_level����)
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
// ���Ĵ��������MAXM�Σ��������MAXN�����
#define MAXN 100
#define MAXM 10

typedef struct nodeParent
{
    char data;
    int parent;
}NODE_PARENT;

typedef struct linkNode
{
    char data;                           //�ڵ�����
    struct linkNode *child[MAXM];        //�ӽڵ�ĵ�ַ
    struct linkNode *parent;             //���ڵ�ĵ�ַ
    int lev;                             //�ڵ�Ĳ��(���ڵ�Ϊ0��
    int num;                             //�ӽڵ�ĸ���
}LINK_NODE;

LINK_NODE *addr_NODE[MAXN];                 //������ʽ�洢һ������ÿ������ָ��

int AddChild(LINK_NODE *parent,LINK_NODE *child,int m)     //m�����У�parent�²���һ���µ��ӽ��child
{
    if(parent == NULL || child == NULL || parent -> num == m) return -1;       //��������ӽڵ�ʧ��
    parent -> child[parent -> num] = child;
    parent -> num++;
    child -> lev = parent -> lev + 1;
    child -> parent = parent;
    return 1;
}

LINK_NODE *Node_initail(int m)  //m��������ʼ�������ݽ�����ͣ���ɽ��ĳ�ʼ������
{
    LINK_NODE *a = (LINK_NODE*)malloc(sizeof(LINK_NODE));
    for(int i = 0; i < m; i++)
        a -> child[i] = NULL;
    a -> num = 0;
    return a;
}

LINK_NODE *creatTreeFromParent(NODE_PARENT inputTree[],int m,int n)    //����inputTree��������ʽ�洢����һ������addr_NODE����¼ÿ������ָ�룬
{                                                                      //m��ʾ������n��ʾ�ڵ����
    int i;
    LINK_NODE *root, *p, *q;

    if(n < 1)  return NULL;                       //��Ϊ����
    root = Node_initail(m);                       //�������ڵ�
    root -> data = inputTree[0].data;
    root -> lev = 0;
    addr_NODE[0] = root;

    for(i = 1; i < n; i++)
    {
        q = Node_initail(m);
        q -> data = inputTree[i].data;
        addr_NODE[i] = q;
        p = addr_NODE[inputTree[i].parent];          //�ҵ��ӽڵ�q�ĸ��ڵ�p
        if(AddChild(p, q, m) < 0)
            printf("����ʧ�ܣ�\n");
    }

    return root;
}

void r_preorder(LINK_NODE *t, int m) //�ݹ�ǰ�������t:����m����
{
    int i;
    if(t !=  NULL)
    {
        printf("%c ", t -> data);
        for(i = 0; i < t -> num; i++)
            r_preorder(t -> child[i], m);
    }
}

void r_postorder(LINK_NODE *t, int m)       //�ݹ���������t:����m����
{
    int i;
    if(t != NULL)
    {
        for(i = 0; i < t -> num; i++)
            r_postorder(t -> child[i], m);
        printf("%c ", t -> data);
    }
}

void hierachicalorder(LINK_NODE *t, int m)         //�ǵݹ��α���
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

void preorderNoRecursion(LINK_NODE *t, int m)   //�ݹ�ǰ�������t:����m����
{
    LINK_NODE* s[MAXN];
    int top, i;
    if(t == NULL) return;
    s[0] = t;
    top = 1;                 //���ڵ���ջ��topָ��ջ������һ��λ��
    while(top > 0)
    {
        t = s[--top];
        printf("%c ", t -> data);
        for(i = t -> num - 1; i >= 0; i--)
            s[top++] = t -> child[i];
    }
}

void postorderNoRecursion(LINK_NODE *t, int m)    //�ǵݹ���������t:����m����
{
    LINK_NODE *s[MAXN], *p;
    int mark[MAXN], top = -1, j;
    s[++top] = t;
    mark[top] = 0;                        //���ڵ���ջ��topָ��ջ��
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

void convert1(NODE_PARENT inputTree[], int *a, int *b)       //�������ű�ʾ�� a����¼��������b����¼�ڵ���
{

    int s[MAXN], num[MAXN], top = -1, m = 0, n = 0;                 //top��¼��ǰջ��Ԫ��,ջ��Ԫ��Ϊ���׽ڵ�
    char ch;                                       //num��¼��ǰ���ڵ���ӽڵ����
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

void convert2(NODE_PARENT inputTree[], int *a, int *b)       //����˫�ױ�ʾ�� a����¼��������b����¼�ڵ���
{
    scanf("%d", b);
    int num[MAXN];
    int mx = 0, i;
    for(i = 0; i < *b; i++)
    {
        scanf("%d %c", &inputTree[i].parent, &inputTree[i].data);
        num[i] = 0;
        if(inputTree[i].parent >= 0) num[inputTree[i].parent]++;          //��¼���ڵ�ĺ�����
    }
    for(int i = 0; i < *b; i++) mx = mx > num[i] ? mx : num[i];
    *a = mx;
}

void convert3(NODE_PARENT inputTree[], int *a, int *b)       //����ǰ��+��ű�ʾ�� a����¼��������b����¼�ڵ���
{
    scanf("%d", b);
    int i, tmp, s1[MAXN], s2[MAXN], num[MAXN], top = 0, mx = 0; //s1��¼�ϸ��ڵ����ţ�s2��¼��Σ�num��¼���ڵ�ĺ��Ӹ���
    scanf("%d %c", &tmp, &inputTree[0].data);                  //topָ��ջ��
    inputTree[0].parent = -1;
    s1[top] = 0;
    s2[top] = tmp;
    num[top] = 0;
    for(i = 1; i < *b; i++)
    {
        scanf("%d %c", &tmp, &inputTree[i].data);
        while(s2[top] >= tmp)                                    //����ϸ��ڵ�Ĳ�β��ȵ�ǰ�ڵ�ͣ����ջֱ���ҵ���αȵ�ǰ�͵Ľڵ�
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

void get_leaf_node(int m, int n)            //���Ҷ�ӽ�����Ϣ
{
    int i, cnt = 0;
    printf("���%d������Ҷ�ӽ��Ϊ�� ", m);
    for(i = 0; i < n; i++)
    {
        if(addr_NODE[i] -> num == 0)
        {
            printf("%c ", addr_NODE[i] -> data);
            cnt++;
        }
    }
    printf("\nҶ�ӽ����ܸ���Ϊ%d\n", cnt);
}

void get_mx_level(int m, int n)                  //�������ε���Ϣ
{
    int i, mx = -1;
    for(i = 0; i < n; i++)
        mx = mx > addr_NODE[i] -> lev ? mx : addr_NODE[i] -> lev;
    printf("���%d�����������Ϊ%d\n", m, mx);
}
int main()
{
    int m, n, tag;
    LINK_NODE *root;
    NODE_PARENT inputTree[MAXN];            //���������ݣ�ÿһ���ڵ��data��parent�����뵽inputTree���飬����
    while(true)
    {
        printf("����ѡ����������ͣ�1.���ű�ʾ 2.˫�ױ�ʾ 3.ǰ�� + ��ű�ʾ\n����������������ѭ��\n");
        scanf("%d", &tag);
        printf("�����룺\n");                     //��2���3�ֱ�ʾ��Ҫ���ȸ����ڵ����
        switch(tag)
        {
            case 1: convert1(inputTree, &m, &n); break;
            case 2: convert2(inputTree, &m, &n); break;
            case 3: convert3(inputTree, &m, &n); break;
            default: tag = -1;
        }
        if(tag == -1) break;

        root = creatTreeFromParent(inputTree, m, n);            //���ڵ��parentΪ-1

        printf("ǰ�����(�ݹ�):   ");     r_preorder(root, m);
        printf("\nǰ�����(�ǵݹ�): ");    preorderNoRecursion(root, m);
        printf("\n\n�������(�ݹ�):   ");  r_postorder(root, m);
        printf("\n�������(�ǵݹ�): ");      postorderNoRecursion(root, m);
        printf("\n\n��α���: ");       hierachicalorder(root, m);
        printf("\n\n");

        printf("���%d�����Ľڵ�����Ϊ%d\n", m, n);
        get_leaf_node(m, n);
        get_mx_level(m, n);
        putchar('\n');
    }
    return 1;
}
