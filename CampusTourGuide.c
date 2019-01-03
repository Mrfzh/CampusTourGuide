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
#define N 10	//������
#define E 28	//��·��

typedef int Status;

//�ڽ�����Ľ������
typedef struct AdjVexNode {
	int adjvex;		//�ڽӶ����ڶ��������е�λ��
	struct AdjVexNode *nextArc;		//ָ����һ�ڽӶ��㣨���ھ��㣩
	float length;			//��·����
} AdjVexNode;

//����
typedef struct{
	char *name;		//������
	int id;			//����
	char *intro;	//���
	struct AdjVexNode *firstArc;	//�ڽ������ͷָ��
} ScenicSpot;

//��·
typedef struct{
	ScenicSpot start;	//���
	ScenicSpot end;		//�յ�
	float length;			//��·����(��λ��km)
} Way;

//У԰��ͼ
typedef struct{
	ScenicSpot * spots;	//��������
	int n;				//������
	int e;				//��·��
	int * tags;			//��־���飬������ͼ�ı����б�Ƕ���������
} Map;

typedef struct {
	int prev;	//��ǰ���·���ϸö����ǰ�������λ��
	float lowcost;	//��ǰ���·���ĳ���
} DistInfo;

//���Ҿ����ڵ�ͼ�е�λ��
int LocateVex_M(Map *M, ScenicSpot spot) {
    int i;
    for (i = 0; i < M->n; i++) {
        if (spot.id == M->spots[i].id)
            return i;
    }
    return -1;
}

//������n�������e����·��У԰��ͼM
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
        start = ways[k].start;  end = ways[k].end;	//�ߵ�������
        i = LocateVex_M(M, start);      j = LocateVex_M(M, end);	//�������λ��
        if(i < 0 || j < 0)
			return ERROR;
        p = (AdjVexNode *)malloc(sizeof(AdjVexNode));
        if(p == NULL)
			return OVERFLOW;
        p->adjvex = j;
        p->nextArc = M->spots[i].firstArc;
		//��ʼ���ߵ�Ȩֵ
		p->length = ways[k].length;
		//��i������ڽ������ͷ����p���
        M->spots[i].firstArc = p;
    }
    return OK;
}

//��ӡ��ͼ
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

