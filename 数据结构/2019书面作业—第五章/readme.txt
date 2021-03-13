//每个测试数据第一个为括号表示法,第二个为双亲表示法,第三个为前序+层次

测试数据1#:                    //只有一个根节点
F

1
-1 F

1
0 F

测试数据2#：       //每个结点只有一个子结点的树
A(B(C(D)(E)))

5
-1 A 0 B 1 C 2 D 3 E

5
0 A 1 B 2 C 3 D 4 E

测试数据3#：   //完全二叉树
A(B(D(H,I),E(J,K)),C(F(L,M),G(N,O)))

15
-1 A 0 B 0 C 1 D 1 E 2 F 2 G 3 H 3 I 4 J 4 K 5 L 5 M 6 N 6 O

15
0 A 1 B 2 D 3 H 3 I 2 E 3 J 3 K 1 C 2 F 3 L 3 M 2 G 3 N 3 O

测试数据4#： //不够规整的树
A(B(C(D,E,F(H), G)))

8
-1 A 0 B 1 C 2 D 2 E 2 F 2 G 5 H

8
0 A 1 B 2 C 3 D 3 E 3 F 3 G 4 H

测试数据5#：   //除了根结点都是叶子结点的树
A(B,C,D,E,F)

6
-1 A 0 B 0 C 0 D 0 E 0 F

6
0 A 1 B 1 C 1 D 1 E 1 F

测试数据6#：        //数据量大的树
A(B(E(N),F(O(P,Q)),G(R(S))),C(H(T),I(U(V,W(X)))),D(J(Y),K,L,M(Z)))

26
-1 A 0 B 0 C 0 D 1 E 1 F 1 G 2 H 2 I 3 J 3 K 3 L 3 M 
4 N 5 O 6 R 7 T 8 U 9 Y 12 Z 14 P 14 Q 15 S 17 V 17 W 24 X

26
0 A 1 B 2 E 3 N 2 F 3 O 4 P 4 Q 2 G 3 R 4 S 1 C 2 H 
3 T 2 I 3 U 4 V 4 W 5 X 1 D 2 J 3 Y 2 K 2 L 2 M 3 Z