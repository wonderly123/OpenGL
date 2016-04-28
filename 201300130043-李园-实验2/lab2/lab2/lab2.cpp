// lab1_2.cpp : �������̨Ӧ�ó������ڵ㡣
//<>���Ѿ����ӵ��������Ŀ��ļ� ����һ�������ļ�Ŀ¼������ļ������Լ�δ������ӵ��ⲿ����ļ�
//���Լ����Ϊ�Լ�д��ͷ�ļ��á��� C++��׼ͷ�ļ���<>

#include "stdafx.h"
#include <gl/glut.h>
#include <math.h>
#include "stdlib.h"
#include <iostream>
#include "stack"
#define EPSILON 0.000001    //��С������
using namespace std;

//��ʼ�����ڽ���

void myinit(double w, double h)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	gluOrtho2D(-w / 2, w / 2, -h / 2, h / 2);
	glMatrixMode(GL_MODELVIEW);
}


//��ṹ��
struct Point
{
	int x;         //x����
	int y;         //y����
};

//�߽ṹ��
struct Line
{
	Point high_point;        //�߶˵�
	Point low_point;         //�Ͷ˵�
	int is_active;           //�Ƿ�Ϊ��Ч�ߣ�ˮƽ�ߣ�0������ˮƽ�ߣ�1��
	double inverse_k;        //б��k�ĵ���
};

//�߽��
struct EdgeNode
{
	double x;              //ɨ������߽����x���꣨�ߵĵͶ˵��x���꣩
	int y_max;             //�ߵĸ߶˵��y����ymax
	double inverse_k;      //б��k�ĵ���
	EdgeNode *next;        //��һ���߽���ָ��
};

//��Ч�߱�
struct ActiveEdgeTable
{
	int y;                 //ɨ����y
	EdgeNode *head;        //�������ͷָ��
};

//Ͱ���
typedef struct Bucket
{
	int y;                //ɨ����y
	EdgeNode *head;       //�������ͷָ��
	Bucket *next;         //��һ��Ͱ��ָ��
} EdgeTable;


//�Ƚ�2����ĸ߶�
int compare(Point p1, Point p2)
{
	if (p1.y > p2.y)
		return 1;
	else if (p1.y == p2.y)
		return 0;
	return -1;
}

//�ɵ����������߶�����
Line* create_lines(Point points[], int n)
{
	Line *lines = (Line*)malloc(n * sizeof(Line));
	for (int i = 0; i < n; ++i)
	{
		Point p1 = points[i];
		Point p2 = points[(i + 1) % n];

		int result = compare(p1, p2);
		if (result == 0)
			lines[i].is_active = 0;
		else
			lines[i].is_active = 1;

		lines[i].high_point = result > 0 ? p1 : p2;
		lines[i].low_point = result < 0 ? p1 : p2;
		lines[i].inverse_k = (double)(p2.x - p1.x) / (double)(p2.y - p1.y);
	}

	return lines;
}

//��ȡ����������͵Ķ˵�
Point get_lowest_point(Line lines[], int n)
{
	Point lowest_point = lines[0].low_point;

	for (int i = 1; i < n; ++i)
	{
		Point low_point = lines[i].low_point;
		if (compare(lowest_point, low_point) > 0)
			lowest_point = low_point;
	}

	return lowest_point;
}

//��ȡ����������ߵĶ˵�
Point get_highest_point(Line lines[], int n)
{
	Point highest_point = lines[0].high_point;

	for (int i = 1; i < n; ++i)
	{
		Point high_point = lines[i].high_point;
		if (compare(highest_point, high_point) < 0)
			highest_point = high_point;
	}

	return highest_point;
}

//����2��Line����
void swap(Line &l1, Line &l2)
{
	Line temp = l1;
	l1 = l2;
	l2 = temp;
}

