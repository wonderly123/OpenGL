// lab1_2.cpp : �������̨Ӧ�ó������ڵ㡣
//

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

//����
void gl_Point(int x, int y) {
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
}

//����������ʵ�ֻ�Բ�� �������Ϊ�뾶,Բ�����꣬����
void Arc_Sign(int radius,int centerX,int centerY,int area){
	int x,y,f;
	x=0;y=0+radius;f=0;
	while(y>0){
		switch(area){
		case 1:
			gl_Point(x+centerX,y+centerY);
			break;
		case 2:
			gl_Point(-x+centerX,y+centerY);
			break;
		case 3:
			gl_Point(-x+centerX,-y+centerY);
			break;
		case 4:
			gl_Point(x+centerX,-y+centerY);
			break;
		}
		if(f>0){
			f=f-2*y+1;
			y=y-1;
		}else{
			f=f+2*x+1;
			x=x+1;
		}
	}
	if(y==centerY){
		gl_Point(x,y);
	}
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
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 1.0);
	Arc_Sign(500, 0, 0, 1);
	glColor3f(0.0, 1.0, 0.0);
	bresenhamArc(420, 20, 20, 1);
	glColor3f(0.0, 1.0, 1.0);
	Arc_Sign(340, 40, 40, 1);
	glColor3f(1.0, 0.0, 0.0);
	bresenhamArc(260, 60, 60, 1);
	glColor3f(1.0, 0.0, 1.0);
	Arc_Sign(180, 80, 80, 1);
	glColor3f(1.0, 1.0, 0.0);
	bresenhamArc(100, 100, 100, 1);
	glColor3f(1.0, 1.0, 1.0);
	Arc_Sign(20, 120, 120, 1);
	glFlush();
}

//���������
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(600, 50);
	glutCreateWindow("DrawArcs");
	glutDisplayFunc(display);
	myinit();
	glutMainLoop();
}


