#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<string.h>

#define UNVISITED 0
#define VISITED 1
#define UNSELECTED 0
#define SELECTED 1
#define INFINITY INT_MAX
#define ERROR   0
#define OK  1
#define OVERFLOW -1
#define FALSE 0
#define TRUE 1
#define N 10	//景点数
#define E 28	//道路数

typedef int Status;

//邻接链表的结点类型
typedef struct AdjVexNode {
	int adjvex;		//邻接顶点在顶点数组中的位序
	struct AdjVexNode *nextArc;		//指向下一邻接顶点（相邻景点）
	float length;			//道路长度
} AdjVexNode;

//景点
typedef struct{
	char *name;		//景点名
	int id;			//代号
	char *intro;	//简介
	struct AdjVexNode *firstArc;	//邻接链表的头指针
} ScenicSpot;

//道路
typedef struct{
	ScenicSpot start;	//起点
	ScenicSpot end;		//终点
	float length;			//道路长度(单位：km)
} Way;

//校园地图
typedef struct{
	ScenicSpot * spots;	//景点数组
	int n;				//景点数
	int e;				//道路数
	int * tags;			//标志数组，用于在图的遍历中标记顶点访问与否
} Map;

typedef struct {
	int prev;	//当前最短路径上该顶点的前驱顶点的位序
	float lowcost;	//当前最短路径的长度
} DistInfo;

//查找景点在地图中的位序
int LocateVex_M(Map *M, ScenicSpot spot) {
    int i;
    for (i = 0; i < M->n; i++) {
        if (spot.id == M->spots[i].id)
            return i;
    }
    return -1;
}

//创建含n个景点和e条道路的校园地图M
Status CreateMap(Map * M, ScenicSpot * spots, int n, Way * ways, int e) {
    int i, j, k;
    ScenicSpot start, end;
    AdjVexNode * p;
    M->n = n;
    M->e = e;
    M->spots = (ScenicSpot *)malloc(n*sizeof(ScenicSpot));
    M->tags = (int *)malloc(n*sizeof(int));
    for(i = 0; i < M->n; i++) {
        M->tags[i] = UNVISITED;
        M->spots[i] = spots[i];
    }
    for(k = 0; k < M->e; k++) {
        start = ways[k].start;  end = ways[k].end;	//边的两顶点
        i = LocateVex_M(M, start);      j = LocateVex_M(M, end);	//两顶点的位序
        if(i < 0 || j < 0)
			return ERROR;
        p = (AdjVexNode *)malloc(sizeof(AdjVexNode));
        if(p == NULL)
			return OVERFLOW;
        p->adjvex = j;
        p->nextArc = M->spots[i].firstArc;
		//初始化边的权值
		p->length = ways[k].length;
		//在i顶点的邻接链表表头插入p结点
        M->spots[i].firstArc = p;
    }
    return OK;
}

//打印地图
void PrintMap(Map M) {
    int i;
    AdjVexNode * fa;
    for (i = 0; i < M.n; i++) {
        printf("%s  ", M.spots[i].name);
        fa = M.spots[i].firstArc;
        while (fa != NULL) {
            int index = fa->adjvex;
            printf("%s(%.1f)  ", M.spots[index].name, fa->length);
            fa = fa -> nextArc;
        }
        printf("\n");
    }
}