//���������������
void sort(Line lines[], int n)
{
	//�Ȱ��Ͷ˵��y���������������
	for (int i = 0; i < n; ++i)
	{
		int min_index = i;
		for (int j = i + 1; j < n; ++j)
		{
			if (lines[j].low_point.y < lines[min_index].low_point.y)
				min_index = j;
		}

		swap(lines[i], lines[min_index]);
	}

	//�ٽ��������鰴�Ͷ˵��x�����������У���x������ȣ���inverse_k����
	for (int i = 0; i < n; ++i)
	{
		int min_index = i;
		for (int j = i + 1; lines[j].low_point.y == lines[i].low_point.y; ++j)
		{
			if (lines[j].low_point.x < lines[min_index].low_point.x)
				min_index = j;
		}

		swap(lines[i], lines[min_index]);

		if (i > 0 && lines[i].low_point.x == lines[i - 1].low_point.x)
		{
			if (lines[i].is_active == 1 && lines[i - 1].is_active == 1)
			{
				if (lines[i].inverse_k < lines[i - 1].inverse_k)
					swap(lines[i], lines[i - 1]);
			}
		}
	}
}

//����һ���߱�
EdgeTable* create_edge_table(Line lines[], int n)
{
	EdgeTable *edge_table = (EdgeTable*)malloc(sizeof(EdgeTable));
	edge_table->head = NULL;
	edge_table->next = NULL;

	sort(lines, n);
	Point lowest_point = get_lowest_point(lines, n);
	Point highest_point = get_highest_point(lines, n);

	EdgeTable *s = edge_table;
	for (int i = lowest_point.y; i <= highest_point.y; ++i)
	{
		Bucket *bucket = (Bucket*)malloc(sizeof(Bucket));
		bucket->y = i;
		bucket->next = NULL;

		bucket->head = (EdgeNode*)malloc(sizeof(EdgeNode));
		bucket->head->next = NULL;

		EdgeNode *p = bucket->head;
		for (int j = 0; j < n; ++j)
		{
			if (lines[j].is_active == 0)
				continue;

			if (lines[j].low_point.y == i)
			{
				EdgeNode *q = (EdgeNode*)malloc(sizeof(EdgeNode));
				q->x = lines[j].low_point.x;
				q->y_max = lines[j].high_point.y;
				q->inverse_k = lines[j].inverse_k;
				q->next = NULL;

				p->next = q;
				p = q;
			}
		}
		s->next = bucket;
		s = bucket;
	}
	return edge_table;
}

//�ӱ߱���ȡ����һ����Ϊ�յ�Ͱ��ʼ����Ч�߱�
ActiveEdgeTable* init_active_table(EdgeTable *edge_table)
{
	ActiveEdgeTable *active_table = (ActiveEdgeTable*)malloc(sizeof(ActiveEdgeTable));
	active_table->y = edge_table->next->y;
	active_table->head = (EdgeNode*)malloc(sizeof(EdgeNode));
	active_table->head->next = NULL;

	EdgeNode *p = edge_table->next->head;
	EdgeNode *q = active_table->head;
	while (p->next != NULL)
	{
		EdgeNode *s = (EdgeNode*)malloc(sizeof(EdgeNode));
		s->x = p->next->x;
		s->y_max = p->next->y_max;
		s->inverse_k = p->next->inverse_k;
		s->next = NULL;
		q->next = s;
		q = s;
		p = p->next;
	}
	return active_table;
}

//����Ч�߱���ɾ��ָ��y_max�ı߽��
void delete_edge(ActiveEdgeTable *active_table, int y_max)
{
	EdgeNode *p = active_table->head;
	while (p->next != NULL)
	{
		EdgeNode *q = p->next;
		if (q->y_max == y_max)
		{
			p->next = q->next;
			free(q);
		}
		else
			p = p->next;
	}
}

//��һ���߽�㰴������ӵ���Ч�߱���
void add_edge(ActiveEdgeTable *active_table, EdgeNode edge)
{
	EdgeNode *t = (EdgeNode*)malloc(sizeof(EdgeNode));
	t->x = edge.x;
	t->y_max = edge.y_max;
	t->inverse_k = edge.inverse_k;
	t->next = NULL;

	EdgeNode *p = active_table->head;
	while (p->next != NULL)
	{
		EdgeNode *q = p->next;
		if ((edge.x < q->x) || (edge.x == q->x && edge.inverse_k < q->inverse_k))
		{
			p->next = t;
			t->next = q;
			return;
		}
		p = p->next;
	}

	p->next = t;
}

