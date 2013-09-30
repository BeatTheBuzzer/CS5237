
#include "basicsP2\pointSetArray.h"
#include "basicsP2\trist.h"

#include "math.h"
#include <iostream>
#include <fstream>
#include "GL\glut.h"
#include <windows.h>
#include <cstdio>
#include <fstream>
#include <strstream>
#include <string>
#include <sstream>

using namespace std;

Trist trist;
vector<pair<double, double>> ps;

int delay;
int lx, ly;
bool mouseldown = false;
double width, height;
double centerX = 0.0, centerY = 0.0;

int signDet(double x1, double y1, double w1, double x2, double y2, double w2, double x3, double y3, double w3)
{
	const double eps = 1e-6;
	double res = x1*(y2*w3-w2*y3) - y1*(x2*w3-w2*x3) + w1*(x2*y3-y2*x3);
	return res < -eps? -1: res > eps;
}

int inTri(int p1Idx, int p2Idx, int p3Idx, int pIdx) 
{
	int s1, s2, s3;
	double v1x, v1y, v2x, v2y;

	v1x = ps[p2Idx].first - ps[p1Idx].first;
	v1y = ps[p2Idx].second - ps[p1Idx].second;
	v2x = ps[pIdx].first - ps[p1Idx].first;
	v2y = ps[pIdx].second - ps[p1Idx].second;
	s1 = signDet(1, 0, 0, 0, v1x, v1y, 0, v2x, v2y);

	v1x = ps[p3Idx].first - ps[p2Idx].first;
	v1y = ps[p3Idx].second - ps[p2Idx].second;
	v2x = ps[pIdx].first - ps[p2Idx].first;
	v2y = ps[pIdx].second - ps[p2Idx].second;
	s2 = signDet(1, 0, 0, 0, v1x, v1y, 0, v2x, v2y);

	v1x = ps[p1Idx].first - ps[p3Idx].first;
	v1y = ps[p1Idx].second - ps[p3Idx].second;
	v2x = ps[pIdx].first - ps[p3Idx].first;
	v2y = ps[pIdx].second - ps[p3Idx].second;
	s3 = signDet(1, 0, 0, 0, v1x, v1y, 0, v2x, v2y);

	if (!s1 || !s2 || !s3) return 0;
	if (s1 == s2 && s2 == s3) return 1;
	return -1;
}

int IP(double x, double y)
{
	int i, noTri, idx, idx1, idx2, idx3;

	ps.push_back(make_pair(x, y));
	idx = (int)ps.size() - 1;
	noTri = trist.noTri();
	for (i = 0; i < noTri; i++) {
		trist.getVertexIdx(i << 3, idx1, idx2, idx3);
		if (inTri(idx1, idx2, idx3, idx) > 0) break;
	}
	if (i == noTri) return 0;
	trist.delTri(i << 3);
	trist.makeTri(idx1, idx2, idx);
	trist.makeTri(idx2, idx3, idx);
	trist.makeTri(idx3, idx1, idx);

	return 1;
}

void zoom(bool zoom_in, double mx, double my)
{
	const double ZOOM_FACTOR = 1.1;
	const double RESTRICTION_MIN = 50.0;
	const double RESTRICTION_MAX = 5000.0;
	double x, y, preX, preY, postX, postY;

	if (zoom_in && (width <= RESTRICTION_MIN || height <= RESTRICTION_MIN)) return;
	else if (!zoom_in && (width >= RESTRICTION_MAX || height >= RESTRICTION_MAX)) return;

	my = glutGet(GLUT_WINDOW_HEIGHT) - my;
	x = (mx / double(glutGet(GLUT_WINDOW_WIDTH))) - 0.5;
	y = (my / double(glutGet(GLUT_WINDOW_HEIGHT))) - 0.5;
	preX = x * width;
	preY = y * height;
	
	if (zoom_in) {
		width /= ZOOM_FACTOR;
		height /= ZOOM_FACTOR;
	}
	else {
		width *= ZOOM_FACTOR;
		height *= ZOOM_FACTOR;
	}
	postX = x * width;
	postY = y * height;
	centerX += (preX - postX);
	centerY += (preY - postY);
}

// These three functions are for those who are not familiar with OpenGL, you can change these or even completely ignore them

void drawAPoint(double x,double y)
{
	glPointSize(5);
	glBegin(GL_POINTS);
	glColor3f(0,0,0);
	glVertex2d(x,y);
	glEnd();
	glPointSize(1);
}

void drawALine(double x1,double y1, double x2, double y2)
{
	glPointSize(1);
	glBegin(GL_LINE_LOOP);
	glColor3f(0,0,1);
	glVertex2d(x1,y1);
	glVertex2d(x2,y2);
	glEnd();
	glPointSize(1);
}

void drawATriangle(double x1,double y1, double x2, double y2, double x3, double y3)
{
	glBegin(GL_POLYGON);
	glColor3f(0,0.5,0);
	glVertex2d(x1,y1);
	glVertex2d(x2,y2);
	glVertex2d(x3,y3);
	glEnd();
}



void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(
		centerX - (width / 2.0), 
		centerX + (width / 2.0), 
		centerY - (height / 2.0), 
		centerY + (height / 2.0), 
		-1, 
		1
	);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// draw your output here (erase the following 3 lines)
	int i, idx1, idx2, idx3, noTri = trist.noTri();
	double x1, y1, x2, y2, x3, y3;

	for (i = 0; i < noTri; i++) {
		trist.getVertexIdx(i << 3, idx1, idx2, idx3);
		x1 = ps[idx1].first; y1 = ps[idx1].second;
		x2 = ps[idx2].first; y2 = ps[idx2].second;
		x3 = ps[idx3].first; y3 = ps[idx3].second;
		drawATriangle(x1, y1, x2, y2, x3, y3);
		drawALine(x1, y1, x2, y2);
		drawALine(x2, y2, x3, y3);
		drawALine(x3, y3, x1, y1);
	}

	glutSwapBuffers ();
}

