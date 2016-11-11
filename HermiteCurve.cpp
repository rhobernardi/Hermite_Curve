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
bool flag = false;

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
		vx2 = (float)p2.x - (dx);
	} else {
		vx1 = (float)p1.x - (dx);
		vx2 = (float)p2.x + (dx);
	}

	if (p1.y < p2.y) {
		vy1 = (float)p1.y + (dy);
		vy2 = (float)p2.y - (dy);
	} else {
		vy1 = (float)p1.y - (dy);
		vy2 = (float)p2.y + (dy);
	}

	PtsDeControle[2].setxy((float) vx1, (float) vy1);
	PtsDeControle[3].setxy((float) vx2, (float) vy2);

	glutPostRedisplay();
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

	Point *ParRes = partialResult(PT);

	for (int k = 0; k < 4; ++k)
	{
		P[k].x += 1*ParRes[0].x + u*ParRes[1].x + pow(u,2)*ParRes[2].x + pow(u,3)*ParRes[3].x; 
		P[k].y += 1*ParRes[0].y + u*ParRes[1].y + pow(u,2)*ParRes[2].y + pow(u,3)*ParRes[3].y;
	}

	for (int i = 0; i < 4; ++i)
	{
		PT[i] = P[i];
	}
}


void keyPressedEvent(unsigned char key, int x, int y)
{
// V0 => velocidade de P0
	if (key == 'w') // aumenta a velocidade V0
	{
		float dx = (float)abs(PtsDeControle[0].x - PtsDeControle[2].x);
		float dy = (float)abs(PtsDeControle[0].y - PtsDeControle[2].y);
		
		PtsDeControle[2].x += dx*0.05;
		PtsDeControle[2].y += dy*0.05;
		
		glutPostRedisplay();
	}

	if (key == 'a') // rotaciona em sentido anti-horario a velocidade V0
	{
		//PtsDeControle[2].x += 3;
		//PtsDeControle[2].y += 3;

		//glTranslated(PtsDeControle[0].x, PtsDeControle[0].y, 0);
		//glRotatef(0.0f, 0.0f, 30.0f, 1.0f);
		//glTranslated(-PtsDeControle[0].x, -PtsDeControle[0].y, 0);

		glutPostRedisplay();
	}

	if (key == 's') // diminui a velocidade V0
	{
		float dx = (float)abs(PtsDeControle[0].x - PtsDeControle[2].x);
		float dy = (float)abs(PtsDeControle[0].y - PtsDeControle[2].y);
		
		PtsDeControle[2].x -= dx*0.05;
		PtsDeControle[2].y -= dy*0.05;

		glutPostRedisplay();
	}

	if (key == 'd') // rotaciona em sentido horario a velocidade V0
	{
		
	}


// V0 => velocidade de P1
	if (key == 'i') // aumenta a velocidade V1
	{
		float dx = (float)abs(PtsDeControle[0].x - PtsDeControle[2].x);
		float dy = (float)abs(PtsDeControle[0].y - PtsDeControle[2].y);
		
		PtsDeControle[3].x += dx*0.05;
		PtsDeControle[3].y += dy*0.05;
		
		glutPostRedisplay();
	}

	if (key == 'j') // rotaciona em sentido anti-horario a velocidade V1
	{

	}

	if (key == 'k') // diminui a velocidade V1
	{
		float dx = (float)abs(PtsDeControle[0].x - PtsDeControle[2].x);
		float dy = (float)abs(PtsDeControle[0].y - PtsDeControle[2].y);
		
		PtsDeControle[3].x -= dx*0.05;
		PtsDeControle[3].y -= dy*0.05;
		
		glutPostRedisplay();
	}

	if (key == 'l') // rotaciona em sentido horario a velocidade V1
	{

	}

	cout << key << " PRESSED" << endl;
}

//void mouseMoveEvent(int x, int y);

void mouseClickEvent(int button, int state, int x, int y)
{	
	if(button == GLUT_LEFT_BUTTON)// && state == GLUT_DOWN) 
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
				calculateSpeedVectors(PtsDeControle[0], PtsDeControle[1]);
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
	loadHermiteMatrix();
}

void Display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	drawDot(PtsDeControle[0].x, PtsDeControle[0].y);
	drawDot(PtsDeControle[1].x, PtsDeControle[1].y);
	drawDot(PtsDeControle[2].x, PtsDeControle[2].y);
	drawDot(PtsDeControle[3].x, PtsDeControle[3].y);

	drawLine(PtsDeControle[0], PtsDeControle[1]);

	drawLineVector(PtsDeControle[0], PtsDeControle[2]);
	drawLineVector(PtsDeControle[1], PtsDeControle[3]);

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
	glutMouseFunc(mouseClickEvent);
	glutKeyboardFunc(keyPressedEvent);

	glutMainLoop();

	return 0;
}
