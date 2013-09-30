#include <string.h>
#include <iostream>
#include "pointSet.h"

using namespace std;

bool equal(const char *cmd, const char *str)
{
	return !strcmp(cmd, str);
}

int main()
{
	PointSet ps;
	int ia, ib, ic, id, pn = 0;
	char cmd[10], linenum[20];
	LongInt x, t, s, a[3][3];

	while (cin >> linenum) {
		cin >> cmd;
		if (equal(cmd, "LX")) {
			cin >> x;
			cout << linenum << ' ' << x << endl;
		}
		else if (equal(cmd, "AD")) {
			cin >> t;
			x = x + t;
			cout << linenum << ' ' << x << endl;
		}
		else if (equal(cmd, "SB")) {
			cin >> t;
			x = x - t;
			cout << linenum << ' ' << x << endl;
		}
		else if (equal(cmd, "MU")) {
			cin >> t;
			x = x * t;
			cout << linenum << ' ' << x << endl;
		}
		else if (equal(cmd, "NG")) {
			x = -x;
			cout << linenum << ' ' << x << endl;
		}
		else if (equal(cmd, "SD")) {
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++) 
					cin >> a[i][j];
			cout << linenum << ' ' << signDet(a) << endl;
		}
		else if (equal(cmd, "AP")) {
			cin >> t >> s;
			ps.addPoint(t, s);
			cout << linenum << " #POINT = " << ++pn << endl;
		}
		else if (equal(cmd, "IT")) {
			cin >> ia >> ib >> ic >> id;
			cout << linenum << ' ' << ps.inTri(ib-1, ic-1, id-1, ia-1) << endl;
		}
		else if (equal(cmd, "IC")) {
			cin >> ia >> ib >> ic >> id;
			cout << linenum << ' ' << ps.inCircle(ib-1, ic-1, id-1, ia-1) << endl;
		}
	}
	return 0;
}
