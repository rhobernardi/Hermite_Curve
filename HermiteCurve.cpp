#include <iostream>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>


using namespace std;

int SCREEN_HEIGHT = 500;
int nPts = 2; // Número de pontos de controle.
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
	glColor3f(0.0, 0.0, 0.0);
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

void calculateVelLine(Point p1, Point p2)
{
	int dx = (int)abs(p1.x-p2.x);
	int dy = (int)abs(p1.y-p2.y);

	int vx1, vx2, vy1, vy2;

	if (p1.x < p2.x) {
		vx1 = (int)p1.x + (dx/3);
		vx2 = (int)p2.x - (dx/3);
	} else {
		vx1 = (int)p1.x - (dx/3);
		vx2 = (int)p2.x + (dx/3);
	}

	if (p1.y < p2.y) {
		vy1 = (int)p1.y + (dy/3);
		vy2 = (int)p2.y - (dy/3);
	} else {
		vy1 = (int)p1.y - (dy/3);
		vy2 = (int)p2.y + (dy/3);
	}

	// cout << "p1.x: " << p1.x << "\tp2.x: " << p2.x << endl;
	// cout << "p1.y: " << p1.y << "\tp2.y: " << p2.y << "\n" << endl;

	// cout << "dx: " << dx/3 << endl;
	// cout << "dy: " << dy/3 << "\n" << endl;

	// cout << "v1.x: " << vx1 << "\tv2.x: " << vx2 << endl;
	// cout << "v1.y: " << vy1 << "\tv2.y: " << vy2 << "\n" << endl;

	PtsDeControle[3].setxy((float) vx1, (float) vy1);
	PtsDeControle[4].setxy((float) vx2, (float) vy2);

	drawDot(PtsDeControle[3].x, PtsDeControle[3].y);
	drawDot(PtsDeControle[4].x, PtsDeControle[4].y);
}

Point drawHermiteCurve(Point PT[], double u)
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
		if (contPoints < 2)
		{
			PtsDeControle[contPoints].setxy((float) x, (float) (SCREEN_HEIGHT - y));
			drawDot(x, SCREEN_HEIGHT - y);
			contPoints++;

			if (contPoints == nPts)
			{
				glColor3f(0.2, 1.0, 0.0);

				// for (int i = 0; i < N; ++i)
				// {
				// 	drawLine(PtsDeControle[i], PtsDeControle[i+1]);
				// }
				
				drawLine(PtsDeControle[0], PtsDeControle[1]);
				calculateVelLine(PtsDeControle[0], PtsDeControle[1]);

				glColor3f(0.0, 0.0, 1.0);
				Point Paux1 = PtsDeControle[0];

				for (double u = 0; u <= 1.0; u+=0.01)
				{
					//Point Paux2 = drawHermiteCurve(PtsDeControle, u);
					//drawLine(Paux1, Paux2);

					//Paux1 = Paux2;
				}
			}
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
	glutCreateWindow("Hermite Curve");
	glutMouseFunc(myMouse);
	glutDisplayFunc(myDisplay);
	myInit();
	glutMainLoop();

	return 0;
}