//��ʼ����ͼ
void InitMap(Map *M) {

	ScenicSpot * spots = (ScenicSpot *)malloc(sizeof(ScenicSpot) * N);
	Way * ways = (Way *)malloc(sizeof(Way) * E);
	int i;
	for (i = 0; i < N; i++) {
		spots[i].firstArc = NULL;
	}
	spots[0].name = "����";
	spots[0].id = 0;
	spots[0].intro = "ѧУ���ţ���������������ľ���ɫ����ѧУ�ı�־��";
	spots[1].name = "ͼ���";
	spots[1].id = 1;
	spots[1].intro = "ͼ��ݲ�����Ƶúܺÿ����������ܱ߾�ɫ�������������㹤�ı�ȥ֮�ء�";
	spots[2].name = "��ѧ¥";
	spots[2].id = 2;
	spots[2].intro = "����������ѧ¥����ѧ��ƽʱ�Ͽκ���ϰ�ĵط���";
	spots[3].name = "�ﾶ��";
	spots[3].id = 3;
	spots[3].intro = "������ѧ��ƽʱ�ܲ������ĵط���Ҳ��ٰ�����У�˻�ȴ��ͻ��";
	spots[4].name = "������";
	spots[4].id = 4;
	spots[4].intro = "��Ҫ������ݺ���ë��ݣ�����������ǹ㹤����ӵ������������ٰ������������";
	spots[5].name = "����";
	spots[5].id = 5;
	spots[5].intro = "�ж�����򳡣���ѧ��ƽʱ������ĵط���";
	spots[6].name = "��һʳ��";
	spots[6].id = 6;
	spots[6].intro = "���һ����λ�ڶ���������";
	spots[7].name = "�ڶ�ʳ��";
	spots[7].id = 7;
	spots[7].intro = "��ƶ�����λ�ڶ���������������װ�޺���ѧУ��Ư���ķ��ã�����ʳ������ࡢ�۸�ʵ�ݡ�";
	spots[8].name = "����ʳ��";
	spots[8].id = 8;
	spots[8].intro = "���������λ������������������¥��ʳ������ࡣ";
	spots[9].name = "����ʳ��";
	spots[9].id = 9;
	spots[9].intro = "����ķ���λ������������������Ư����";

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

//����ָ��
void commands() {
	printf("1. ���⾰�������Ϣ�Ĳ�ѯ\n");
	printf("2. ��ѯ������������֮���һ����̵ļ�·��\n");
	printf("3. ��ѯ������������֮������м�·��\n");
	printf("4. ��ѯ;������������ѣ��̣�·��\n");
	printf("5. ����͵�·���޸�����\n");
	printf("6. ��ѯÿ��������ڽӾ�����Ϣ\n");
	printf("7. У԰����ͼ\n");
	printf("8. �˳�����\n");
}

//����
void spots(Map M) {
	int i;
	for (i = 0; i < M.n; i++) {
        printf("%d. %s\n", i, M.spots[i].name);
	}
}

//���֮ǰ������
void clearInput() {
    char c = getchar();
    while (c != '\n') {
        c = getchar();
    }
}

//�Ͻ�˹�����㷨�����ͼM�д�i���㵽�������о�������·��������Dist����
Status Dijkstra(Map M, int i, DistInfo **Dist) {
    int j, m, k, min;
    AdjVexNode *p;
    *Dist = (DistInfo *)malloc(M.n * sizeof(DistInfo));
    //��ʼ��
    for (j = 0; j < M.n; j++) {
        (*Dist)[j].lowcost = INFINITY;
        M.tags[j] = UNSELECTED;
    }
    //����Դ��i���������бߴ���Dist
    for (p = M.spots[i].firstArc; p != NULL; p = p ->nextArc) {
        (*Dist)[p ->adjvex].prev = i;
        (*Dist)[p ->adjvex].lowcost = p->length;
    }
    //����Դ��i��Dist��Ϣ
    (*Dist)[i].prev = -1;
    (*Dist)[i].lowcost = 0;
    //��Դ�㵽������������·��
    for (m = 1; m < M.n; m++) {
        min = INFINITY;
        k = 0;
        for (j = 0; j < M.n; j++) {
            if (UNSELECTED == M.tags[j] && (*Dist)[j].lowcost < min) {
                k = j;
                min = (*Dist)[k].lowcost;
            }
        }
        //k�������㣺δ��ѡ���ҵ��ö�������·������������δ��ѡ��Ķ��������
        M.tags[k] = SELECTED;
        //����k������ڽӱ�
        for (p = M.spots[k].firstArc; p != NULL; p = p ->nextArc) {
            j = p ->adjvex; //j���ڽӶ����λ��
            if (UNSELECTED == M.tags[j] && (*Dist)[k].lowcost + p ->length < (*Dist)[j].lowcost) {
                //��i->k->j�ľ���С�ڵ�ǰi->j�����·��ʱ������Dist
                (*Dist)[j].lowcost = (*Dist)[k].lowcost + p ->length;
                (*Dist)[j].prev = k;
            }
        }
    }

    return OK;
}

//���Դ�㵽k��������·��
Status OutputPath(Map M, DistInfo *Dist, int k) {
    if (Dist[k].lowcost == INFINITY) {
        return ERROR;
    }
    if (-1 == k) {
        return OK;
    }
    OutputPath(M, Dist, Dist[k].prev);  //����ݹ��ȡ·���ϵĶ���
    printf("%s  ", M.spots[k].name);    //���������ǰ·���Ķ���
}

//�Ե�·�;���Ĳ���
void operation() {
    printf("1. ���Ӿ���\n");
    printf("2. ɾ������\n");
    printf("3. �޸ľ�����Ϣ\n");
    printf("4. ���ӵ�·\n");
    printf("5. ɾ����·\n");
    printf("6. �޸ĵ�·��Ϣ\n");
}

//�ж�M��ͼ���Ƿ���ھ���spot
Status hasSpot(Map M, ScenicSpot spot) {
    int i;
    for (i = 0; i < M.n; i++) {
        if (strcmp(M.spots[i].name, spot.name) == 0) {
            return TRUE;
        }
    }
    return FALSE;
}

//���Ӿ���
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
    M->tags = t;    //����
    spot.id = M->n;
    M->spots[M->n] = spot;
    M->tags[M->n] = UNVISITED;
    M->n++;

    return OK;
}

