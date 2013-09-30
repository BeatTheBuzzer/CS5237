#include "basicsP2\pointSetArray.h"
#include "basicsP2\Trist.h"

#include "dag.h"
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
#include <vector>
using namespace std;

PointSetArray myPointSet;
Trist myTrist, noIPTrist;
std::vector<int> vec;
double scale[]={0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
int nowS=9;
int DX=500, DY=300;

bool IPhelper(Trist &tri1, int x, int y);

void delaunay_triangulation();

// These three functions are for those who are not familiar with OpenGL, you can change these or even completely ignore them

void drawAPoint(double x,double y)
{
	x*=scale[nowS];
	y*=scale[nowS];
	x+=DX;
	y+=DY;
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
}

void drawATriangle(int ix1, int ix2, int ix3)
{
	LongInt xl,yl;
	int x1,x2,x3,y1,y2,y3;
	myPointSet.getPoint(ix1,xl,yl);
	x1=atof(xl.printOut().c_str())*scale[nowS]+DX;
	y1=atof(yl.printOut().c_str())*scale[nowS]+DY;

	myPointSet.getPoint(ix2,xl,yl);
	x2=atof(xl.printOut().c_str())*scale[nowS]+DX;
	y2=atof(yl.printOut().c_str())*scale[nowS]+DY;

	myPointSet.getPoint(ix3,xl,yl);
	x3=atof(xl.printOut().c_str())*scale[nowS]+DX;
	y3=atof(yl.printOut().c_str())*scale[nowS]+DY;

	glBegin(GL_POLYGON);
	glColor3f(0,0.5,0);
	glVertex2d(x1,y1);
	glVertex2d(x2,y2);
	glVertex2d(x3,y3);
	glEnd();

	drawALine(x1,y1,x2,y2);
	drawALine(x1,y1,x3,y3);
	drawALine(x2,y2,x3,y3);

}

void display(void)
{
	int i, ix1, ix2, ix3;
	LongInt x1,y1;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();

	for (i=1;i<=myTrist.noTri();i++){
		myTrist.getVertexIdx(i<<3,ix1,ix2,ix3);
		if (ix1 == -1) continue;
		drawATriangle(ix1,ix2,ix3);
	}

	for (i=1;i<=myPointSet.noPt();i++){
		myPointSet.getPoint(i,x1,y1);
		drawAPoint(atof(x1.printOut().c_str()),atof(y1.printOut().c_str()));
	}

	glPopMatrix();
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

int getVertex(Trist &tri, int idx, int idx1, int idx2, int &triangleIdx){
	int i,a,b,c;
	for (i=1;i<=tri.noTri();i++){
		tri.getVertexIdx(i<<3,a,b,c);
		if(a==idx1){
			if(b==idx2){
				triangleIdx = i;
				if (c !=idx)
				return c;
			}
			else if(c==idx2) {
				triangleIdx = i;
				if (b !=idx)
				return b;
			}
		}else if(a==idx2){
			if(b==idx1) {
				triangleIdx = i;
				if (c !=idx)
				return c;
			}
			else if(c==idx1) {
				triangleIdx = i;
				if (c !=idx)
				return c;
			}
		}else{
			if((b==idx1&&c==idx2)||(b==idx2&&c==idx1)){
				triangleIdx = i;
				if (a !=idx)
				return a;
			}
		}
	}
	return -1;
}

void readFile(){

	string line_noStr;

	string line;   // each line of the file
	string command;// the command of each line
	string numberStr; // for single LongInt operation
	string outputAns = "Answer of your computation"; // the answer you computed

	ifstream inputFile("input.txt",ios::in);

	int delay=0;

	myPointSet.eraseAllPoints();
	myTrist.eraseAllTris();
	noIPTrist.eraseAllTris();
	if(inputFile.fail()){
		cerr << "Error: Cannot read input file \"" << "input.txt" << "\"";
		return;
	}

	while(inputFile.good()){

		getline(inputFile,line);
		if(line.empty()) {
			command = "";
			continue; 
		}// in case the line has nothing in it

		stringstream linestream(line);

		linestream >> line_noStr;
		linestream >> command;         // get the command
		if (delay>0)
			Sleep(delay);

		if(!command.compare("AP")){
			linestream >> numberStr;
			LongInt p1 = LongInt::LongInt(numberStr.c_str());

			linestream >> numberStr;
			LongInt p2 = LongInt::LongInt(numberStr.c_str());

			int output = myPointSet.addPoint(p1, p2);
		}
		else if(!command.compare("OT")){
			int idx[3];
			linestream >> numberStr;
			idx[0]=atoi(numberStr.c_str());
			linestream >> numberStr;
			idx[1]=atoi(numberStr.c_str());
			linestream >> numberStr;
			idx[2]=atoi(numberStr.c_str());
			myTrist.makeTri(idx[0],idx[1],idx[2]);
			noIPTrist.makeTri(idx[0],idx[1],idx[2]);
		}
		else if(!command.compare("IP")){
			linestream >> numberStr;
			linestream >> numberStr;
		}
		else if(!command.compare("DY")){
			linestream >> numberStr;
			delay=atoi(numberStr.c_str())*1000;
			cout<<"Delay "<<numberStr<<" second(s) for each instruction"<<endl;
		}
		else if (!command.compare("CD")) {
			delaunay_triangulation();
		}
		else{
			cerr << "Exception: Wrong input command" << endl;
		}
		display();				
	}
	vec.clear();
}

void writeFile()
{
	char outfile[]="savefile.txt";
	int i,a,b,c;
	LongInt x1,y1;
	ofstream fout(outfile,ios::out);
	for (i=1;i<=myPointSet.noPt();i++){
		myPointSet.getPoint(i,x1,y1);
		fout<<"Point "<<i<<": "<<x1.printOut()<<"\t"<<y1.printOut()<<endl;
	}
	for (i=1;i<=noIPTrist.noTri();i++){
		noIPTrist.getVertexIdx(i<<3,a,b,c);
		fout<<"Triangle "<<i<<": "<<a<<"\t"<<b<<"\t"<<c<<endl;
	}
	cout<<"Wirte to file successfully!"<<endl;
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

	case 'Q':
	case 'q':
		exit(0);
		break;

	case '+':
		if (nowS<19)
			nowS++;
		break;

	case '-':
		if (nowS>0)
			nowS--;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
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
	}
	glutPostRedisplay();
}

void special(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_UP:
		DY-=5;
		break;
	case GLUT_KEY_DOWN:
		DY+=5;
		break;
	case GLUT_KEY_LEFT:
		DX-=5;
		break;
	case GLUT_KEY_RIGHT:
		DX+=5;
		break;
	}
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	cout<<"CS5237 Phase II"<< endl<< endl;
	cout << "Right mouse click: OT operation"<<endl;
	cout << "Q: Quit" <<endl;
	cout << "R: Read in control points from \"input.txt\"" <<endl;
	cout << "W: Write control points to \"input.txt\"" <<endl;
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (1000, 700);
	glutInitWindowPosition (50, 50);
	glutCreateWindow ("CS5237 Phase II");
	init ();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutMainLoop();

	return 0;
}

