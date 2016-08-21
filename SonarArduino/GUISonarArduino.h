#if !defined (GUISonarArduinoH)
#define GUISonarArduinoH

#include <windows.h>
#include "glut.h"

namespace guiSAhH
{
	float width = 1200;
	float height = 600;

	//����������
	int distance = 0;
	int maxDistance = 158;

	//��� ������
	enum TypeSonar { line, circle };
	TypeSonar typeSonar = circle;

	//����� ������
	enum AngleSonar { coverage180 = 180, coverage360 = 360 };
	AngleSonar angleSonar = coverage180;

	//��������������� ���� ��� ��������� ��� �������
	float scalePixel = 1;

	//������� ���� ������
	int angle = 0;

	//������� ��������
	int *objectAngle;
	int *objectAngleOld;

	struct Color
	{
		float r;
		float g;
		float b;

		Color(float r, float g, float b) : r(r), g(g), b(b)
		{}
	};

	struct Vector2i
	{
		int x, y;

		Vector2i(int x, int y) : x(x), y(y)
		{}
	};

	struct Vector2f
	{
		float x, y;

		Vector2f(float x, float y) : x(x), y(y)
		{}
	};

	class Circle
	{
		Vector2i position;
		Vector2f angle;
		int _r;
		Color _color;

	public:
		Circle(Vector2i position, Vector2f angle, int r, Color color) : position(position), angle(angle), _r(r), _color(color)
		{}

		//��������� �����/��������� 
		void Draw(Vector2i, Vector2f, int, int);

		//��������� ����� (�������) �� ������
		void DrawPoint(Vector2f, Color, float, int, int, int);

		//��������� ��������
		void DrawObject(TypeSonar typeSonar);

		//��������� �����  ��� �������
		void DrawCircle(Vector2f, int);

		//��������� �����
		void DrawLine(int angle);
	};

	void Circle::Draw(Vector2i position, Vector2f angle, int step, int r)
	{
		glColor3f(_color.r, _color.g, _color.b);
		glBegin(GL_TRIANGLE_FAN);

		glVertex2f(position.x, position.y);
		for (int i = angle.x; i <= angle.y; i += step) {
			float a = (float)i / 57.2974;
			glVertex2f(cos(a) * r, sin(a) * r + position.y);
		}
		glEnd();
	}

	void Circle::DrawPoint(Vector2f angle, Color color, float rad, int distance, int step, int r)
	{
		glColor3f(color.r, color.g, color.b);
		glBegin(GL_TRIANGLE_FAN);

		glVertex2f(cos(rad) * (distance * scalePixel), sin(rad) * (distance * scalePixel) + this->position.y);
		for (int j = angle.x; j <= angle.y; j += step)
		{
			float a = (float)j / 57.2974;
			glVertex2f(cos(a) * r + cos(rad) * (distance * scalePixel), sin(a) * r + sin(rad) * (distance * scalePixel) + this->position.y);
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

			for (int i = 0; i < angleSonar; i++)
			{
				float a = (float)i / 57.2974;

				//������������ �����, ������ ���� ��� ������ � ������� ��������� ������
				if (objectAngle[i] > maxDistance)
					objectAngle[i] = maxDistance;

				//��������� ����������
				glVertex2f(cos(a) * (objectAngle[i] * scalePixel), sin(a) * (objectAngle[i] * scalePixel) + this->position.y);
				glVertex2f(cos(a + 0.0174) * (objectAngle[i] * scalePixel), sin(a + 0.0174) * (objectAngle[i] * scalePixel) + this->position.y);

				//�������� ����������
				glVertex2f(cos(a + 0.0174) * _r, sin(a + 0.0174) * _r + this->position.y);
				glVertex2f(cos(a) * _r, sin(a) * _r + this->position.y);
			}
			glEnd();
			break;
		case (circle):
			for (int i = 0; i < angleSonar; i++)
			{
				//�������� � ��������
				float rad = (float)i / 57.2974;

				//������������ �����, ������ ���� ��� ������ � ������� ��������� ������
				if (objectAngle[i] > maxDistance)
					objectAngle[i] = maxDistance;

					//���� ������ ������� ���������� - ���������� ����������� ���������
					if (objectAngle[i] != objectAngleOld[i])
						this->DrawPoint(Vector2f(0, 360), Color(0, 0, 1), rad, objectAngleOld[i], 60, 4);

					//���������� ������ ������
					this->DrawPoint(Vector2f(0, 360), Color(1, 0, 0), rad, objectAngle[i], 60, 4);
			}
			break;
		}
	}

