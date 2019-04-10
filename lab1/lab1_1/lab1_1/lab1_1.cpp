#include "stdafx.h"
#include <gl/glut.h>
#include "math.h"
#include "stdlib.h"

//��ʼ�����ڽ���
void myinit(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	gluOrtho2D(0.0, 500.0, 0.0, 500.0);
	glMatrixMode(GL_MODELVIEW);
}

//DDA����
void dda_line(int xa, int ya, int xb, int yb) {
	GLfloat delta_x, delta_y, x, y;
	int dx, dy, steps;
	dx = xb - xa;
	dy = yb - ya;
	if (abs(dx)>abs(dy))
		steps = abs(dx);
	else
		steps = abs(dy);

	delta_x = (GLfloat)dx / (GLfloat)steps;
	delta_y = (GLfloat)dy / (GLfloat)steps;
	x = xa;
	y = ya;
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POINTS);
	glVertex3f(x, y, 0);

	for (int i = 0; i <= steps; i++)
	{
		x += delta_x;
		y += delta_y;
		glBegin(GL_POINTS);
		glVertex3f(x, y, 0);
		glEnd();
	}
}

//����������ֱ��
void sign_line(int xa, int ya, int xb, int yb)
{
	int x = xa, y = ya;
	int a = ya - yb, b = xb - xa;
	int cx, cy;
	if (b >= 0)
	{
		//xb >= xa�����
		cx = 1;
		//�������һ�
	}
	else {
		// x2 < x1 ����� 
		b = -b; //ʹ b>=0 
		cx = -1; // �������� 
	} if (a <= 0) {
		//y2 >= y1 ����� 
		cy = 1; // �������ϻ�
	}
	else {
		//y2 < y1 ����� 
		a = -a; //ʹ a<=0 
		cy = -1; // �������»� 
	}
	int d, d1, d2;
	glBegin(GL_POINTS);
	if (-a <= b) // б�ʾ���ֵ <= 1 
	{
		d = 2 * a + b;
		d1 = 2 * a;
		d2 = 2 * (a + b);
		while (x != xb)
		{
			if (d < 0)
			{//ȥ���Ϸ����ص� 
				y += cy;
				d += d2;
			}
			else {
				//ȡ���·����ص� 
				d += d1;
			}
			x += cx;
			glVertex2f(x, y);
		}
	}
	else
		// б�ʾ���ֵ > 1 
	{
		//�����������ֱ��y=xΪ��Գ�
		//�����ɽ�б�ʾ���ֵ����1�����ת��Ϊб�ʵľ���ֵС��1����� 
		d = 2 * b + a;
		d1 = 2 * a;
		d2 = 2 * (a + b);
		while (y != yb)
		{
			if (d < 0)
			{
				d += d1;
			}
			else
			{
				x += cx;
				d += d2;
			}
			y += cy;
			glVertex2f(x, y);
		}
	}
	glEnd();
}
// ���� Bresenham �㷨��ֱ��
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
		glVertex2f(x, y);
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

//display����
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	//����DDAֱ�������㷨
	glColor3f(1.0, 1.0, 0.0);
	dda_line(50, 50, 200, 450);
	//����������ֱ�������㷨
	glColor3f(0.0, 1.0, 1.0);
	sign_line(50, 50, 400, 400);
	//����bresenham��ֱ�������㷨
	glColor3f(1.0, 0.0, 1.0);
	bresenham_line(50, 50, 450, 200);
	glFlush();


}

//���������
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(600, 50);
	glutCreateWindow("DrawLine");
	glutDisplayFunc(display);
	myinit();
	glutMainLoop();
}