bool IPHelper(uo_tripple ut, int pidx)
{
	int idx;
	
	if ((idx = myTrist.getTriangleIndex(ut.a, ut.b, ut.c)) < 0) return false;
	myTrist.delTri(idx << 3);
	myTrist.makeTri(ut.a, ut.b, pidx, true);
	myTrist.makeTri(ut.a, ut.c, pidx, true);
	myTrist.makeTri(ut.b, ut.c, pidx, true);

	return true;
}


void FLIPHelper_(DAG &dag, int ea, int eb, int p)
{
	FIndex f1;
	OrTri ot1, ot2;
	int a, b, c, p2;

	f1 = myTrist.getTriangleIndex(ea, eb, p);
	ot1 = myTrist.FIndex2OrTri(f1, ea, eb);
	ot2 = myTrist.fnext(ot1);
	if (ot2 < 0) return;
	myTrist.getVertexIdx(ot2, a, b, c);
	if (myPointSet.inCircle(a, b, c, p) < 0) return;

	if (a != ea && a != eb) p2 = a;
	else if (b != ea && b != eb) p2 = b;
	else p2 = c;
	myTrist.delTri(ot1);
	myTrist.delTri(ot2);
	myTrist.makeTri(ea, p, p2, true);
	myTrist.makeTri(eb, p, p2, true);
	dag.flip(ea, eb, p, p2);
}

void FLIPHelper(DAG &dag, uo_tripple ut, int p)
{
	FLIPHelper_(dag, ut.a, ut.b, p);
	display();
	FLIPHelper_(dag, ut.a, ut.c, p);
	display();
	FLIPHelper_(dag, ut.b, ut.c, p);
	display();
}

void delaunay_triangulation()
{
	DAG dag;
	int i;
	uo_tripple ut;

	dag.init(&myPointSet, 1, 2, 3);
	myTrist.eraseAllTris();
	myTrist.makeTri(1, 2, 3);

	for (i = 4; i <= myPointSet.noPt(); i++) {
		ut = dag.insert(i);
		if (ut.a == 0 && ut.b == 0 && ut.c == 0) continue;
		if (!IPHelper(ut, i)) return;
		display();
		FLIPHelper(dag, ut, i);
	}
}