//������Ч�߱�����߱��ж�Ӧ��Ͱ�ϲ�
ActiveEdgeTable* update_active_table(ActiveEdgeTable *active_table, EdgeTable *edge_table)
{
	//����ɨ����y
	++active_table->y;

	//ɾ��y=ymax�ı�
	delete_edge(active_table, active_table->y);

	//���±߽�������
	EdgeNode *p = active_table->head->next;
	while (p != NULL)
	{
		p->x += p->inverse_k;
		p = p->next;
	}

	//�ҵ��߱��ж�Ӧ��Ͱ
	EdgeTable *q = edge_table;
	while ((q = q->next) != NULL && q->y != active_table->y);

	//����ҵ�������кϲ�
	if (q != NULL)
	{
		EdgeNode *s = q->head;
		while ((s = s->next) != NULL)
		{
			add_edge(active_table, *s);
		}
	}

	return active_table;
}

//��������εı߿�
void DrawPolygon(Point points[], int n)
{
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < n; ++i)
		glVertex3f(points[i].x, points[i].y, 0);
	glEnd();
}

//��ָ�������ش�С�������
void Fill(Point points[], int n)
{
	Line *lines = create_lines(points, n);
	EdgeTable *edge_table = create_edge_table(lines, n);
	ActiveEdgeTable *active_table = init_active_table(edge_table);

	while (active_table->head->next != NULL)
	{
		EdgeNode *p = active_table->head;
		int b = -1;
		while (p->next != NULL)
		{
			if (b > 0)
			{
				int left = p->x;
				int right = p->next->x;
				//������Ǿֲ���͵㣬����б߽紦��
				if (!(p->x - p->next->x >= -EPSILON && p->x - p->next->x <= EPSILON))
				{
					//������߽�
					if (!(p->x - left >= -EPSILON && p->x - left <= EPSILON))
						left += 1;
					//�����ұ߽�
					if (p->next->x - right >= -EPSILON && p->next->x - right <= EPSILON)
						right -= 1;
				}
				for (int i = left; i <= right; ++i)
				{
					glBegin(GL_POINTS);
					glVertex2d(i, active_table->y);
					glEnd();
					glFlush();
				}
			}
			p = p->next;
			b = -b;
		}
		active_table = update_active_table(active_table, edge_table);
	}
}

//ɨ�������ӣ���Ʊ߽磩����㷨
void glPoint(int x, int y, int r, int g, int b) {
	glColor3ub(r, g, b);
	glPointSize(1);
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
	glFlush();
}

bool sameColor(int r1, int g1, int b1, int r2, int g2, int b2) {
	//�ݲ��
	int dis = 1;
	if (fabs(r1 - r2) <= dis&&fabs(g1 - g2) <= dis&&fabs(b1 - b2) <= dis) {
		return true;
	}
	else {
		return false;
	}

}

GLubyte oldColor[3] = { 255, 255, 255 };
GLubyte borderColor[3] = { 255,0,0 };
int halfWidth, halfHeight;

