#include <iostream>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>


using namespace std;

int SCREEN_HEIGHT = 500;
int nPts = 5; // Número de pontos de controle.
int N = nPts-1;
int contPoints = 0;

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

Point PtsDeControle[20];

int fatorial(int n)
{
	if (n<=1)
		return(1);
	else
		n=n*fatorial(n-1);
	
	return n;
}

float binomio_coef(int n,int k)
{
	return fatorial(n) / (fatorial(k)*fatorial(n-k));
}

void drawDot(int x, int y)
{
	glBegin(GL_POINTS);
		glVertex2i(x,y);
	glEnd();

	glFlush();
}

void drawLine(Point p1, Point p2)
{
	glBegin(GL_LINES);
		glVertex2f(p1.x, p1.y);
		glVertex2f(p2.x, p2.y);
	glEnd();

	glFlush();
}


Point drawBezierGeneralized(Point PT[], double u)
{
	Point P;
	P.x = 0; P.y = 0;

	for (int k = 0; k <= N; ++k)
	{
		P.x += binomio_coef(N,k)*pow((1-u),N-k)*pow(u,k)*PT[k].x;
		P.y += binomio_coef(N,k)*pow((1-u),N-k)*pow(u,k)*PT[k].y;
	}
}


void myMouse(int button, int state, int x, int y)
{
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
	{
		PtsDeControle[contPoints].setxy((float) x, (float) (SCREEN_HEIGHT - y));
		drawDot(x, SCREEN_HEIGHT - y);

		if (contPoints == nPts)
		{
			glColor3f(0.2, 1.0, 0.0);

			for (int i = 0; i < nPts; ++i)
			{
				drawLine(PtsDeControle[i], PtsDeControle[i+1]);
			}

			glColor3f(0.0, 0.0, 1.0);
			Point Paux1 = PtsDeControle[0];

			for (double u = 0; u <= 1.0; u+=0.01)
			{
				Point Paux2 = drawBezierGeneralized(PtsDeControle, u);
				drawLine(Paux1, Paux2);

				Paux1 = Paux2;
			}

			//contPoints = 0;
		}
	}
}


// Funções de cabeçalho.
void myInit()
{
	glClearColor(1.0,1.0,1.0,0.0);
	glColor3f(0.0,0.0,0.0);
	glPointSize(3);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0,640.0,0.0,500.0);

}

void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(640,500);
	glutInitWindowPosition(100,150);
	glutCreateWindow("Bezier Curve");
	glutMouseFunc(myMouse);
	glutDisplayFunc(myDisplay);
	myInit();
	glutMainLoop();

	return 0;
}
