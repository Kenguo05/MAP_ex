#define _CRT_SECURE_NO_WARNIGS
#include "logiclayer.h"
#include <math.h>
#include <stdio.h>

STOP* GIS::st = 0;
LINE* GIS::ls = 0;
int GIS::ns = 0;
int GIS::nl = 0;
int GIS::obs = 0;
TMAP GIS::raw;
TMAP GIS::tra;

//===========================================================================================================

STOP::STOP(int n, int x, int y) :numb(n), x(x), y(y) {}

int& STOP::X() {
	return x;
}

int& STOP::Y() {
	return y;
}

int& STOP::N() {
	return numb;
}

//===========================================================================================================

LINE::LINE(int n, int c, int* stop) :numb(n), stop(c ? new int[c] : nullptr), nofs(stop ? c : 0) {
	if (LINE::stop == nullptr && nofs != 0)
	{
		throw "LINE类内存分配错误！";
	}
	for (int i = 0; i < nofs; i++)
	{
		LINE::stop[i] = stop[i];
	}
}

LINE::LINE(const LINE& r) :numb(r.numb), stop(r.nofs ? new int[r.nofs] : nullptr), nofs(stop ? r.nofs : 0) {
	if (stop == nullptr && r.nofs != 0)
	{
		throw "LINE类内存分配错误！";
	}
	for (int i = 0; i < nofs; i++)
	{
		stop[i] = r.stop[i];
	}
}

LINE::LINE(LINE&& r)noexcept :numb(r.numb), stop(r.stop), nofs(r.nofs) {
	(int&)(r.numb) = (int&)(r.nofs) = 0;
	(int*&)(r.stop) = nullptr;
}

LINE& LINE::operator=(const LINE& r) {
	if (this == &r)
	{
		return *this;
	}
	if (stop)
	{
		delete[] stop;
	}
	(int&)numb = r.numb;
	(int*&)stop = new int[r.nofs];
	if (stop == nullptr)
	{
		throw "LINE类operator=操作错误！";
	}
	(int&)nofs = stop ? r.nofs : 0;
	for (int i = 0; i < nofs; i++)
	{
		stop[i] = r.stop[i];
	}
	return *this;
}

LINE& LINE::operator=(LINE&& r)noexcept {
	if (this == &r)
	{
		return *this;
	}
	if (stop)
	{
		delete[] stop;
	}
	(int&)numb = r.numb;
	(int*&)stop = r.stop;
	(int&)nofs = r.nofs;
	(int&)(r.numb) = (int&)(r.nofs) = 0;
	(int*&)r.stop = nullptr;
	return *this;
}

int LINE::has(int s)const {
	for (int i = 0; i < nofs; i++)
	{
		if (stop[i] == s)
		{
			return i;
		}
	}
	return -1;
}

int LINE::cross(const LINE& b)const {
	if (this == &b)
	{
		return 0;
	}
	for (int i = 0; i < nofs; i++)
	{
		if (b.has(stop[i]) != -1)
		{
			return 1;
		}
	}
	return 0;
}

LINE::operator int()const {
	return numb;
}

int LINE::NOFS()const {
	return nofs;
}

double LINE::dist(int b, int e)const {
	double d;
	int bi, ei;
	int x1, y1, x2, y2, w;
	if ((bi = has(b)) == -1)
	{
		throw "计算站间距离没有找到站！";
	}
	if ((ei = has(e)) == -1)
	{
		throw "计算站间距离没有找到站！";
	}
	if (bi > ei)
	{
		b = ei;
		ei = bi;
		bi = b;
	}
	for ( d = 0, b = bi; b < ei; b++)
	{
		x1 = GIS::st[w = stop[b]].X();
		y1 = GIS::st[w].Y();
		x2 = GIS::st[w = stop[b + 1]].X() - x1;
		y2 = GIS::st[w].Y() - y1;
		d += sqrt(double(x2 * x2 + y2 * y2));
	}
	return d;
}

int& LINE::operator[](int x) {
	return stop[x];
}

LINE::~LINE()noexcept {
	if (stop)
	{
		delete[] stop;
		(int*&)stop = nullptr;
		(int&)numb = (int&)nofs = 0;
	}
}

