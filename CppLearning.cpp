#include "stdafx.h"
#include <windows.h>
#include <gl\glut.h>

#include <ctime>
#include <iostream>

#include <stdio.h>
#include <conio.h>
#include <locale.h>

//������ � ������
#include <cstdlib>

float width = 1200;
float height = 600;
float scalePixel = 1;

//������� ���� ������
int angle = 0;

//��� ������
enum TypeSonar {line, circle};
TypeSonar typeSonar;

//����������
int distance = 0;
int maxDistance = 250;

//������� ��������
int objectAngle[180];
int objectAngleOld[180];

struct Color
{
	float r;
	float g;
	float b;

	Color(float r, float g, float b) : r(r), g(g), b(b)
	{}
};

class Circle
{
	int _x;
	int _y;
	int _angleStart;
	int _angleEnd;
	int _r;
	Color _color;

public:
	Circle(int x, int y, int angleStart, int angleEnd, int r, Color color) : _x(x), _y(y), _angleStart(angleStart), _angleEnd(angleEnd), _r(r), _color(color)
	{}

	//��������� ����� 
	void Draw();

	//��������� ��������
	void DrawObject(TypeSonar typeSonar);

	//��������� �����  ��� �������
	void DrawCircle(int r);

	//��������� �����
	void DrawLine(int angle);
};

void Circle::Draw()
{
	glColor3f(_color.r, _color.g, _color.b);
	glBegin(GL_TRIANGLE_FAN);

	glVertex2f(0, _y); //�����
	for (int i = _angleStart; i <= _angleEnd; i++) {
		float a = (float)i / 57.2974; //�������� � ��������
		glVertex2f(cos(a) * _r, sin(a) * _r + _y);
	}
	glEnd();
}

void Circle::DrawObject(TypeSonar typeSonar)
{
	switch (typeSonar)
	{
		case (line):
			glColor3f(1, 0.138, 0.138);
			glBegin(GL_QUADS);

			for (int i = 0; i < 180; i++) {
				float a = (float)i / 57.2974; //�������� � ��������

				//��������� ����������
				glVertex2f(cos(a) * (objectAngle[i] * scalePixel), sin(a) * (objectAngle[i] * scalePixel) + _y);
				glVertex2f(cos(a + 0.0174) * (objectAngle[i] * scalePixel), sin(a + 0.0174) * (objectAngle[i] * scalePixel) + _y);

				//�������� ����������
				glVertex2f(cos(a + 0.0174) * _r, sin(a + 0.0174) * _r + _y);
				glVertex2f(cos(a) * _r, sin(a) * _r + _y);
			}
			glEnd();
		break;
		case (circle) :
			for (int i = 0; i < 180; i++) {
				float a = (float)i / 57.2974; //�������� � ��������

				//���� ������ ������� ����������
				if (objectAngle[i] != objectAngleOld[i])
				{
					glColor3f(0, 0, 1);
					glBegin(GL_TRIANGLE_FAN);
					//���������� ����������� ���������
					glVertex2f(cos(a) * (objectAngleOld[i] * scalePixel), sin(a) * (objectAngleOld[i] * scalePixel) + _y); //�����
					for (int j = 0; j <= 360; j++) {
						float ra = (float)j / 57.2974; //�������� � ��������
						glVertex2f(cos(ra) * 5 + cos(a) * (objectAngleOld[i] * scalePixel), sin(ra) * 5 + sin(a) * (objectAngleOld[i] * scalePixel) + _y);
					}
					glEnd();
				}

				glColor3f(1, 0, 0);
				glBegin(GL_TRIANGLE_FAN);
				//���������� ������ ������
				glVertex2f(cos(a) * (objectAngle[i] * scalePixel), sin(a) * (objectAngle[i] * scalePixel) + _y); //�����
				for (int j = 0; j <= 360; j++) {
					float ra = (float)j / 57.2974; //�������� � ��������
					glVertex2f(cos(ra) * 5 + cos(a) * (objectAngle[i] * scalePixel), sin(ra) * 5 + sin(a) * (objectAngle[i] * scalePixel) + _y);
				}
				glEnd();
			}
		break;
	}
}

void Circle::DrawCircle(int r)
{
	glColor3f(_color.r, _color.g, _color.b);
	glBegin(GL_LINE_LOOP);

	for (int i = _angleStart; i <= _angleEnd; i++) {
		float a = (float)i / 57.2974; //�������� � ��������
		glVertex2f(cos(a) * r, sin(a) * r + _y);
	}
	glEnd();
}

void Circle::DrawLine(int angle)
{
	glColor3f(_color.r, _color.g, _color.b);
	glBegin(GL_LINES);
		glVertex2f(_x, _y);
		float a = (float)angle / 57.2974; //�������� � ��������
		glVertex2f(cos(a) * _r, sin(a) * _r + _y);
	glEnd();
}

void Initialize()
{
	glClearColor(0, 0, 0, 1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-width / 2, width / 2, -height / 2, height / 2);
	glMatrixMode(GL_MODELVIEW);

	//������������� ������� ��������
	for (int i = 0; i < 180; i++)
	{
		objectAngle[i] = maxDistance * scalePixel;
		objectAngleOld[i] = maxDistance * scalePixel;
	}
}

void resize(int w, int h)
{
	width = w;
	height = h;
	scalePixel = (height * 0.9f) / maxDistance;
}

void display()
{
	int height90 = height * 0.9f;

	glClear(GL_COLOR_BUFFER_BIT);

	//��� ������
		Circle Back(0, -height90 / 2, 0, 180, height90, Color(0, 0.8, 0));
		Back.Draw();

	//����������� ��������
		Back.DrawObject(circle);

	//�������� ������ (�����)
		Circle Marking(0, -height90 / 2, 0, 180, height90, Color(0.9, 0.9, 0.9));
		Marking.DrawCircle(height90);
		Marking.DrawCircle(height90 * 0.8);
		Marking.DrawCircle(height90 * 0.6);
		Marking.DrawCircle(height90 * 0.4);
		Marking.DrawCircle(height90 * 0.2);

	//�������� ������ (�����)
		Marking.DrawLine(0);
		Marking.DrawLine(22);
		Marking.DrawLine(45);
		Marking.DrawLine(67);
		Marking.DrawLine(90);
		Marking.DrawLine(112);
		Marking.DrawLine(135);
		Marking.DrawLine(157);
		Marking.DrawLine(180);

	//����� ���������� ������
		Circle Line(0, -height90 / 2, angle, angle + 1, height90, Color(1, 1, 1));
		Line.Draw();

	glutSwapBuffers();
}

//��������
void Timer(int)
{
	glutPostRedisplay();
	glutTimerFunc(5, Timer, 0);

	//��������� ������ � ������
		angle += 1;
		
		if (angle > 179)
		{
			angle = 0;
			for (int i = 0; i < 180; i++)
			{
				objectAngleOld[i] = objectAngle[i];
			}
		}

		distance = -rand() % 175 + maxDistance;
		objectAngle[angle] = distance;
}

int main(int argc, char * argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_STENCIL);

	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Sonar");
	Initialize();

	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutTimerFunc(5, Timer, 0);

	std::cout << "An image built in " << clock() << " ms";

	//��������� ������

	glutMainLoop();
	return 0;
}