//初始化地图
void InitMap(Map *M) {

	ScenicSpot * spots = (ScenicSpot *)malloc(sizeof(ScenicSpot) * N);
	Way * ways = (Way *)malloc(sizeof(Way) * E);
	int i;
	for (i = 0; i < N; i++) {
		spots[i].firstArc = NULL;
	}
	spots[0].name = "正门";
	spots[0].id = 0;
	spots[0].intro = "学校正门，排列整齐的柱子颇具特色，是学校的标志。";
	spots[1].name = "图书馆";
	spots[1].id = 1;
	spots[1].intro = "图书馆不仅设计得很好看，而且其周边景色优美，是来到广工的必去之地。";
	spots[2].name = "教学楼";
	spots[2].id = 2;
	spots[2].intro = "共有六栋教学楼，是学生平时上课和自习的地方。";
	spots[3].name = "田径场";
	spots[3].id = 3;
	spots[3].intro = "这里是学生平时跑步锻炼的地方，也会举办例如校运会等大型活动。";
	spots[4].name = "体育馆";
	spots[4].id = 4;
	spots[4].intro = "主要有篮球馆和羽毛球馆，其中篮球馆是广工篮球队的主场，经常举办大型篮球赛。";
	spots[5].name = "网球场";
	spots[5].id = 5;
	spots[5].intro = "有多个网球场，是学生平时打网球的地方。";
	spots[6].name = "第一食堂";
	spots[6].id = 6;
	spots[6].intro = "简称一饭，位于东生活区。";
	spots[7].name = "第二食堂";
	spots[7].id = 7;
	spots[7].intro = "简称二饭，位于东生活区，经过新装修后，是学校最漂亮的饭堂，而且食物种类多、价格实惠。";
	spots[8].name = "第三食堂";
	spots[8].id = 8;
	spots[8].intro = "简称三饭，位于西生活区，有两层楼，食物种类多。";
	spots[9].name = "第四食堂";
	spots[9].id = 9;
	spots[9].intro = "简称四饭，位于西生活区，环境很漂亮。";

	ways[0].start = spots[0];
	ways[0].end = spots[1];
	ways[0].length = 0.5;
	ways[1].start = spots[1];
	ways[1].end = spots[0];
	ways[1].length = 0.5;

	ways[2].start = spots[1];
	ways[2].end = spots[2];
	ways[2].length = 0.2;
	ways[3].start = spots[2];
	ways[3].end = spots[1];
	ways[3].length = 0.2;

	ways[4].start = spots[1];
	ways[4].end = spots[3];
	ways[4].length = 0.6;
	ways[5].start = spots[3];
	ways[5].end = spots[1];
	ways[5].length = 0.6;

	ways[6].start = spots[2];
	ways[6].end = spots[3];
	ways[6].length = 0.7;
	ways[7].start = spots[3];
	ways[7].end = spots[2];
	ways[7].length = 0.7;

	ways[8].start = spots[2];
	ways[8].end = spots[6];
	ways[8].length = 0.8;
	ways[9].start = spots[6];
	ways[9].end = spots[2];
	ways[9].length = 0.8;

	ways[10].start = spots[2];
	ways[10].end = spots[7];
	ways[10].length = 0.6;
	ways[11].start = spots[7];
	ways[11].end = spots[2];
	ways[11].length = 0.6;

	ways[12].start = spots[3];
	ways[12].end = spots[4];
	ways[12].length = 0.1;
	ways[13].start = spots[4];
	ways[13].end = spots[3];
	ways[13].length = 0.1;

	ways[14].start = spots[3];
	ways[14].end = spots[7];
	ways[14].length = 0.6;
	ways[15].start = spots[7];
	ways[15].end = spots[3];
	ways[15].length = 0.6;

	ways[16].start = spots[4];
	ways[16].end = spots[5];
	ways[16].length = 0.3;
	ways[17].start = spots[5];
	ways[17].end = spots[4];
	ways[17].length = 0.3;

	ways[18].start = spots[4];
	ways[18].end = spots[8];
	ways[18].length = 0.5;
	ways[19].start = spots[8];
	ways[19].end = spots[4];
	ways[19].length = 0.5;

	ways[20].start = spots[5];
	ways[20].end = spots[8];
	ways[20].length = 0.4;
	ways[21].start = spots[8];
	ways[21].end = spots[5];
	ways[21].length = 0.4;

	ways[22].start = spots[6];
	ways[22].end = spots[7];
	ways[22].length = 0.4;
	ways[23].start = spots[7];
	ways[23].end = spots[6];
	ways[23].length = 0.4;

	ways[24].start = spots[7];
	ways[24].end = spots[8];
	ways[24].length = 0.9;
	ways[25].start = spots[8];
	ways[25].end = spots[7];
	ways[25].length = 0.9;

	ways[26].start = spots[8];
	ways[26].end = spots[9];
	ways[26].length = 0.5;
	ways[27].start = spots[9];
	ways[27].end = spots[8];
	ways[27].length = 0.5;

	CreateMap(M, spots, N, ways, E);
}

//操作指令
void commands() {
	printf("1. 任意景点相关信息的查询\n");
	printf("2. 查询任意两个景点之间的一条最短的简单路径\n");
	printf("3. 查询任意两个景点之间的所有简单路径\n");
	printf("4. 查询途经多个景点的最佳（短）路径\n");
	printf("5. 景点和道路的修改扩充\n");
	printf("6. 查询每个景点的邻接景点信息\n");
	printf("7. 校园仿真图\n");
	printf("8. 退出程序\n");
}

//景点
void spots(Map M) {
	int i;
	for (i = 0; i < M.n; i++) {
        printf("%d. %s\n", i, M.spots[i].name);
	}
}

//清除之前的输入
void clearInput() {
    char c = getchar();
    while (c != '\n') {
        c = getchar();
    }
}