void zzFill(int startX, int startY, int r, int g, int b) {

	stack<Point> pixelStack;
	//x,y�Ǹ������������ص㣬rgb����Ҫ������ɫ��RGBֵ
	Point point = { startX, startY };
	pixelStack.push(point);
	int saveX;
	int xRight, xLeft;
	int x, y;
	//���ջ��Ϊ��
	while (!pixelStack.empty()) {
		//��ȡ��˵�Ԫ��
		Point tempPoint = pixelStack.top();
		//ɾ����˵�Ԫ��
		pixelStack.pop();
		saveX = tempPoint.x;
		x = tempPoint.x;
		y = tempPoint.y;
		GLubyte iPixel[3];
		glReadPixels(x + halfWidth, y + halfHeight, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &iPixel);
		//���û�е����ұ߽磬�����
		while (!sameColor(iPixel[0], iPixel[1], iPixel[2], borderColor[0],
			borderColor[1], borderColor[2])) {
			glPoint(x, y, r, g, b);
			x = x + 1;
			glReadPixels(x + halfWidth, y + halfHeight, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &iPixel);
			printf("r:%d,g:%d,b:%d\n", iPixel[0], iPixel[1], iPixel[2]);
		}
		xRight = x - 1;
		x = saveX - 1;
		glReadPixels(x + halfWidth, y + halfWidth, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &iPixel);
		//���û�е�����߽磬�����
		while (!sameColor(iPixel[0], iPixel[1], iPixel[2], borderColor[0],
			borderColor[1], borderColor[2])) {
			glPoint(x, y, r, g, b);
			x = x - 1;
			glReadPixels(x + halfWidth, y + halfWidth, 1, 1,
				GL_RGB, GL_UNSIGNED_BYTE, &iPixel);
		}
		//������˵�
		xLeft = x + 1;
		//���ұߵĵ㿪ʼ
		x = xRight;
		//����϶˵�ɨ����
		y = y + 1;
		while (x >= xLeft) {
			glReadPixels(x + halfWidth, y + halfWidth, 1, 1,
				GL_RGB, GL_UNSIGNED_BYTE, &iPixel);
			if (!sameColor(iPixel[0], iPixel[1], iPixel[2], borderColor[0],
				borderColor[1], borderColor[2]) && !sameColor(iPixel[0],
					iPixel[1], iPixel[2], r, g, b)) {
				//����Ϸ��ĵ㲻�Ǳ߽�㣬ֱ��ѹ��
				Point p = { x, y };
				pixelStack.push(p);
				//ѹ��֮��ֹͣѭ��
				break;
			}
			else {
				x--;
				glReadPixels(x + halfWidth, y + halfWidth, 1, 1, GL_RGB,
					GL_UNSIGNED_BYTE, &iPixel);
			}
		}
		//����¶˵�ɨ����
		y = y - 2;
		//���ұߵĵ㿪ʼ
		x = xRight;
		while (x >= xLeft) {
			glReadPixels(x + halfWidth, y + halfWidth, 1, 1,
				GL_RGB, GL_UNSIGNED_BYTE, &iPixel);
			if (!sameColor(iPixel[0], iPixel[1], iPixel[2],
				borderColor[0], borderColor[1], borderColor[2])
				&& !sameColor(iPixel[0], iPixel[1], iPixel[2], r, g, b)) {
				//����Ϸ��ĵ㲻�Ǳ߽�㣬ֱ��ѹ��
				Point p = { x, y };
				//ѹ��֮��ֹͣѭ��
				pixelStack.push(p);
				break;
			}
			else {
				x--;
				glReadPixels(x + halfWidth, y + halfWidth, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &iPixel);
			}
		}
	}
}



//bresenham ����ֱ��
void bresenham_line(int x1, int y1, int x2, int y2)
{
	int x = x1;
	int y = y1;
	int dx = abs(x2 - x1);
	int dy = abs(y2 - y1);
	int s1 = x2 > x1 ? 1 : -1;
	int s2 = y2 > y1 ? 1 : -1;
	bool change = false;
	// Ĭ�ϲ����� dx��dy 
	if (dy > dx)
		// ��б�ʴ��� 1 ʱ��dx��dy ���� 
	{
		int temp = dx;
		dx = dy;
		dy = temp;
		change = true;
	}
	int p = 2 * dy - dx;
	glBegin(GL_POINTS);
	for (int i = 0; i < dx; i++)
	{
		glVertex3f(x, y, 0);
		if (p >= 0)
		{
			if (!change)
				// ��б�� < 1 ʱ��ѡȡ�������ص� 
				y += s2;
			else// ��б�� > 1 ʱ��ѡȡ�������ص� 
				x += s1;
			p -= 2 * dx;
		} if (!change)
			x += s1;
		// ��б�� < 1 ʱ��ȡ x 
		else y += s2;
		// ��б�� > 1 ʱ��ȡ y 
		p += 2 * dy;
	}
	glEnd();
}

//����
void gl_Point(int x, int y) {
	glBegin(GL_POINTS);
	glVertex3i(x, y, 0);
	glEnd();
}



