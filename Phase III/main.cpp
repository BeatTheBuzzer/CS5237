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
#include <stack>
using namespace std;

const int largeCoor=500;

DAG dag;
PointSetArray myPointSet;
Trist myTrist, noIPTrist;
std::vector<int> vec;
std::stack<pair<int, int>> eg_stack;
double scale[]={0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
int nowS=9;
int DX=500, DY=300;
int delay;

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

	for (i=4;i<=myPointSet.noPt();i++){
		myPointSet.getPoint(i,x1,y1);
		drawAPoint(atof(x1.printOut().c_str()),atof(y1.printOut().c_str()));
	}

	glPopMatrix();
	glutSwapBuffers ();
	if (delay > 0) Sleep(delay);
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

bool IPhelper(Trist &tri1, int x, int y, bool readfile){
	int i,a,b,c,d, triangleIdx1, triangleIdx2, triangleIdx3;
	MyPoint pd;
	quad_indices qi;
	//cout<<x<<endl;cout<<y<<endl;
	x=x/scale[nowS];
	y=y/scale[nowS];
	pd.x=LongInt(x);
	pd.y=LongInt(y);
	d=myPointSet.addPoint(LongInt(x),LongInt(y));

	qi = dag.insert(d);
	if (qi.d != -1 || qi.a == -1) return false;
	myTrist.makeTri(qi.a, qi.b, d, true);
	myTrist.makeTri(qi.a, qi.c, d, true);
	myTrist.makeTri(qi.b, qi.c, d, true);
	return true;
}

void readFile(){

	string line_noStr;

	string line;   // each line of the file
	string command;// the command of each line
	string numberStr; // for single LongInt operation
	string outputAns = "Answer of your computation"; // the answer you computed

	ifstream inputFile("input.txt",ios::in);
	delay = 0;

	myPointSet.eraseAllPoints();
	myTrist.eraseAllTris();
	noIPTrist.eraseAllTris();
	myPointSet.addPoint(-largeCoor,largeCoor/2);
	myPointSet.addPoint(largeCoor,largeCoor/2);
	myPointSet.addPoint(0,-largeCoor/2);
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
			int x, y;
			linestream >> numberStr;
			x=atoi(numberStr.c_str());
			linestream >> numberStr;
			y=atoi(numberStr.c_str());
			if (IPhelper(myTrist, x, y, true))
				cout<<"IP successfully!"<<endl;
			else cout<<"The inserted point is not in any triangles"<<endl;
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
	char linenum[10];
	char outfile[]="savefile.txt";
	LongInt x1,y1;
	ofstream fout(outfile,ios::out);
	for (int i=1;i<=myPointSet.noPt();i++){
		myPointSet.getPoint(i,x1,y1);
		sprintf(linenum, "%04d", i - 1);
		fout << linenum << ": AP " << x1.printOut() << " " << y1.printOut() << endl;
	}
	sprintf(linenum, "%04d", myPointSet.noPt());
	fout << linenum << ": CD" << endl;
	fout.close();
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
	case 'c':
	case 'C':
		delaunay_triangulation();
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
		x-=DX;
		y-=DY;
		if (IPhelper(myTrist, x, y, false))
			cout<<"IP successfully!"<<endl;
		else cout<<"The inserted point is not in any triangles"<<endl;
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
	cout << "W: Write control points to \"savefile.txt\"" <<endl;
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

bool IPHelper(quad_indices qi, int pidx)
{
	int idx;

	while (!eg_stack.empty()) eg_stack.pop();

	if (qi.d == -1) {
		if ((idx = myTrist.getTriangleIndex(qi.a, qi.b, qi.c)) < 0) return false;
		myTrist.delTri(idx << 3);
		display();
		myTrist.makeTri(qi.a, qi.b, pidx, true);
		display();
		myTrist.makeTri(qi.a, qi.c, pidx, true);
		display();
		myTrist.makeTri(qi.b, qi.c, pidx, true);
		display();
		eg_stack.push(make_pair(qi.a, qi.b));
		eg_stack.push(make_pair(qi.a, qi.c));
		eg_stack.push(make_pair(qi.b, qi.c));
	}
	else {
		if ((idx = myTrist.getTriangleIndex(qi.a, qi.b, qi.c)) < 0) return false;
		myTrist.delTri(idx << 3);
		display();
		myTrist.makeTri(qi.a, qi.c, pidx, true);
		display();
		myTrist.makeTri(qi.b, qi.c, pidx, true);
		display();

		if ((idx = myTrist.getTriangleIndex(qi.a, qi.b, qi.d)) < 0) return false;
		myTrist.delTri(idx << 3);
		display();
		myTrist.makeTri(qi.a, qi.d, pidx, true);
		display();
		myTrist.makeTri(qi.b, qi.d, pidx, true);
		display();

		eg_stack.push(make_pair(qi.a, qi.c));
		eg_stack.push(make_pair(qi.a, qi.d));
		eg_stack.push(make_pair(qi.b, qi.c));
		eg_stack.push(make_pair(qi.b, qi.d));
	}

	return true;
}

void legalize(pair<int, int> edge)
{
	OrTri ot1, ot2;
	int a, b, u, v, incir;
	pair<int, int> indices;
	
	indices = myTrist.fromEdge2Indices(edge.first, edge.second);
	if (indices.second == -1) return;
	ot1 = myTrist.FIndex2OrTri(indices.first, edge.first, edge.second);
	ot2 = myTrist.FIndex2OrTri(indices.second, edge.first, edge.second);
	myTrist.getVertexIdx(ot1, a, b, u);
	myTrist.getVertexIdx(ot2, a, b, v);
	if (u == v) return;
	
	incir = myPointSet.inCircle(edge.first, edge.second, u, v);
	if (incir == 0) {
		cerr << "Degeneracy!" << endl;
		exit(-1);
	}
	else if (incir < 0) return;

	myTrist.delTri(ot1);
	display();
	myTrist.delTri(ot2);
	display();
	myTrist.makeTri(edge.first, u, v, true);
	display();
	myTrist.makeTri(edge.second, u, v, true);
	display();
	eg_stack.push(make_pair(edge.first, u));
	eg_stack.push(make_pair(edge.first, v));
	eg_stack.push(make_pair(edge.second, u));
	eg_stack.push(make_pair(edge.second, v));
	dag.flip(edge.first, edge.second, u, v);
}

void edge_legalization()
{
	pair<int, int> edge;

	while (!eg_stack.empty()) {
		edge = eg_stack.top();
		eg_stack.pop();
		legalize(edge);
	}
}

void delaunay_triangulation()
{
	int i, a, b, c;
	quad_indices qi;

	SYSTEMTIME st;
	GetLocalTime(&st);
	int start = (((st.wHour*60+st.wMinute)*60)+st.wSecond)*1000+st.wMilliseconds;
	cerr << "Start: " << start << endl;

	dag.init(&myPointSet, &myTrist, 1, 2, 3);
	myTrist.eraseAllTris();
	myTrist.makeTri(1, 2, 3);
	display();

	for (i = 4; i <= myPointSet.noPt(); i++) {
		qi = dag.insert(i);
		if (qi.a == -1 && qi.b == -1 && qi.c == -1 && qi.d == -1) continue;
		if (!IPHelper(qi, i)) return;
		edge_legalization();
	}

	for (i = 1; i <= myTrist.noTri(); i++) {
		myTrist.getVertexIdx(i << 3, a, b, c);
		if (a == 1 || a == 2 || a == 3) myTrist.delTri(i << 3);
		if (b == 1 || b == 2 || b == 3) myTrist.delTri(i << 3);
		if (c == 1 || c == 2 || c == 3) myTrist.delTri(i << 3);
	}

	GetLocalTime(&st);
	int end = (((st.wHour*60+st.wMinute)*60)+st.wSecond)*1000+st.wMilliseconds;
	cerr << "End: " << end << endl;
	cerr << "Elapsed Time(ms): " << end-start << endl;
}