//迪杰斯特拉算法：求地图M中从i景点到其他所有景点的最短路径，并由Dist返回
Status Dijkstra(Map M, int i, DistInfo **Dist) {
    int j, m, k, min;
    AdjVexNode *p;
    *Dist = (DistInfo *)malloc(M.n * sizeof(DistInfo));
    //初始化
    for (j = 0; j < M.n; j++) {
        (*Dist)[j].lowcost = INFINITY;
        M.tags[j] = UNSELECTED;
    }
    //将从源点i引出的所有边存入Dist
    for (p = M.spots[i].firstArc; p != NULL; p = p ->nextArc) {
        (*Dist)[p ->adjvex].prev = i;
        (*Dist)[p ->adjvex].lowcost = p->length;
    }
    //设置源点i的Dist信息
    (*Dist)[i].prev = -1;
    (*Dist)[i].lowcost = 0;
    //求源点到其他顶点的最短路径
    for (m = 1; m < M.n; m++) {
        min = INFINITY;
        k = 0;
        for (j = 0; j < M.n; j++) {
            if (UNSELECTED == M.tags[j] && (*Dist)[j].lowcost < min) {
                k = j;
                min = (*Dist)[k].lowcost;
            }
        }
        //k顶点满足：未被选择，且到该顶点的最短路径长度在所有未被选择的顶点中最短
        M.tags[k] = SELECTED;
        //遍历k顶点的邻接表
        for (p = M.spots[k].firstArc; p != NULL; p = p ->nextArc) {
            j = p ->adjvex; //j是邻接顶点的位序
            if (UNSELECTED == M.tags[j] && (*Dist)[k].lowcost + p ->length < (*Dist)[j].lowcost) {
                //当i->k->j的距离小于当前i->j的最短路径时，更新Dist
                (*Dist)[j].lowcost = (*Dist)[k].lowcost + p ->length;
                (*Dist)[j].prev = k;
            }
        }
    }

    return OK;
}

//输出源点到k顶点的最短路径
Status OutputPath(Map M, DistInfo *Dist, int k) {
    if (Dist[k].lowcost == INFINITY) {
        return ERROR;
    }
    if (-1 == k) {
        return OK;
    }
    OutputPath(M, Dist, Dist[k].prev);  //逆向递归获取路径上的顶点
    printf("%s  ", M.spots[k].name);    //正向输出当前路径的顶点
}

//对道路和景点的操作
void operation() {
    printf("1. 增加景点\n");
    printf("2. 删除景点\n");
    printf("3. 修改景点信息\n");
    printf("4. 增加道路\n");
    printf("5. 删除道路\n");
    printf("6. 修改道路信息\n");
}

//判断M地图中是否存在景点spot
Status hasSpot(Map M, ScenicSpot spot) {
    int i;
    for (i = 0; i < M.n; i++) {
        if (strcmp(M.spots[i].name, spot.name) == 0) {
            return TRUE;
        }
    }
    return FALSE;
}

//增加景点
Status addSpot(Map * M, ScenicSpot spot) {
    if (hasSpot(*M, spot) == TRUE) {
        return ERROR;
    }
    ScenicSpot *s = (ScenicSpot *)realloc(M->spots, (M->n+1)*sizeof(ScenicSpot));
    int *t = (int *)realloc(M->tags, (M->n+1)*sizeof(int));
    if (s == NULL || t == NULL) {
        return OVERFLOW;
    }
    M->spots = s;
    M->tags = t;    //扩容
    spot.id = M->n;
    M->spots[M->n] = spot;
    M->tags[M->n] = UNVISITED;
    M->n++;

    return OK;
}

//删除景点
Status removeSpot(Map * M, int index) {
    int e = 0;  //跟删除顶点有关的边数
    int i;
    AdjVexNode * p = M->spots[index].firstArc;
    AdjVexNode * pparent;
    while (p != NULL) {
        e++;
        p = p ->nextArc;
    }
    //先删除spot数组中的该顶点
    if (index != M->n-1) {
        //如果是最后一个顶点，不用管它
        //对于其他位置上的顶点，后面的前移
        for (i = index; i < M->n-1; i++) {
            M->spots[i] = M->spots[i+1];
        }
    }
    //更新顶点数和边数
    M->n--;
    M->e = M->e - e;
    //再更新邻接链表
    for (i = 0; i < M->n; i++) {
        p = M->spots[i].firstArc;
        if (p->adjvex == index) {   //如果头指针就是指向要删除的结点
            M->spots[i].firstArc = p->nextArc;  //删除结点
            p = M->spots[i].firstArc;
            while (p != NULL) {     //更新邻接结点的位序
                if (p->adjvex > index) {
                    p->adjvex--;
                }
                p = p->nextArc;
            }
        } else {
            if (p->adjvex > index) {
                p->adjvex--;
            }
            pparent = p;        //保存上一结点
            p = p->nextArc;     //指向下一结点
            while (p != NULL) {
                if (p->adjvex == index) {   //若找到要删除的顶点
                    pparent->nextArc = p->nextArc;
                } else {
                    if (p->adjvex > index) { //更新位序
                        p->adjvex--;
                    }
                    pparent = p;    //保存上一结点
                }
                p = pparent->nextArc;     //指向下一结点
            }
        }
    }

    return OK;
}