//===========================================================================================================

TRAN::TRAN(int from, int to, int stop) :from(from), to(to), stop(stop) {}

int TRAN::operator==(const TRAN& t)const {
	return from == t.from && to == t.to && stop == t.stop;
}

int& TRAN::F() {
	return from;
}

int& TRAN::T() {
	return to;
}

int& TRAN::S() {
	return stop;
}

//===========================================================================================================

ROUTE::ROUTE(TRAN* tran, int noft) :tran(noft ? new TRAN[noft] : nullptr), noft(tran ? noft : 0) {
	if (ROUTE::tran == nullptr && noft != 0)
	{
		throw "ROUTE类内存分配错误！";
	}
	for (int i = 0; i < noft; i++)
	{
		ROUTE::tran[i] = tran[i];
	}
}

ROUTE::ROUTE(const TRAN& t) :tran(new TRAN[1]), noft(tran ? 1 : 0) {
	if (tran == nullptr)
	{
		throw "ROUTE类内存分配错误！";
	}
	if (tran)
	{
		*tran = t;
	}
}

ROUTE::ROUTE(const ROUTE& r) :tran(r.noft ? new TRAN[r.noft] : nullptr), noft(tran ? r.noft : 0) {
	if (tran == nullptr && r.noft != 0)
	{
		throw "ROUTE类内存分配错误！";
	}
	for (int i = 0; i < noft; i++)
	{
		tran[i] = r.tran[i];
	}
}

ROUTE::ROUTE(ROUTE&& r)noexcept :tran(r.tran), noft(r.noft) {
	(TRAN*&)(r.tran) = nullptr;
	(int&)(r.noft) = 0;
}

ROUTE::~ROUTE()noexcept {
	if (tran) 
	{
		delete[] tran;
		(TRAN*&)tran = nullptr;
		(int&)noft = 0;
	}
}

int ROUTE::print()const {
	for (int i = 0; i < noft; i++)
	{
		if (tran[i].S() == -1)
		{
			printf("\t乘坐公交%d可直达\n", tran[i].F() + 1);
		}
	}
	return 1;
}

ROUTE::operator int()const { 
	return noft; 
}

int ROUTE::operator==(const ROUTE& r)const {
	int m = 1;
	if (noft != r.noft) return 0;
	for (int x = 0; x < noft; x++) {
		if (tran[x] == r.tran[x]) continue;
		m = 0;
		break;
	}
	return m;
}

ROUTE ROUTE::operator*()const {
	int nn = noft;
	TRAN* t = new TRAN[noft];
	if (t == nullptr)
	{
		throw "ROUTE类operator*操作内存分配错误";
	}
	for (int x = 0; x < nn; x++)
	{
		t[x] = tran[x];
	}
	for (int x = 0; x < nn - 1; x++)
		for (int y = x + 1; y < nn; y++) 
		{
			if (t[x].S() == t[y].S() && t[x].F() == t[y].T()) 
			{
				for (int m = x, n = y + 1; n < nn; n++, m++) t[m] = t[n];
				nn -= (y + 1 - x);
				y = x;
			}
		}
	ROUTE r(t, nn);
	delete[] t;
	return r;
}

TRAN& ROUTE::operator[](int x) { 
	if (x < 0 || x > noft) throw "下标错误";
	return tran[x];
}

ROUTE  ROUTE::operator+(const ROUTE& r)const {
	int x, y;
	ROUTE s;
	if (noft == 0) 
		return *this;
	if (r.noft == 0) 
		return r;
	if (tran[noft - 1].T() != r.tran[0].F())
		throw "Route can nott be concated!";
	try {
		(TRAN*&)(s.tran) = new TRAN[noft + r.noft];
	}
	catch (...) {
		throw "Memory allocation for ROUTE construction error!";
	}
	(int&)(s.noft) = s.tran ? noft + r.noft : 0;
	for (x = 0; x < noft; x++) 
		s.tran[x] = tran[x];
	for (y = 0; y < r.noft; y++) 
		s.tran[x++] = r.tran[y];
	return *s;
}