	void Circle::DrawCircle(Vector2f angle, int r)
	{
		glColor3f(_color.r, _color.g, _color.b);
		glBegin(GL_LINE_LOOP);

		for (int i = angle.x; i <= angle.y; i++) {
			float a = (float)i / 57.2974; //�������� � ��������
			glVertex2f(cos(a) * r, sin(a) * r + this->position.y);
		}
		glEnd();
	}

	void Circle::DrawLine(int angle)
	{
		glColor3f(_color.r, _color.g, _color.b);
		glBegin(GL_LINES);
			glVertex2f(this->position.x, this->position.y);
			float a = (float)angle / 57.2974;
			glVertex2f(cos(a) * _r, sin(a) * _r + this->position.y);
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
		for (int i = 0; i < angleSonar; i++)
		{
			objectAngle[i] = maxDistance * scalePixel;
			objectAngleOld[i] = maxDistance * scalePixel;
		}
	}

	void resize(int w, int h)
	{
		width = w;
		height = h;

		switch (angleSonar)
		{
		case (coverage360):
			scalePixel = (height / 2 * 0.9f) / maxDistance;
			break;
		default:
			scalePixel = (height * 0.9f) / maxDistance;
			break;
		}
	}

	void display()
	{
		int height90 = height * 0.9f;

		glClear(GL_COLOR_BUFFER_BIT);

		//��������� ����������� ������ �� ���� 
		if (angleSonar == coverage360)
		{
			//��� ������
			Circle Back(Vector2i(0, 0), Vector2f(0, 360), height90 / 2, Color(0, 0.8, 0));
				Back.Draw(Vector2i(0, 0), Vector2f(0, 360), 5, height90 / 2);

			//����������� ��������
				Back.DrawObject(typeSonar);

			//�������� ������ (�����)
			Circle Marking(Vector2i(0, 0), Vector2f(0, 360), height90 / 2, Color(0.9, 0.9, 0.9));
				for (float i = 1; i >= 0.2f; i -= 0.2f)
					Marking.DrawCircle(Vector2f(0, 360), height90 / 2 * i);

			//�������� ������ (�����)
			for (float i = 0; i <= 360; i += 22.5f)
				Marking.DrawLine(i);

			//����� ���������� ������
			Circle Line(Vector2i(0, 0), Vector2f(angle, angle + 1), height90 / 2, Color(1, 1, 1));
				Line.Draw(Vector2i(0, 0), Vector2f(angle, angle + 1), 1, height90 / 2);

			Circle Line1(Vector2i(0, 0), Vector2f(angle, angle + 1), -height90 / 2, Color(1, 1, 1));
				Line1.Draw(Vector2i(0, 0), Vector2f(angle, angle + 1), 1, -height90 / 2);
		}
		else
		{
			//��� ������
			Circle Back(Vector2i(0, -height90 / 2), Vector2f(0, 180), height90, Color(0, 0.8, 0));
			Back.Draw(Vector2i(0, -height90 / 2), Vector2f(0, 180), 5, height90);

			//����������� ��������
			Back.DrawObject(typeSonar);

			//�������� ������ (�����)
			Circle Marking(Vector2i(0, -height90 / 2), Vector2f(0, 180), height90, Color(0.9, 0.9, 0.9));
			for (float i = 1; i >= 0.2f; i -= 0.2f)
				Marking.DrawCircle(Vector2f(0, 180), height90 * i);

			//�������� ������ (�����)
			for (float i = 0; i <= 180; i += 22.5f)
				Marking.DrawLine(i);

			//����� ���������� ������
			Circle Line(Vector2i(0, -height90 / 2), Vector2f(angle, angle + 1), height90, Color(1, 1, 1));
				Line.Draw(Vector2i(0, -height90 / 2), Vector2f(angle, angle + 1), 1, height90);
		}

		glutSwapBuffers();
	}
}
#endif