int main() {
	Map *M = (Map *)malloc(sizeof(Map));
	InitMap(M);

	int exit = 0;
	int code;
	int s1, s2;
	DistInfo *Dist;
	int ops;
	char *name;
	char *intro;

	printf("欢迎来到广东工业大学，本导游系统提供以下操作:\n");
	commands();

	while(!exit) {
        code = -1;    //重置命令
        printf("请输入操作指令（输入序号即可，若想查看指令表，请输入0）：");
        scanf("%d", &code);
        switch (code) {
            //显示指令
            case 0:
                commands();
                break;
			//任意景点相关信息的查询
            case 1:
				s1 = -1;
				spots(*M);
				printf("请输入要查询的景点序号：");
				scanf("%d", &s1);
				while (s1 < 0 || s1 >= M->n) {
					printf("输入无效，请重新输入：");
                    clearInput();
					scanf("%d", &s1);
				}
				//输出景点信息
				printf("%s:%s\n", M->spots[s1].name, M->spots[s1].intro);
                break;
			//查询任意两个景点之间的一条最短的简单路径
            case 2:
                s1 = s2 = -1;
                spots(*M);
				printf("请输入起点序号：");
                clearInput();
                scanf("%d", &s1);
				while (s1 < 0 || s1 >= M->n) {
					printf("输入无效，请重新输入：");
                    clearInput();
					scanf("%d", &s1);
				}
				printf("请输入终点序号：");
                clearInput();
                scanf("%d", &s2);
				while (s2 < 0 || s2 >= M->n) {
					printf("输入无效，请重新输入：");
                    clearInput();
					scanf("%d", &s2);
				}
                Dijkstra(*M, s1, &Dist);
                if (ERROR == OutputPath(*M, Dist, s2)) {
					printf("这两个顶点之间不存在路径\n");
				} else {
					printf("\n");
				}
                break;
			//查询任意两个景点之间的所有简单路径
            case 3:

                break;
			//查询途经多个景点的最佳（短）路径
            case 4:

                break;
			//景点和道路的修改扩充
            case 5:
                ops = -1;
                operation();
                printf("请输入要执行的操作：");
                scanf("%d", &ops);
                while (ops < 0 || ops > 6) {
					printf("输入无效，请重新输入：");
                    clearInput();
					scanf("%d", &ops);
                }
                switch(ops) {
                    //增加景点
                    case 1:
                        //重新分配空间
                        name = (char *)malloc(sizeof(char));
                        intro = (char *)malloc(sizeof(char));
                        clearInput();
                        printf("请输入景点名：");
                        scanf("%s", name);
                        clearInput();
                        printf("请输入景点的简介：");
                        scanf("%s", intro);
						ScenicSpot spot;
						spot.name = name;
						spot.intro = intro;
						spot.firstArc = NULL;
						if (!(OK == addSpot(M, spot))) {
                            printf("新增景点失败，该景点已存在或其他未知原因\n");
						} else {
                            printf("增加顶点成功\n");
						}
                        break;
                    //删除景点
                    case 2:
                        s1 = -1;
                        spots(*M);
                        printf("请输入要删除的景点序号：");
                        clearInput();
                        scanf("%d", &s1);
                        while (s1 < 0 || s1 >= M->n) {
                            printf("输入无效，请重新输入：");
                            clearInput();
                            scanf("%d", &s1);
                        }
                        if (removeSpot(M, s1) == OK) {
                            printf("删除顶点成功\n");
                        }

                        break;
                    //修改景点信息
                    case 3:
                        break;
                    //增加道路
                    case 4:
                        break;
                    //删除道路
                    case 5:
                        break;
                    //修改道路信息
                    case 6:
                        break;
                    default:
                        break;
                }

                break;
			//查询每个景点的邻接景点信息
            case 6:

                break;
			//校园仿真图
            case 7:
				PrintMap(*M);
                break;
            //退出
            case 8:
                exit = 1;
                break;
            default:
                printf("无效的操作指令，请重新输入！\n");
				break;
        }
    }

	printf("感谢您的使用！\n");

	return 0;
}