ROUTE& ROUTE::operator=(const ROUTE& r) {
	if (this == &r) return *this;
	if (tran) delete[]tran;
	try {
		(TRAN*&)tran = new TRAN[r.noft];
	}
	catch (...) {
		throw "Memory allocation for ROUTE construction error!";
	}
	(int&)noft = tran ? r.noft : 0;
	for (int x = 0; x < noft; x++) tran[x] = r.tran[x];
	return *this;
}
ROUTE& ROUTE::operator=(ROUTE&& r) noexcept {
	if (this == &r) return *this;
	if (tran) delete[]tran;
	(TRAN*&)tran = r.tran;
	(int&)noft = r.noft;
	(TRAN*&)(r.tran) = nullptr;
	(int&)(r.noft) = 0;
	return *this;
}

ROUTE& ROUTE::operator+=(const ROUTE& r) { return *this = *this + r; }

//===========================================================================================================

NODE::NODE(ROUTE* p, int n) :p(n ? new ROUTE[n] : nullptr), n(p ? n : 0) {
	if (NODE::p == nullptr && n != 0) throw "memory allocation for ROUTE construction error!";
	for (int x = 0; x < n; x++) NODE::p[x] = p[x];
}

NODE::NODE(int n) : p(n ? new ROUTE[n] : nullptr), n(p ? n : 0) {
	if (p == nullptr && n != 0) throw "memory allocation for NODE construction error!";
}

NODE::NODE(const NODE& n) : p(n.n ? new ROUTE[n.n] : nullptr), n(p ? n.n : 0) {
	if (p == nullptr && n.n != 0) throw "memory allocation for NODE construction error!";
	for (int x = 0; x < NODE::n; x++) p[x] = n.p[x];
}

NODE::NODE(NODE&& n)noexcept : p(n.p), n(n.n) {
	(ROUTE*&)(n.p) = nullptr;
	(int&)n.n = 0;
}

NODE NODE::operator*()const {
	int n = NODE::n;
	if (n == 0) return *this;
	ROUTE* t = new ROUTE[n];
	if (t == nullptr) throw "memory allocation for NODE::operator*() error!";
	for (int x = 0; x < n; x++) t[x] = p[x];
	for (int x = 0; x < n - 1; x++)
		for (int y = x + 1; y < n; y++) {
			if (t[x] == t[y]) {
				for (int m = x + 1, n = y + 1; n < n - 1; n++, m++)
					t[m] = t[n];
				n -= (y - x);
				y = x;
			}
		}
	NODE r(t, n);
	try {
		if (t != nullptr)delete[]t;
		t = nullptr;
	}
	catch (...) {
		throw "initializing failed";
	}
	return r;
}

NODE NODE::operator+(const ROUTE& n)const {
	NODE r(NODE::n + 1);
	for (int x = 0; x < NODE::n; x++) r.p[x] = *p[x];
	r.p[NODE::n] = n;
	return *r;
}

NODE NODE::operator+(const NODE& n)const {
	if (NODE::n == 0) return n;
	if (n.n == 0) return *this;
	NODE r(NODE::n + n.n);
	for (int x = 0; x < NODE::n; x++) r.p[x] = *p[x];
	for (int x = 0; x < n.n; x++) r.p[x + NODE::n] = *n.p[x];
	return *r;
}

NODE NODE::operator*(const NODE& n)const {
	if (NODE::n == 0) return *this;
	if (n.n == 0) return n;
	NODE r(NODE::n * n.n);
	int m, f, h, k = 0;
	for (int x = 0; x < NODE::n; x++)
		for (int y = 0; y < n.n; y++) {
			if (p[x][-1 + p[x]].T() != n.p[y][0].F()) throw "Can not tansship from buses!";
			try {
				r.p[k] = p[x] + n.p[y];
			}
			catch (const char* e) {
				const char* p = e;
			}
			k++;
		}
	return *r;
}

NODE& NODE::operator=(const NODE& n) {
	if (this == &n) return *this;
	if (p) delete[]p;
	(ROUTE*&)p = new ROUTE[n.n];
	if (p == nullptr) throw "Memory allocation for NODE construction error!";
	(int&)(NODE::n) = p ? n.n : 0;
	for (int x = 0; x < n.n; x++) p[x] = n.p[x];
	return *this;
}

