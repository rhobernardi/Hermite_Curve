#include <iostream>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>


//Classe "Point".
class Point
{
	public:
		float x, y;
		void setxy(float x2, float y2)
		{
			x = x2; y = y2;
		}
		//Sobrecarga do operador "=" para que não seja atribuído apenas "ponteiros".
		const Point & operator=(const Point &rPoint)
		{
			x = rPoint.x;
			y = rPoint.y;
			return *this;
		}
};



using namespace std;

int SCREEN_HEIGHT = 500;
int nPts = 2; // Número de pontos da reta, P0 e P1.
int N = nPts-1;
int contPoints = 0;
int flag = 0;
Point PtsDeControle[4]; // [P0, P1, V0, V1]


void drawDot(int x, int y)
{
	glColor3f(0.0, 0.0, 0.0); // pontos pretos para facilitar a visualização
	glBegin(GL_POINTS);
		glVertex2i(x,y);
	glEnd();

	glFlush();
}


void drawLine(Point p1, Point p2)
{
	glColor3f(0.0, 0.0, 1.0); // linha azul
	glBegin(GL_LINES);
		glVertex2f(p1.x, p1.y);
		glVertex2f(p2.x, p2.y);
	glEnd();

	glFlush();
}


void drawLineVector(Point p1, Point p2)
{
	glColor3f(0.0, 1.0, 0.0); // linha verde
	glBegin(GL_LINES);
		glVertex2f(p1.x, p1.y);
		glVertex2f(p2.x, p2.y);
	glEnd();

	glFlush();
}


void calculateSpeedVectors(Point p1, Point p2)
{
	float dx = (float)abs(p1.x-p2.x);
	float dy = (float)abs(p1.y-p2.y);

	if (contPoints == 2)
	{
		dx = dx/3;
		dy = dy/3;
	}

	float vx1, vx2, vy1, vy2;

	if (p1.x < p2.x) {
		vx1 = (float)p1.x + (dx);
		vx2 = (float)p2.x + (dx); // -
	} else {
		vx1 = (float)p1.x + (dx); // -
		vx2 = (float)p2.x + (dx);
	}

	if (p1.y < p2.y) {
		vy1 = (float)p1.y + (dy);
		vy2 = (float)p2.y + (dy); // -
	} else {
		vy1 = (float)p1.y + (dy); // -
		vy2 = (float)p2.y + (dy);
	}

	PtsDeControle[2].setxy((float) vx1, (float) vy1);
	PtsDeControle[3].setxy((float) vx2, (float) vy2);

	glutPostRedisplay();
}


Point calculateCurve(Point *PT, double u)
{
	Point P0, P1, V0, V1;
	Point H;

	P0.setxy(PT[0].x, PT[0].y);
	P1.setxy(PT[1].x, PT[1].y);
	V0.setxy(PT[2].x, PT[2].y);
	V1.setxy(PT[3].x, PT[3].y);

	H.x = P0.x*(2*pow(u, 3) - 3* pow(u, 2) +1) + P1.x*(-2* pow(u, 3) + 3* pow(u,2)) + 
			V0.x*(pow(u, 3) - 2* pow(u, 2) + u) + V1.x*(pow(u,3) - pow(u,2));
	
	H.y = P0.y*(2*pow(u, 3) - 3* pow(u, 2) +1) + P1.y*(-2* pow(u, 3) + 3* pow(u,2)) + 
			V0.y*(pow(u, 3) - 2* pow(u, 2) + u) + V1.y*(pow(u,3) - pow(u,2));

	return H;
}

void drawHermiteCurve(Point PT[])
{
	Point paux1, paux2;

	paux1 = PT[0];
	for (double u = 0; u < 1.0; u+=0.01)
	{
		paux2 = calculateCurve(PT, u);

		drawLine(paux1, paux2);

		paux1 = paux2;
	}

}


void keyPressedEvent(unsigned char key, int x, int y)
{
	if (key == 27) exit(0);
}


void mousePassEvent(int x, int y)
{
	if (flag == 1)
	{
		PtsDeControle[1].setxy(x, SCREEN_HEIGHT - y);
		glutPostRedisplay();
	}

	if (flag == 2)
	{
		PtsDeControle[2].setxy(x, SCREEN_HEIGHT - y);
		glutPostRedisplay();
	}

	if (flag == 3)
	{
		PtsDeControle[3].setxy(x, SCREEN_HEIGHT - y);
		glutPostRedisplay();
	}
}


void mouseClickEvent(int button, int state, int x, int y)
{	
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (contPoints < 2)
		{
			PtsDeControle[contPoints].setxy((float) x, (float) (SCREEN_HEIGHT - y));
			drawDot(x, SCREEN_HEIGHT - y);
			contPoints++;

			// ativa flag que desenha linha acompanhando o rastro do mouse
			if(contPoints == 1) flag = 1;
			else flag = 0;

			if (contPoints == nPts)
			{
				glColor3f(0.0, 0.0, 1.0);
				
				drawLine(PtsDeControle[0], PtsDeControle[1]);
				calculateSpeedVectors(PtsDeControle[0], PtsDeControle[1]);
			}
		}

		else if(flag != 0 && state == GLUT_DOWN) flag = 0;

		else
		{
			if ((x >= PtsDeControle[2].x - 2 && x <= PtsDeControle[2].x + 2) && 
				(y >= SCREEN_HEIGHT - PtsDeControle[2].y - 2 && y <= SCREEN_HEIGHT - PtsDeControle[2].y + 2) &&
				state == GLUT_DOWN)
			{
				flag = 2;
			}

			else if ((x >= PtsDeControle[3].x - 2 && x <= PtsDeControle[3].x + 2) && 
				(y >= SCREEN_HEIGHT - PtsDeControle[3].y - 2 && y <= SCREEN_HEIGHT - PtsDeControle[3].y + 2) &&
				state == GLUT_DOWN)
			{
				flag = 3;
			}			
		}
	}
}

// Funções de cabeçalho.
void Init()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glColor3f(0.0, 0.0, 0.0);
	glPointSize(3);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 640.0, 0.0, 500.0);
}

void Display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	drawDot(PtsDeControle[0].x, PtsDeControle[0].y);
	drawDot(PtsDeControle[1].x, PtsDeControle[1].y);
	drawDot(PtsDeControle[2].x, PtsDeControle[2].y);
	drawDot(PtsDeControle[3].x, PtsDeControle[3].y);

	if (flag == 1)
	{
		drawLine(PtsDeControle[0], PtsDeControle[1]);
	}

	if(flag != 1)
	{
		drawLineVector(PtsDeControle[0], PtsDeControle[2]);
		drawLineVector(PtsDeControle[1], PtsDeControle[3]);
		drawHermiteCurve(PtsDeControle);
	}

	glFlush();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(640,500);
	glutInitWindowPosition(100,150);
	glutCreateWindow("Hermite Curve");
	
	Init();
	glutDisplayFunc(Display);
	glutPassiveMotionFunc(mousePassEvent);
	glutMouseFunc(mouseClickEvent);
	glutKeyboardFunc(keyPressedEvent);

	glutMainLoop();

	return 0;
}