//ɾ������
Status removeSpot(Map * M, int index) {
    int e = 0;  //��ɾ�������йصı���
    int i;
    AdjVexNode * p = M->spots[index].firstArc;
    AdjVexNode * pparent;
    while (p != NULL) {
        e++;
        p = p ->nextArc;
    }
    //��ɾ��spot�����еĸö���
    if (index != M->n-1) {
        //��������һ�����㣬���ù���
        //��������λ���ϵĶ��㣬�����ǰ��
        for (i = index; i < M->n-1; i++) {
            M->spots[i] = M->spots[i+1];
        }
    }
    //���¶������ͱ���
    M->n--;
    M->e = M->e - e;
    //�ٸ����ڽ�����
    for (i = 0; i < M->n; i++) {
        p = M->spots[i].firstArc;
        if (p->adjvex == index) {   //���ͷָ�����ָ��Ҫɾ���Ľ��
            M->spots[i].firstArc = p->nextArc;  //ɾ�����
            p = M->spots[i].firstArc;
            while (p != NULL) {     //�����ڽӽ���λ��
                if (p->adjvex > index) {
                    p->adjvex--;
                }
                p = p->nextArc;
            }
        } else {
            if (p->adjvex > index) {
                p->adjvex--;
            }
            pparent = p;        //������һ���
            p = p->nextArc;     //ָ����һ���
            while (p != NULL) {
                if (p->adjvex == index) {   //���ҵ�Ҫɾ���Ķ���
                    pparent->nextArc = p->nextArc;
                } else {
                    if (p->adjvex > index) { //����λ��
                        p->adjvex--;
                    }
                    pparent = p;    //������һ���
                }
                p = pparent->nextArc;     //ָ����һ���
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

	printf("��ӭ�����㶫��ҵ��ѧ��������ϵͳ�ṩ���²���:\n");
	commands();

	while(!exit) {
        code = -1;    //��������
        printf("���������ָ�������ż��ɣ�����鿴ָ���������0����");
        scanf("%d", &code);
        switch (code) {
            //��ʾָ��
            case 0:
                commands();
                break;
			//���⾰�������Ϣ�Ĳ�ѯ
            case 1:
				s1 = -1;
				spots(*M);
				printf("������Ҫ��ѯ�ľ�����ţ�");
				scanf("%d", &s1);
				while (s1 < 0 || s1 >= M->n) {
					printf("������Ч�����������룺");
                    clearInput();
					scanf("%d", &s1);
				}
				//���������Ϣ
				printf("%s:%s\n", M->spots[s1].name, M->spots[s1].intro);
                break;
			//��ѯ������������֮���һ����̵ļ�·��
            case 2:
                s1 = s2 = -1;
                spots(*M);
				printf("�����������ţ�");
                clearInput();
                scanf("%d", &s1);
				while (s1 < 0 || s1 >= M->n) {
					printf("������Ч�����������룺");
                    clearInput();
					scanf("%d", &s1);
				}
				printf("�������յ���ţ�");
                clearInput();
                scanf("%d", &s2);
				while (s2 < 0 || s2 >= M->n) {
					printf("������Ч�����������룺");
                    clearInput();
					scanf("%d", &s2);
				}
                Dijkstra(*M, s1, &Dist);
                if (ERROR == OutputPath(*M, Dist, s2)) {
					printf("����������֮�䲻����·��\n");
				} else {
					printf("\n");
				}
                break;
			//��ѯ������������֮������м�·��
            case 3:

                break;
			//��ѯ;������������ѣ��̣�·��
            case 4:

                break;
			//����͵�·���޸�����
            case 5:
                ops = -1;
                operation();
                printf("������Ҫִ�еĲ�����");
                scanf("%d", &ops);
                while (ops < 0 || ops > 6) {
					printf("������Ч�����������룺");
                    clearInput();
					scanf("%d", &ops);
                }
                switch(ops) {
                    //���Ӿ���
                    case 1:
                        //���·���ռ�
                        name = (char *)malloc(sizeof(char));
                        intro = (char *)malloc(sizeof(char));
                        clearInput();
                        printf("�����뾰������");
                        scanf("%s", name);
                        clearInput();
                        printf("�����뾰��ļ�飺");
                        scanf("%s", intro);
						ScenicSpot spot;
						spot.name = name;
						spot.intro = intro;
						spot.firstArc = NULL;
						if (!(OK == addSpot(M, spot))) {
                            printf("��������ʧ�ܣ��þ����Ѵ��ڻ�����δ֪ԭ��\n");
						} else {
                            printf("���Ӷ���ɹ�\n");
						}
                        break;
                    //ɾ������
                    case 2:
                        s1 = -1;
                        spots(*M);
                        printf("������Ҫɾ���ľ�����ţ�");
                        clearInput();
                        scanf("%d", &s1);
                        while (s1 < 0 || s1 >= M->n) {
                            printf("������Ч�����������룺");
                            clearInput();
                            scanf("%d", &s1);
                        }
                        if (removeSpot(M, s1) == OK) {
                            printf("ɾ������ɹ�\n");
                        }

                        break;
                    //�޸ľ�����Ϣ
                    case 3:
                        break;
                    //���ӵ�·
                    case 4:
                        break;
                    //ɾ����·
                    case 5:
                        break;
                    //�޸ĵ�·��Ϣ
                    case 6:
                        break;
                    default:
                        break;
                }

                break;
			//��ѯÿ��������ڽӾ�����Ϣ
            case 6:

                break;
			//У԰����ͼ
            case 7:
				PrintMap(*M);
                break;
            //�˳�
            case 8:
                exit = 1;
                break;
            default:
                printf("��Ч�Ĳ���ָ����������룡\n");
				break;
        }
    }

	printf("��л����ʹ�ã�\n");

	return 0;
}