NODE& NODE::operator=(NODE&& n)noexcept {
	if (this == &n) return *this;
	if (p) delete[]p;
	(ROUTE*&)p = n.p;
	(int&)(NODE::n) = n.n;
	(ROUTE*&)(n.p) = nullptr;
	(int&)(n.n) = 0;
	return *this;
}

NODE& NODE::operator+=(const ROUTE& n) {
	return *this = *this + n;
}

NODE& NODE::operator+=(const NODE& n) {
	return *this = *this + n;
}

NODE& NODE::operator*=(const NODE& n) {
	return *this = *this * n;
}

ROUTE& NODE::operator [](int x) {
	if (x < 0 || x >= n) throw "Subscription x of NODE::operator [](int x) is wrong!";
	return p[x];
}

NODE::operator int& () { return n; }

NODE::~NODE()noexcept {
	if (p) {
		delete[]p;
		(ROUTE*&)p = nullptr;
		(int&)n = 0;
	}
}

void NODE::print()const {
	for (int m = 0; m < n; m++) {
		p[m].print();
	}
}

//===========================================================================================================

TMAP::TMAP(int r, int c) : p((r != 0 && c != 0) ? new NODE[r * c] : nullptr), r(p ? r : 0), c(p ? c : 0) {
	if (TMAP::p == nullptr && r != 0 && c != 0) throw "Memory allocation for TMAP construction error!";
}

TMAP::TMAP(const TMAP& a) : p((a.r != 0 && a.c != 0) ? new NODE[a.r * a.c] : nullptr), r(p ? a.r : 0), c(p ? a.c : 0) {
	if (p == nullptr && a.r != 0 && a.c != 0) throw "Memory allocation for TMAP construction error!";
	for (int k = r * c - 1; k >= 0; k--) p[k] = a.p[k];
}

TMAP::TMAP(TMAP&& a)noexcept : p(a.p), r(a.r), c(a.c) {
	(NODE*&)(a.p) = nullptr;
	(int&)(a.r) = (int&)(a.c) = 0;
}

TMAP::~TMAP() {
	if (p) {
		delete[] p;
		(NODE*&)p = nullptr;
		(int&)r = (int&)c = 0;
	}
}

int TMAP::notZero()const {
	for (int x = r * c - 1; x >= 0; x--) if (p[x].operator int& () == 0) return 0;
	return 1;
}

int& TMAP::operator()(int x, int y) {
	if (x < 0 || x >= r) throw "Subscript bound error!";
	if (y < 0 || y >= c) throw "Subscript bound error!";
	return p[x * c + y];
}

NODE* TMAP::operator[](int r) {
	if (r < 0 || r >= TMAP::r) throw "Subscript bound error!";
	return p + r * c;
}

TMAP& TMAP::operator=(const TMAP& a) {
	if (this == &a) return *this;
	if (p) delete[]p;
	(NODE*&)p = new NODE[a.r * a.c];
	if (p == nullptr) throw "Memory allocation for TMAP assignment error!";
	(int&)r = a.r;
	(int&)c = a.c;
	for (int k = r * c - 1; k >= 0; k--)
		p[k] = a.p[k];
	return *this;
}

TMAP& TMAP::operator=(TMAP&& a) {
	if (this == &a) return *this;
	if (p) delete[]p;
	(NODE*&)p = a.p;
	(int&)r = a.r;
	(int&)c = a.c;
	(NODE*&)(a.p) = nullptr;
	(int&)(a.r) = (int&)(a.c) = 0;
	return *this;
}

TMAP TMAP::operator*(const TMAP& a)const {
	if (c != a.r) throw "TMAP can nott mutiply!";
	int t, m, u, v, w, x, y, z;
	TMAP s(r, a.c);                 
	for (int h = 0; h < r; h++)
		for (int j = a.c - 1; j >= 0; j--) {
			if (h == j) continue;
			t = h * s.c + j;
			for (int k = 0; k < c; k++)
				if (k != h && k != j) 
					s.p[t] += p[h * c + k] * a.p[k * a.c + j];
		}
	return s;
}