//Bresenham��Բ�㷨
void bresenhamArc(int R, int centerX, int centerY, int area) {
	int x, y, d;
	x = 0; y = R; d = 3 - 2 * R;
	while (x<y) {
		switch (area) {
		case 1:
			gl_Point(x + centerX, y + centerY);
			break;
		case 2:
			gl_Point(y + centerX, x + centerY);
			break;
		case 3:
			gl_Point(y + centerX, -x + centerY);
			break;
		case 4:
			gl_Point(x + centerX, -y + centerY);
			break;
		case 5:
			gl_Point(-x + centerX, -y + centerY);
			break;
		case 6:
			gl_Point(-y + centerX, -x + centerY);
			break;
		case 7:
			gl_Point(-y + centerX, x + centerY);
			break;
		case 8:
			gl_Point(-x + centerX, y + centerY);
			break;
		}
		if (d<0) {
			d = d + 4 * x + 6;
		}
		else {
			d = d + 4 * (x - y) + 10;
			y = y - 1;
		}
		x = x + 1;
	}
	if (x == y) {
		gl_Point(x, y);
	}
}
//display����
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	double w = 1400.0;
	double h = 1400.0;

	//���̸� �� ֱ��
	glColor3f(1.0, 1.0, 1.0);
	glPointSize(3.0f);
	bresenham_line(-0.42*w, -0.47*h, 0.42*w, -0.47*h);
	bresenham_line(-0.42*w, -0.47*h, -0.42*w, 0.47*h);
	bresenham_line(0.42*w, 0.47*h, -0.42*w, 0.47*h);
	bresenham_line(0.42*w, 0.47*h, 0.42*w, -0.47*h);


	glPointSize(1.0f);
	bresenham_line(-0.4*w, -0.45*h, 0.4*w, -0.45*h);
	bresenham_line(-0.4*w, -0.45*h, -0.4*w, 0.45*h);
	bresenham_line(0.4*w, 0.45*h, -0.4*w, 0.45*h);
	bresenham_line(0.4*w, 0.45*h, 0.4*w, -0.45*h);

	for (int i = -0.35*h; i < 0.45*h; i = i + 0.1*h)
	{
		bresenham_line(-0.4*w, i, 0.4*w, i);
	}



	for (int i = -0.3*w; i < 0.4*w; i += 0.1*w)
	{
		bresenham_line(i, -0.45*h, i, -0.05*h);
		bresenham_line(i, 0.45*h, i, 0.05*h);
	}
	//���̸� �� б��
	bresenham_line(-0.1*w, -0.45*h, 0.1*w, -0.25*h);
	bresenham_line(-0.1*w, -0.25*h, 0.1*w, -0.45*h);
	bresenham_line(-0.1*w, 0.45*h, 0.1*w, 0.25*h);
	bresenham_line(-0.1*w, 0.25*h, 0.1*w, 0.45*h);

	//�������̱�־��200<x<300 225<y<275 z=0

	//��Ȧ
	glColor3ub(138, 2, 2);
	glPointSize(1.0);
	for (int j = 1; j < 9; j++)
	{
		bresenhamArc(0.05*h, 0, 0, j);
	}
	glFlush();
	int x0 = -0.05*h, y0 = 0.05*h;
	double k = 0.0001;
	
	//���οյĴ�ɽ
	//����εĶ�������
	Point points[] = { { 81, 442 },{ 81, 384 },{ 208, 384 },{ 293, 213 },{ 387, 298 },
	{ 509, 69 },{ 616, 281 },{ 667,192 },{ 792,435 },{ 879,435 },{ 879,493 },{ 822,493 },
	{ 838,525 },{ 773,526 },{ 757,495 },{ 706,495 },{ 716,519 },{ 638,522 },
	{ 626,495 },{ 571,495 } ,{ 576,604 },{ 474,591 },{ 474,506 },{ 428,505 },
	{ 463,442 },{ 389,442 },{ 356,499 },{ 293,490 },{ 314,442 },{ 252,442 },
	{ 226,495 },{ 154,494 },{ 179,442 }
	};
	
	//���㶥�����
	int n = sizeof(points) / sizeof(Point);
	for (int i = 0; i < n;i++)
	{
		points[i].x = points[i].x*k*h +x0;
		points[i].y = -points[i].y*k*h+y0;
	}
	//ʹ�ú�ɫ��������εı߿�
	glColor3ub(156, 12, 19);
	DrawPolygon(points, n);
	glFlush();

	//ָ�����С
	glPointSize(1.0f);

	//ʹ�ú�ɫ�������
	glColor3ub(156, 12, 19);
	Fill(points, n);
	glFlush();

	//��ɽ���οղ���
	//�ο�1    ����εĶ�������
	Point space1[] = { { 281,382 },{ 317,315 },{ 354,348 },{ 339,383 } };
	Point space2[] = { { 419,382 },{ 511,209 },{ 581,383 },{ 541,383 },
	{ 541,320 },{ 502,322 },{ 502,383 } };
	Point space3[] = { { 556,435 },{ 582,389 },{ 603,434 } };
	Point space4[] = { { 682,436 },{ 643,342 },{ 670,321 },{ 727,436 } };
	//���㶥�����
	int n1 = sizeof(space1) / sizeof(Point);
	int n2 = sizeof(space2) / sizeof(Point);
	int n3 = sizeof(space3) / sizeof(Point);
	int n4 = sizeof(space4) / sizeof(Point);

	for (int i = 0; i < n1; i++)
	{
		space1[i].x = space1[i].x*k*h+x0;
		space1[i].y = -space1[i].y*k*h+y0;
	}
	for (int i = 0; i < n2; i++)
	{
		space2[i].x = space2[i].x*k*h + x0;;
		space2[i].y = -space2[i].y*k*h+y0;
	}
	for (int i = 0; i < n3; i++)
	{
		space3[i].x = space3[i].x*k*h+x0;
		space3[i].y = -space3[i].y*k*h+y0;
	}
	for (int i = 0; i < n4; i++)
	{
		space4[i].x = space4[i].x*k*h+x0;
		space4[i].y = -space4[i].y*k*h+y0;
	}
	//ʹ�ú�ɫ��������εı߿�
	glColor3ub(0, 0, 0);
	DrawPolygon(space1, n1);
	DrawPolygon(space2, n2);
	DrawPolygon(space3, n3);
	DrawPolygon(space4, n4);
	glFlush();

	//ָ�����С
	glPointSize(1.0f);

	//ʹ�ú�ɫ����֮���Ϊ����ɫ���������
	glColor3ub(0, 0, 0);
	Fill(space1, n1);
	Fill(space2, n2);
	Fill(space3, n3);
	Fill(space4, n4);
	glFlush();
	
	//water flower
	glColor3ub(156, 12, 19);
	glPointSize(2.0);
	bresenhamArc(20, 0, 0, 4);
	bresenhamArc(20, 0, 0, 5);
	bresenhamArc(20, 25, 0, 4);
	bresenhamArc(20, 25, 0, 5);
	bresenhamArc(20, -25, 0, 4);
	bresenhamArc(20, -25, 0, 5);
	bresenhamArc(20, 50, 0, 4);
	bresenhamArc(20, 50, 0, 5);
	bresenhamArc(20, -50, 0, 4);
	bresenhamArc(20, -50, 0, 5);
	glPointSize(1.0);
	bresenham_line(-37, -30, -37, -50);
	bresenham_line(-16, -30, -16, -40);
	bresenham_line(-8, -30, -8, -50);
	bresenham_line(-16, -30, -8, -30);
	bresenham_line(-16, -40, -8, -40);
	bresenham_line(-16, -50, -8, -50);

	bresenham_line(8, -30, 8, -50);
	bresenham_line(16, -30, 16, -50);
	bresenham_line(8, -30, 16, -30);
	bresenham_line(8, -50, 16, -50);

	bresenham_line(37, -30, 37, -50);

	glFlush();


	//���� ����
	glColor3f(1.0, 1.0, 1.0);
	glPointSize(3.0f);
	for (int i = -0.4*w; i <= 0.4*w; i = i + 0.1*w)
	{
		for (int j = 1; j < 9; j++)
		{
			bresenhamArc(0.04*h, i, -0.45*h, j);
			bresenhamArc(0.04*h, i, 0.45*h, j);
		}
	}
	for (int i = -0.4*w; i <= 0.4*w; i = i + 0.2*w)
	{
		for (int j = 1; j < 9; j++)
		{
			bresenhamArc(0.04*h, i, -0.15*h, j);
			bresenhamArc(0.04*h, i, 0.15*h, j);
		}
	}
	for (int i = -0.3*w; i < 0.4*w; i = i + 0.6*w)
	{
		for (int j = 1; j < 9; j++)
		{
			bresenhamArc(0.04*h, i, -0.25*h, j);
			bresenhamArc(0.04*h, i, 0.25*h, j);
		}
	}
	glFlush();
	


}

//main
int main(int argc, char** argv)
{
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(700, 700);
	glutInitWindowPosition(500, 0);
	glutCreateWindow("Chinese ChessBoard");
	glutDisplayFunc(display);
	myinit(1400,1400);
	glutMainLoop();
}