void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,w,h,0);  
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void init(void)
{
	glClearColor (1.0,1.0,1.0, 1.0);
}



void readFile(){
	string line_noStr;

	string line;   // each line of the file
	string command;// the command of each line
	string numberStr; // for single LongInt operation
	string outputAns = "Answer of your computation"; // the answer you computed
	double x, y;
	int idx1, idx2, idx3;

	ifstream inputFile("input.txt",ios::in);

	if(inputFile.fail()){
		cerr << "Error: Cannot read input file \"" << "input.txt" << "\"";
		return;
	}

	while(inputFile.good()) {
		getline(inputFile, line);
		if(line.empty()) {
			command = "";
			continue; 
		}// in case the line has nothing in it

		stringstream linestream(line);

		linestream >> line_noStr;
		linestream >> command;         // get the command

		if(!command.compare("AP")){
			linestream >> x >> y;
			ps.push_back(make_pair(x, y));
		}
		else if(!command.compare("OT")){
			linestream >> idx1 >> idx2 >> idx3;
			trist.makeTri(idx1-1, idx2-1, idx3-1);
		}
		else if(!command.compare("IP")){
			linestream >> x >> y;
			IP(x, y);
		}
		else if(!command.compare("DY")){
			linestream >> delay;
		}
		else{
			cerr << "Exception: Wrong input command" << endl;
		}
	}

}

void writeFile()
{
	char line[16];
	int i, j, idx1, idx2, idx3;
	ofstream outputFile("savefile.txt", ios::out);

	for (i = 0; i < (int)ps.size(); i++) {
		sprintf_s(line, "%04d: ", i);
		outputFile << line << "AP " << ps[i].first << " " << ps[i].second << endl;
	}
	for (j = 0; j < trist.noTri(); j++) {
		sprintf_s(line, "%04d: ", i + j);
		trist.getVertexIdx(j << 3, idx1, idx2, idx3);
		outputFile << line << "OT " << idx1+1 << " " << idx2+1 << " " << idx3+1 << endl;
	}
	outputFile.close();
	cout << "Data >> savefile.txt" << endl;
}

void keyboard (unsigned char key, int x, int y)
{
	switch (key) {
	case 'r':
	case 'R':
		readFile();
		break;

	case 'w':
	case 'W':
		writeFile();
		break;
	
	case '+':
	case '=':
		zoom(true, x, y);
		break;

	case '-':
	case '_':
		zoom(false, x, y);
		break;

	case 'Q':
	case 'q':
		exit(0);
		break;

	default:
		break;
	}

	glutPostRedisplay();
}



void mouse(int button, int state, int mx, int my)
{
	double x, y, X, Y;

	my = glutGet(GLUT_WINDOW_HEIGHT) - my;
	x = (mx / double(glutGet(GLUT_WINDOW_WIDTH))) - 0.5;
	y = (my / double(glutGet(GLUT_WINDOW_HEIGHT))) - 0.5;
	X = centerX + x * width;
	Y = centerY + y * height;

	/*button: GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, or GLUT_RIGHT_BUTTON */
	/*state: GLUT_UP or GLUT_DOWN */
	enum
	{
		MOUSE_LEFT_BUTTON = 0,
		MOUSE_MIDDLE_BUTTON = 1,
		MOUSE_RIGHT_BUTTON = 2,
		MOUSE_SCROLL_UP = 3,
		MOUSE_SCROLL_DOWN = 4
	};
	if((button == MOUSE_RIGHT_BUTTON)&&(state == GLUT_UP))
	{
		if (!IP(X, Y)) {
			cerr << "(" << X << ", " << Y << ") is invalid" << endl;
		}
	}
	else if (button == MOUSE_LEFT_BUTTON) {
		if (state == GLUT_DOWN) mouseldown = true, lx = X, ly = Y;
		else if (state == GLUT_UP) mouseldown = false;
	}
	glutPostRedisplay();
}

void mouse_motion(int mx, int my)
{
	double x, y, X, Y;

	if (!mouseldown) return;
	
	my = glutGet(GLUT_WINDOW_HEIGHT) - my;
	x = (mx / double(glutGet(GLUT_WINDOW_WIDTH))) - 0.5;
	y = (my / double(glutGet(GLUT_WINDOW_HEIGHT))) - 0.5;
	X = centerX + x * width;
	Y = centerY + y * height;
	centerX += lx - X;
	centerY += ly - Y;

	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	cout<<"CS5237 Phase II"<< endl<< endl;
	cout << "Right mouse click: OT operation"<<endl;
	cout << "Q: Quit" <<endl;
	cout << "R: Read in control points from \"input.txt\"" <<endl;
	cout << "W: Write control points to \"savefile.txt\"" <<endl;
	cout << "+/-: Zoom in/out" << endl;
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (1000, 700);
	glutInitWindowPosition (50, 50);
	glutCreateWindow ("CS5237 Phase II");
	init ();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(mouse_motion);
	glutKeyboardFunc(keyboard);

	width = glutGet(GLUT_WINDOW_WIDTH);
	height = glutGet(GLUT_WINDOW_HEIGHT);
	glutMainLoop();

	return 0;
}