TMAP TMAP::operator+(const TMAP& a)const {
	if (r != a.r && c != a.c) throw "TMAP can nott add!";
	TMAP s(*this);
	for (int h = r * c - 1; h >= 0; h--) s.p[h] += a.p[h];
	return s;
}

TMAP& TMAP::operator+=(const TMAP& a) { return *this = *this + a; }

TMAP& TMAP::operator*=(const TMAP& a) { return *this = *this * a; }

TMAP& TMAP::operator()(int ro, int co, const ROUTE& a) {
	p[ro * c + co] += a;
	return *this;
}

int TMAP::miniTran(int s, int t, int& notf, ROUTE(&r)[100])const {
	int k, u, v, w, x, y, z;
	int b = 0, e = 0; 
	int nott[100]{};      
	int bls[20], els[20];   
	NODE rou;
	for (z = 0; z < GIS::nl; z++) { 
		if (GIS::ls[z].has(s) != -1) if (b < 20) bls[b++] = z;
		if (GIS::ls[z].has(t) != -1) if (e < 20) els[e++] = z;
	}
	for (x = z = 0; x < b; x++)
		for (y = 0; y < e; y++) {
			rou = GIS::tra[bls[x]][els[y]]; 
			w = GIS::tra(bls[x], els[y]);
			if (w == 0) continue;   
			for (v = 0; v < w; v++) {
				u = rou[v];       
				if (z == 0 || u < nott[0]) {
					nott[0] = u;
					r[0] = rou[v];
					z = 1;
				}
				if (u == nott[0]) { 
					if (z == 100) return z;
					nott[z] = u;
					r[z] = rou[v];
					z++;
				}
			}
		}
	notf = nott[0]; 
	return z;    
}

double TMAP::getDist(int b, int e, ROUTE& r) {
	int x, y;
	double d = 0;
	if (1 == r && r[0].F() == r[0].T()) {  
		d = GIS::ls[r[0].F()].dist(b, e);
		return d;
	}
	d = GIS::ls[r[0].F()].dist(b, r[0].S());
	y = (int)r - 1;       
	for (x = 0; x < y; x++)
		d += GIS::ls[r[x].T()].dist(r[x].S(), r[x + 1].S());
	d += GIS::ls[r[y].T()].dist(r[y].S(), e);
	return d;
}

int TMAP::miniDist(int s, int t, double& dist, ROUTE(&r)[100])const {
	int k, u, v, w, x, y, z;
	int b = 0, e = 0;      
	double dot[100]{};      
	int bls[20], els[20];   
	NODE rou;
	for (z = 0; z < GIS::nl; z++) {             
		if (GIS::ls[z].has(s) != -1) if (b < 20) bls[b++] = z;
		if (GIS::ls[z].has(t) != -1) if (e < 20) els[e++] = z;
	}
	for (x = z = 0; x < b; x++)                 
		for (y = 0; y < e; y++) {               
			rou = GIS::tra[bls[x]][els[y]];    
			w = GIS::tra(bls[x], els[y]);       
			if (w == 0) continue;               
			for (v = 0; v < w; v++) {
				u = TMAP::getDist(s, t, rou[v]);
				if (z == 0 || u < dot[0]) {    
					dot[0] = u;
					r[0] = rou[v];
					z = 1;
				}
				if (u == dot[0]) {            
					if (z == 100) return z;
					dot[z] = u;
					r[z] = rou[v];
					z++;
				}
			}
		}
	dist = dot[0];  
	return z;      
}

void TMAP::print()const {
	for (int x = 0; x < r; x++)
		for (int y = 0; y < c; y++) {
			printf("Node(%d,%d) has %d routes:\n", x, y, (int)(p[x * c + y]));
			p[x * c + y].print();
		}

}

//===========================================================================================================

GIS::GIS() { obs++; }

