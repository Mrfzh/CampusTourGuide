//邻接链表的结点类型
typedef struct AdjVexNode {
	int adjvex;						//邻接顶点在顶点数组中的位序
	struct AdjVexNode * nextArc;	//指向下一邻接顶点
	int length;						//存储道路长度
} AdjVexNode;	