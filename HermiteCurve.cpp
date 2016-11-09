#include <iostream>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>


using namespace std;

int SCREEN_HEIGHT = 500;
int nPts = 2; // Número de pontos da reta, P0 e P1.
int N = nPts-1;
int contPoints = 0;
int HermiteMatrix[4][4];

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

Point PtsDeControle[4]; // [P0, P1, V0, V1]


void loadHermiteMatrix()
{
	HermiteMatrix[0][0] =  1;
	HermiteMatrix[0][1] =  0;
	HermiteMatrix[0][2] = -3;
	HermiteMatrix[0][3] =  2;
	HermiteMatrix[1][0] =  0;			// [1   0  -3   2]
	HermiteMatrix[1][1] =  0;			// [0   0   3  -2]
	HermiteMatrix[1][2] =  3;			// [0   1  -2   1]
	HermiteMatrix[1][3] = -2;			// [0   0  -1   1]
	HermiteMatrix[2][0] =  0;
	HermiteMatrix[2][1] =  1;
	HermiteMatrix[2][2] = -2;
	HermiteMatrix[2][3] =  1;
	HermiteMatrix[3][0] =  0;
	HermiteMatrix[3][1] =  0;
	HermiteMatrix[3][2] = -1;
	HermiteMatrix[3][3] =  1;
}


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


void calculateVelLine(Point p1, Point p2)
{
	float dx = (float)abs(p1.x-p2.x);
	float dy = (float)abs(p1.y-p2.y);

	float vx1, vx2, vy1, vy2;

	if (p1.x < p2.x) {
		vx1 = (float)p1.x + (dx/3);
		vx2 = (float)p2.x - (dx/3);
	} else {
		vx1 = (float)p1.x - (dx/3);
		vx2 = (float)p2.x + (dx/3);
	}

	if (p1.y < p2.y) {
		vy1 = (float)p1.y + (dy/3);
		vy2 = (float)p2.y - (dy/3);
	} else {
		vy1 = (float)p1.y - (dy/3);
		vy2 = (float)p2.y + (dy/3);
	}

	// cout << "p1.x: " << p1.x << "\tp2.x: " << p2.x << endl;
	// cout << "p1.y: " << p1.y << "\tp2.y: " << p2.y << "\n" << endl;

	// cout << "dx: " << dx/3 << endl;
	// cout << "dy: " << dy/3 << "\n" << endl;

	// cout << "v1.x: " << vx1 << "\tv2.x: " << vx2 << endl;
	// cout << "v1.y: " << vy1 << "\tv2.y: " << vy2 << "\n" << endl;

	PtsDeControle[2].setxy((float) vx1, (float) vy1);
	PtsDeControle[3].setxy((float) vx2, (float) vy2);

	drawDot(PtsDeControle[2].x, PtsDeControle[2].y);
	drawDot(PtsDeControle[3].x, PtsDeControle[3].y);

	drawLineVector(PtsDeControle[0], PtsDeControle[2]);
	drawLineVector(PtsDeControle[1], PtsDeControle[3]);
}


Point *partialResult(Point *PT)
{
	Point *result = (Point *) calloc(4,sizeof(Point));

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			result[i].x += HermiteMatrix[i][j]*PT[j].x;
			result[i].y += HermiteMatrix[i][j]*PT[j].y;
		}
	}

	return result;
}


void drawHermiteCurve(Point *PT, double u)
{
	Point *P = (Point *) calloc(4, sizeof(Point));
	//P.x = 0; P.y = 0;

	Point *ParRes = partialResult(PT);

	for (int k = 0; k < 4; ++k)
	{
		P[k].x += 1*ParRes[0].x + u*ParRes[1].x + pow(u,2)*ParRes[2].x + pow(u,3)*ParRes[3].x; //binomio_coef(N,k)*pow((1-u),N-k)*pow(u,k)*PT[k].x;
		P[k].y += 1*ParRes[0].y + u*ParRes[1].y + pow(u,2)*ParRes[2].y + pow(u,3)*ParRes[3].y;//P.y += binomio_coef(N,k)*pow((1-u),N-k)*pow(u,k)*PT[k].y;
	}

	for (int i = 0; i < 4; ++i)
	{
		PT[i] = P[i];
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

			if (contPoints == nPts)
			{
				glColor3f(0.0, 0.0, 1.0);
				
				drawLine(PtsDeControle[0], PtsDeControle[1]);
				calculateVelLine(PtsDeControle[0], PtsDeControle[1]);
			}
		}

		else
		{
			cout << "x: " << x << "\ty: " << y << endl;

			if ((x >= PtsDeControle[2].x-2 && x <= PtsDeControle[2].x+2) && 
				(y >= SCREEN_HEIGHT - PtsDeControle[2].y-2 && y <= SCREEN_HEIGHT - PtsDeControle[2].y+2))
			{
				cout << "V1 PRESSED" << endl;

				glColor3f(0.0, 0.0, 1.0);

				//cout << "old DOT = x: " << PtsDeControle[2].x << "\ty: " << PtsDeControle[2].y << endl;
				//PtsDeControle[2].x += 4;
				//PtsDeControle[2].y += 4;

				//drawDot(PtsDeControle[2].x, PtsDeControle[2].y);

				//cout << "new DOT = x: " << PtsDeControle[2].x << "\ty: " << PtsDeControle[2].y << endl;


				for (double u = 0; u <= 1.0; u+=0.01)
				{
					drawHermiteCurve(PtsDeControle, u);
					
					drawLine(PtsDeControle[0], PtsDeControle[1]);
					
				}
				//cout << "newest DOT = x: " << PtsDeControle[2].x << "\ty: " << PtsDeControle[2].y << endl;
			}

			else if ((x >= PtsDeControle[3].x-2 && x <= PtsDeControle[3].x+2) && 
					(y >= SCREEN_HEIGHT - PtsDeControle[3].y-2 && y <= SCREEN_HEIGHT - PtsDeControle[3].y+2))
			{
				cout << "V2 PRESSED" << endl;
			}
		}

		//cout << "SÓ SAIU DO IF AGORA ==========================================" << endl;
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
	loadHermiteMatrix();
}

void Display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(640,500);
	glutInitWindowPosition(100,150);
	glutCreateWindow("Hermite Curve");
	glutMouseFunc(mouseClickEvent);
	glutDisplayFunc(Display);
	Init();
	glutMainLoop();

	return 0;
}