GIS::GIS(const char* flstop, const char* flline) {
	int  m, n, p, q, r, * s, * t;
	FILE* fs, * fl;
	fs = fopen(flstop, "r");
	fl = fopen(flline, "r");
	if (fs == 0 || fl == 0) throw "File open error!";
	fscanf(fs, "%d", &ns);
	st = new STOP[ns];
	for (m = 0; m < ns; m++) {
		fscanf(fs, "%d%d", &st[m].X(), &st[m].Y());
		st[m].N() = m + 1;      
	}
	fclose(fs);
	fscanf(fl, "%d", &nl);
	s = new int[nl];         
	t = new int[100];        
	for (m = 0; m < nl; m++) {
		fscanf(fl, "%d", &s[m]);
	}
	*(LINE**)&ls = new LINE[nl];
	for (m = 0; m < nl; m++) {
		for (n = 0; n < s[m]; n++) {
			fscanf(fl, "%d", &t[n]);
			t[n]--;
		}
		ls[m] = LINE(m + 1, s[m], t);
	}
	fclose(fl);
	for (m = 0; m < nl; m++) {  
		for (p = n = 0; n < nl; n++)
			if (m != n) p += GIS::ls[m].cross(GIS::ls[n]);
		if (p == 0) {
			printf("line %d does nott cross any line\n", m + 1);
			throw  "there is independent line";
		}
	}
	TMAP ra(nl, nl);
	ROUTE a;
	TRAN* u = new TRAN[100];
	for (m = 0; m < nl; m++)
		for (n = 0; n < nl; n++)
		{
			if (m == n) {         
				u[0] = TRAN(m, n, -1);  
				a = ROUTE(&u[0], 1);  
				ra(m, n, a);
				continue;
			}
			p = 0;             
			for (q = GIS::ls[m].NOFS() - 1; q >= 0; q--) {
				r = GIS::ls[m][q];
				if (GIS::ls[n].has(r) != -1)
				{
					u[p] = TRAN(m, n, r);
					a = ROUTE(&u[p++], 1); 
					ra(m, n, a);
				}
			}
		}
	tra = raw = ra;
	for (n = 2; n < nl; n++) {
		raw *= ra;
		tra += raw;
	}
	raw = ra;
	delete s;
	delete t;
	delete[]u;
	obs++;
}

GIS::~GIS() {
	obs--;
	if (obs) return;
	if (st) { delete[]st; *(STOP**)&st = 0; }
	if (ls) { delete[]ls; *(LINE**)&ls = 0; }
}

int GIS::miniTran(int fx, int fy, int tx, int ty, int& f, int& t, int& n, ROUTE(&r)[100]) {
	int m;
	double df, tf, dt, tt;
	f = 0;      
	df = sqrt((st[0].X() - fx) * (st[0].X() - fx) + (st[0].Y() - fy) * (st[0].Y() - fy));
	t = 0;    
	dt = sqrt((st[0].X() - tx) * (st[0].X() - tx) + (st[0].Y() - ty) * (st[0].Y() - ty));
	for (m = 1; m < GIS::ns; m++) {       
		tf = sqrt((st[m].X() - fx) * (st[m].X() - fx) + (st[m].Y() - fy) * (st[m].Y() - fy));
		if (df > tf) { df = tf;  f = m; }   
		tt = sqrt((st[m].X() - tx) * (st[m].X() - tx) + (st[m].Y() - ty) * (st[m].Y() - ty));
		if (dt > tt) { dt = tt;  t = m; }
	}
	if (f == t) return 0;  
	return GIS::tra.miniTran(f, t, n, r);
}

int GIS::miniDist(int fx, int fy, int tx, int ty, int& f, int& t, double& d, ROUTE(&r)[100]) {
	int m;
	double df, tf, dt, tt;
	f = 0;
	df = sqrt((st[0].X() - fx) * (st[0].X() - fx) + (st[0].Y() - fy) * (st[0].Y() - fy));
	t = 0;
	dt = sqrt((st[0].X() - tx) * (st[0].X() - tx) + (st[0].Y() - ty) * (st[0].Y() - ty));
	for (m = 1; m < GIS::ns; m++) {
		tf = sqrt((st[m].X() - fx) * (st[m].X() - fx) + (st[m].Y() - fy) * (st[m].Y() - fy));
		if (df > tf) { df = tf;  f = m; }
		tt = sqrt((st[m].X() - tx) * (st[m].X() - tx) + (st[m].Y() - ty) * (st[m].Y() - ty));
		if (dt > tt) { dt = tt;  t = m; }
	}
	if (f == t) return 0;  
	return GIS::tra.miniDist(f, t, d, r);
}