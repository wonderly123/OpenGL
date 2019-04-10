// lab1_2.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "stdafx.h"
#include <gl/glut.h>
#include <math.h>
#include "stdlib.h"

//�������
#include <iostream>
using namespace std;

//��ʼ�����ڽ���
void myinit(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	gluOrtho2D(0.0, 500.0, 0.0, 500.0);
	glMatrixMode(GL_MODELVIEW);
}

//DDA����ֱ��
//void dda_line(int xa, int ya, int xb, int yb) {
//	GLfloat delta_x, delta_y, x, y;
//	int dx, dy, steps;
//	dx = xb - xa;
//	dy = yb - ya;
//	if (abs(dx)>abs(dy))
//		steps = abs(dx);
//	else
//		steps = abs(dy);
//	delta_x = (GLfloat)dx / (GLfloat)steps;
//	delta_y = (GLfloat)dy / (GLfloat)steps;
//	x = xa;
//	y = ya;
//	glClear(GL_COLOR_BUFFER_BIT);
//	glBegin(GL_POINTS);
//	glVertex3f(x, y, 0);
//
//	for (int i = 0; i <= steps; i++)
//	{
//		x += delta_x;
//		y += delta_y;
//		glBegin(GL_POINTS);
//		glVertex3f(x, y, 0);
//		glEnd();
//	}
//}

//����������ֱ��
//void sign_line(int xa, int ya, int xb, int yb)
//{
//	int x = xa, y = ya;
//	int a = ya - yb, b = xb - xa;
//	int cx, cy;
//	if (b >= 0)
//	{
//		//xb >= xa�����
//		cx = 1;
//		//�������һ�
//	}
//	else {
//		// x2 < x1 ����� 
//		b = -b; //ʹ b>=0 
//		cx = -1; // �������� 
//	} if (a <= 0) {
//		//y2 >= y1 ����� 
//		cy = 1; // �������ϻ�
//	}
//	else {
//		//y2 < y1 ����� 
//		a = -a; //ʹ a<=0 
//		cy = -1; // �������»� 
//	}
//	int d, d1, d2;
//	glBegin(GL_POINTS);
//	if (-a <= b) // б�ʾ���ֵ <= 1 
//	{
//		d = 2 * a + b;
//		d1 = 2 * a;
//		d2 = 2 * (a + b);
//		while (x != xb)
//		{
//			if (d < 0)
//			{//ȥ���Ϸ����ص� 
//				y += cy;
//				d += d2;
//			}
//			else {
//				//ȡ���·����ص� 
//				d += d1;
//			}
//			x += cx;
//			glVertex2f(x, y);
//		}
//	}
//	else
//		// б�ʾ���ֵ > 1 
//	{
//		//�����������ֱ��y=xΪ��Գ�
//		//�����ɽ�б�ʾ���ֵ����1�����ת��Ϊб�ʵľ���ֵС��1����� 
//		d = 2 * b + a;
//		d1 = 2 * a;
//		d2 = 2 * (a + b);
//		while (y != yb)
//		{
//			if (d < 0)
//			{
//				d += d1;
//			}
//			else
//			{
//				x += cx;
//				d += d2;
//			}
//			y += cy;
//			glVertex2f(x, y);
//		}
//	}
//	glEnd();
//}

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

//����
void gl_Point(int x, int y) {
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
}

////����������ʵ�ֻ�Բ�� �������Ϊ�뾶,Բ�����꣬����
//void Arc_Sign(int radius, int centerX, int centerY, int area) {
//	int x, y, f;
//	x = 0; y = 0 + radius; f = 0;
//	while (y>0) {
//		switch (area) {
//		case 1:
//			gl_Point(x + centerX, y + centerY);
//			break;
//		case 2:
//			gl_Point(-x + centerX, y + centerY);
//			break;
//		case 3:
//			gl_Point(-x + centerX, -y + centerY);
//			break;
//		case 4:
//			gl_Point(x + centerX, -y + centerY);
//			break;
//		}
//		if (f>0) {
//			f = f - 2 * y + 1;
//			y = y - 1;
//		}
//		else {
//			f = f + 2 * x + 1;
//			x = x + 1;
//		}
//	}
//	if (y == centerY) {
//		gl_Point(x, y);
//	}
//}

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
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	
	//���̸� �� ֱ��
	glColor3f(1.0, 1.0, 1.0);
	glPointSize(3.0f);
	bresenham_line(45, 20, 45,480);
	bresenham_line(45, 480, 455, 480);
	bresenham_line(455, 480, 455, 20);
	bresenham_line(455, 20, 45, 20);

	glPointSize(1.0f);
	bresenham_line(50, 25, 50, 475);
	bresenham_line(50, 475, 450, 475);
	bresenham_line(450, 475, 450, 25);
	bresenham_line(450, 25, 50, 25); 

	for (int i = 75; i < 426; i=i+50)
	{
		bresenham_line(50, i, 450, i);
	}



	for (int i = 100; i < 401; i+=50)
	{
		bresenham_line(i, 25, i, 225);
		bresenham_line(i, 275, i, 475);
	}
	//���̸� �� б��
	bresenham_line(200, 25, 300, 125);
	bresenham_line(200, 125, 300, 25);
	bresenham_line(200, 475, 300, 375);
	bresenham_line(200, 375, 300, 475);
		
	//���� ����
	glPointSize(3.0f);
	for (int i = 50; i < 451; i=i+50)
	{
		for (int j = 1; j < 9; j++)
		{
			bresenhamArc(20, i, 25, j);
			bresenhamArc(20, i, 475, j);
		}
	}
	for (int i = 50; i < 451; i = i + 100)
	{
		for (int j = 1; j < 9; j++)
		{
			bresenhamArc(20, i, 225, j);
			bresenhamArc(20, i, 275, j);
		}
	}
	for (int i = 100; i < 401; i = i + 300)
	{
		for (int j = 1; j < 9; j++)
		{
			bresenhamArc(20, i, 125, j);
			bresenhamArc(20, i, 375, j);
		}
	}
	glFlush();

	
}

//���������
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(600, 50);
	glutCreateWindow("Chinese ChessBoard");
	glutDisplayFunc(display);
	myinit();
	glutMainLoop();
}


