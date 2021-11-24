#pragma once
class STOP {
	int numb;
	int x, y;
public:
	STOP(int n = 0, int x = 0, int y = 0);
	virtual int& X();
	virtual int& Y();
	virtual int& N();
};

class LINE {
	const int numb;
	int* const stop;
	const int nofs;
public:
	LINE(int numb = 0, int nofs = 0, int* stop = nullptr);
	LINE(const LINE& r);
	LINE(LINE&& r)noexcept;
	LINE& operator=(const LINE& r);
	LINE& operator=(LINE&& r)noexcept;
	virtual int has(int s)const;
	virtual int cross(const LINE& b)const;
	virtual operator int()const;
	virtual int NOFS()const;
	virtual double dist(int d, int a)const;
	virtual int& operator[](int x);
	virtual ~LINE()noexcept;
};

class TRAN {
	int from;
	int to;
	int stop;
public:
	TRAN(int from = 0, int to = 0, int stop = 0);
	int operator==(const TRAN& t)const;
	virtual int& F();
	virtual int& T();
	virtual int& S();
};

class ROUTE {
	TRAN* const tran;
	const int noft;
public:
	ROUTE(TRAN* tran = nullptr, int nofs = 0);
	ROUTE(const TRAN& t);
	ROUTE(const ROUTE& r);
	ROUTE(ROUTE&& r)noexcept;
	virtual operator int()const;
	virtual int operator==(const ROUTE& r)const;
	virtual ROUTE operator*()const;
	virtual TRAN& operator[](int);
	virtual ROUTE operator+(const ROUTE& r)const;
	virtual ROUTE& operator=(const ROUTE& r);
	virtual ROUTE& operator=(ROUTE&& r)noexcept;
	virtual ROUTE& operator+=(const ROUTE& r);
	virtual ~ROUTE()noexcept;
	virtual int print()const;
};

class NODE {
	ROUTE* const p;
	int n;
public:
	NODE(ROUTE* p, int n);
	NODE(int n = 0);
	NODE(const NODE& n);
	NODE(NODE&& n)noexcept;
	virtual NODE operator*()const;
	virtual NODE operator+(const ROUTE& n)const;
	virtual NODE operator+(const NODE& n)const;
	virtual NODE operator*(const NODE& n)const;
	virtual NODE& operator=(const NODE& n);
	virtual NODE& operator+=(const NODE& n);
	virtual NODE& operator+=(const ROUTE& n);
	virtual NODE& operator*=(const NODE& n);
	virtual NODE& operator=(NODE&& n)noexcept;
	virtual ROUTE& operator[](int x);
	virtual operator int& ();
	virtual ~NODE()noexcept;
	virtual void print()const;
};

class TMAP {
	NODE* const p;
	const int r, c;
public:
	TMAP(int r = 0, int c = 0);
	TMAP(const TMAP& a);
	TMAP(TMAP&& a)noexcept;
	virtual ~TMAP();
	virtual int notZero()const;
	virtual int miniTran(int b, int e, int& noft, ROUTE(&r)[100])const;
	virtual int miniDist(int b, int e, double& dist, ROUTE(&r)[100])const;
	static double getDist(int b, int e, ROUTE& r);
	virtual NODE* operator[](int r);
	virtual int& operator()(int r, int c);
	virtual TMAP operator*(const TMAP& a)const;
	virtual TMAP operator+(const TMAP& a)const;
	virtual TMAP& operator=(const TMAP& a);
	virtual TMAP& operator=(TMAP&& a);
	virtual TMAP& operator+=(const TMAP& a);
	virtual TMAP& operator*=(const TMAP& a);
	virtual TMAP& operator()(int r, int c, const ROUTE& a);
	virtual void print()const;
};

struct GIS {
	static STOP* st;
	static LINE* ls;
	static int ns, nl;
	static TMAP raw, tra;
	static int obs;
public:
	GIS();
	GIS(const char* flstop, const char* flline);
	int miniTran(int fx, int fy, int tx, int ty, int& f, int& t, int& n, ROUTE(&r)[100]);
	int miniDist(int fx, int fy, int tx, int ty, int& f, int& t, double& d, ROUTE(&r)[100]);
	~GIS();
};

extern GIS* gis;