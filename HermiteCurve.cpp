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
int HermiteMatrix[4][4];
int angl = 0;
int flag = 0;
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

void rotatePoint(Point eixo, Point *pt, int angl)
{
	float rmatrix[3][3];					// matriz de rotação 2D 3x3
	float result[3], point[3];

	result[0] = 0;
	result[1] = 0;
	result[2] = 0;

	point[0] = pt->x;
	point[1] = pt->y;
	point[2] = 1;

	rmatrix[0][0] = cos(angl);				// [ cos(a)  -sin(a)   0]
	rmatrix[0][1] = sin(angl);				// [ sin(a)   cos(a)   0]
	rmatrix[0][2] = 0;						// [ 0        0        1]
	rmatrix[1][0] = -sin(angl);
	rmatrix[1][1] = cos(angl);
	rmatrix[1][2] = 0;
	rmatrix[2][0] = 0;
	rmatrix[2][1] = 0;
	rmatrix[2][2] = 1;

	//glTranslatef(-eixo.x, -eixo.y, 0);
	
	point[0] += -eixo.x;
	point[1] += -eixo.y;
	
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			result[i] += rmatrix[i][j]*point[j];
		}
	}

	point[0] += eixo.x;
	point[1] += eixo.y;

	//glTranslatef(eixo.x, eixo.y, 0);, 

	pt->x = result[0];
	pt->y = result[1];

	// fazer uma função que translada o vetor até a origem e rotaciona. a função deve usar matrizes de translação e rotação
}

Point *constResult(Point *PT)
{
	Point *result = (Point *) calloc(4,sizeof(Point));
	float x = 0, y = 0;

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			x += HermiteMatrix[i][j]*PT[j].x;
			y += HermiteMatrix[i][j]*PT[j].y;

			result[j].setxy(x,y);
		}
	}

	return result;
}


void drawHermiteCurve(Point *PT, double u)
{
	Point *P = (Point *) calloc(4, sizeof(Point));

	Point *ParRes = constResult(PT);

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
/*// V0 => velocidade de P0
	if (key == 'w') // aumenta a velocidade V0
	{
		float dx = (float)abs(PtsDeControle[0].x - PtsDeControle[2].x);
		float dy = (float)abs(PtsDeControle[0].y - PtsDeControle[2].y);
		
		PtsDeControle[2].setxy(PtsDeControle[2].x + dx*0.05, PtsDeControle[2].y + dy*0.05);
	}

	if (key == 'a' && contPoints == 2) // rotaciona em sentido anti-horario a velocidade V0
	{
		//angl += 5;
		//cout << "control[0].x: " << PtsDeControle[0].x << "control[1].x: " << PtsDeControle[1].x << "control[2].x: " << PtsDeControle[2].x << "control[3].x: " << PtsDeControle[3].x << endl;
		//cout << "control[0].y: " << PtsDeControle[0].y << "control[1].y: " << PtsDeControle[1].y << "control[2].y: " << PtsDeControle[2].y << "control[3].y: " << PtsDeControle[3].y << endl;
		
		float dx = (float)abs(PtsDeControle[0].x - PtsDeControle[2].x);
		float dy = (float)abs(PtsDeControle[0].y - PtsDeControle[2].y);
		
		angl += 45;

		//PtsDeControle[2].x = dx*sin(angl);
		//PtsDeControle[2].y = dy*cos(angl);
		//
		rotatePoint(PtsDeControle[0], &PtsDeControle[2], angl);

	}

	if (key == 's') // diminui a velocidade V0
	{
		float dx = (float)abs(PtsDeControle[0].x - PtsDeControle[2].x);
		float dy = (float)abs(PtsDeControle[0].y - PtsDeControle[2].y);
		
		PtsDeControle[2].setxy(PtsDeControle[2].x - dx*0.05, PtsDeControle[2].y - dy*0.05);
	}

	if (key == 'd' && contPoints == 2) // rotaciona em sentido horario a velocidade V0
	{
		angl -= 5;
	}


// V0 => velocidade de P1
	if (key == 'i') // aumenta a velocidade V1
	{
		float dx = (float)abs(PtsDeControle[0].x - PtsDeControle[3].x);
		float dy = (float)abs(PtsDeControle[0].y - PtsDeControle[3].y);
		
		PtsDeControle[2].setxy(PtsDeControle[3].x - dx*0.05, PtsDeControle[3].y - dy*0.05);
	}

	if (key == 'j' && contPoints == 2) // rotaciona em sentido anti-horario a velocidade V1
	{

	}

	if (key == 'k') // diminui a velocidade V1
	{
		float dx = (float)abs(PtsDeControle[0].x - PtsDeControle[3].x);
		float dy = (float)abs(PtsDeControle[0].y - PtsDeControle[3].y);
		
		PtsDeControle[2].setxy(PtsDeControle[3].x - dx*0.05, PtsDeControle[3].y - dy*0.05);
	}

	if (key == 'l' && contPoints == 2) // rotaciona em sentido horario a velocidade V1
	{

	}
*/
	if (key == 27) exit(0);

	cout << key << " PRESSED" << endl;
	glutPostRedisplay();
}


void mousePassEvent(int x, int y)
{
	//cout << "x:" << x << "\ty: " << SCREEN_HEIGHT - y << endl;
	//cout << "CP[0].x: " << PtsDeControle[0].x << "\tCP[0].y: " << PtsDeControle[0].y << endl;
	if (flag == 1)
	{
		PtsDeControle[1].setxy(x, SCREEN_HEIGHT - y);
	}

	if (flag == 2)
	{
		PtsDeControle[2].setxy(x, SCREEN_HEIGHT - y);
	}

	if (flag == 3)
	{
		PtsDeControle[3].setxy(x, SCREEN_HEIGHT - y);
	}

	glutPostRedisplay();
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
			if(contPoints == 1)flag = 1;
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

	if(flag != 1)
	{
		drawLineVector(PtsDeControle[0], PtsDeControle[2]);
		drawLineVector(PtsDeControle[1], PtsDeControle[3]);
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
