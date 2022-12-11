// -------------------------------------------------------------------------------------------------
// Lua Global
// -------------------------------------------------------------------------------------------------
#define GLOBAL_FLOAT(name, defaultval) (Global::floatmap.find(#name) == Global::floatmap.end() ? defaultval : Global::floatmap[#name])
#define GLOBAL_INT(name, defaultval) (Global::intmap.find(#name) == Global::intmap.end() ? defaultval : Global::intmap[#name])
#define GLOBAL_STRING(name, defaultval) (Global::stringmap.find(#name) == Global::stringmap.end() ? defaultval : Global::stringmap[#name])

#define LUA_FLOAT(name, defaultval) (Global::floatmap.find(#name) == Global::floatmap.end() ? defaultval : Global::floatmap[#name])
#define LUA_INT(name, defaultval) (Global::intmap.find(#name) == Global::intmap.end() ? defaultval : Global::intmap[#name])
#define LUA_STRING(name, defaultval) (Global::stringmap.find(#name) == Global::stringmap.end() ? defaultval : Global::stringmap[#name])
#define LUA_VEC3(name, defaultval) (Global::vec3map.find(#name) == Global::vec3map.end() ? defaultval : Global::vec3map[#name])

#define LUAPM_VEC3(start)	vec3(luaparam[start],luaparam[start+1],luaparam[start+2])
#define LUAPM_REAL(index)	luaparam[index]
#define LUAPM_INT(index)	(int)luaparam[index]

real luaparam[100] = { 0 };
// -------------------------------------------------------------------------------------------------
real gpixelsize = 1;
static int rnd(lua_State* L)
{
	if (lua_gettop(L) == 2)
	{
		real min = lua_tonumber(L, 1);
		real max = lua_tonumber(L, 2);

		lua_pushnumber(L, rrnd(min, max));
	}
	else
		lua_pushnumber(L, rrnd(0, 1));
	return 1;
}
static int bnd(lua_State* L)
{
	real min = lua_tonumber(L, 1);
	real max = lua_tonumber(L, 2);
	real a = lua_tonumber(L, 3);

	real pw = 1;
	if (lua_gettop(L) >= 4)
		pw = lua_tonumber(L, 4);

	lua_pushnumber(L, blend(min, max, a, pw));
	return 1;
}
static int atan2(lua_State* L)
{
	real y = lua_tonumber(L, 1);
	real x = lua_tonumber(L, 2);

	lua_pushnumber(L, atan2(y, x));
	return 1;
}
static int pow(lua_State* L)
{
	real x = lua_tonumber(L, 1);
	real y = lua_tonumber(L, 2);

	lua_pushnumber(L, pow(x, y));
	return 1;
}
static int print(lua_State* L)
{
	std::string str = lua_tostring(L, 1);
	PRINT(str);

	return 0;
}
static int pixsize(lua_State* L)
{
	real sz = lua_tonumber(L, 1);
	gpixelsize = sz;
	return 0;
}

static int pixel(lua_State* L)
{
	real x = lua_tonumber(L, 1);
	real y = lua_tonumber(L, 2);
	real z = 0;
	if (lua_gettop(L) >= 3)
		z = lua_tonumber(L, 3);
	if (gpixelsize > 1)
	{
		point(vec(x, y, z), gpixelsize, color);
	}
	else
		pixel(vec(x, y, z), color);
	return 0;
}
static int pixeli(lua_State* L)
{
	int x = lua_tonumber(L, 1);
	int y = lua_tonumber(L, 2);
	if (gpixelsize > 1)
	{
		pointi(x, y, gpixelsize, color);
	}
	else
		pixel(x, y, color);
	return 0;
}

static int pset(lua_State* L)
{
	real x = lua_tonumber(L, 1);
	real y = lua_tonumber(L, 2);
	real z = 0;
	if (lua_gettop(L) >= 3)
		z = lua_tonumber(L, 3);
	//if (gpixelsize > 1)
	//{
	//	PSET(vec(x, y, z), color, gpixelsize);
	//}
	//else
	//	PSET(vec(x, y, z), color);
	return 0;
}
// ------------------------------------------------
static int triang(lua_State* L)
{
	real x1 = lua_tonumber(L, 1);
	real y1 = lua_tonumber(L, 2);
	real z1 = lua_tonumber(L, 3);

	real x2 = lua_tonumber(L, 1 + 3);
	real y2 = lua_tonumber(L, 2 + 3);
	real z2 = lua_tonumber(L, 3 + 3);

	real x3 = lua_tonumber(L, 1 + 6);
	real y3 = lua_tonumber(L, 2 + 6);
	real z3 = lua_tonumber(L, 3 + 6);

	triang0(vec(x1, y1, z1), vec(x2, y2, z2), vec(x3, y3, z3));

	return 0;
}
// ------------------------------------------------
static int triangi(lua_State* L)
{
	int t1 = lua_tonumber(L, 1);
	int t2 = lua_tonumber(L, 2);
	int t3 = lua_tonumber(L, 3);

	if (verliststackpos >= estack.size())
		return 0;

	if (t1 < estack[verliststackpos].size() && t2 < estack[verliststackpos].size() && t3 < estack[verliststackpos].size())
		triang0(estack[verliststackpos][t1], estack[verliststackpos][t2], estack[verliststackpos][t3]);

	return 0;
}

static int plane(lua_State* L)
{
	real x1 = lua_tonumber(L, 1);
	real y1 = lua_tonumber(L, 2);
	real z1 = lua_tonumber(L, 3);

	real x2 = lua_tonumber(L, 1 + 3);
	real y2 = lua_tonumber(L, 2 + 3);
	real z2 = lua_tonumber(L, 3 + 3);

	real x3 = lua_tonumber(L, 1 + 6);
	real y3 = lua_tonumber(L, 2 + 6);
	real z3 = lua_tonumber(L, 3 + 6);

	real x4 = lua_tonumber(L, 1 + 9);
	real y4 = lua_tonumber(L, 2 + 9);
	real z4 = lua_tonumber(L, 3 + 9);

	plane0(vec(x1, y1, z1), vec(x2, y2, z2), vec(x3, y3, z3), vec(x4, y4, z4));

	return 0;
}
static int pyramid(lua_State* L)
{
	real x = lua_tonumber(L, 1);
	real y = lua_tonumber(L, 2);
	real z = lua_tonumber(L, 3);

	real r = lua_tonumber(L, 4);
	real h = r;
	if (lua_gettop(L) >= 5)
		h = lua_tonumber(L, 5);

	pyramid(vec(x, y, z), r, h);
	return 0;
}
static int sphere(lua_State* L)
{
	real x = lua_tonumber(L, 1);
	real y = lua_tonumber(L, 2);
	real z = lua_tonumber(L, 3);

	real r = lua_tonumber(L, 4);

	GenSphere(vec(x, y, z), r);
	return 0;
}
static int cube(lua_State* L)
{
	real x = lua_tonumber(L, 1);
	real y = lua_tonumber(L, 2);
	real z = lua_tonumber(L, 3);

	real w = lua_tonumber(L, 4);
	real h = lua_tonumber(L, 5);
	real l = lua_tonumber(L, 6);
	cube(vec(x, y, z), w, h, l);
	return 0;
}

static int coorddummy(lua_State* L)
{
	real x = lua_tonumber(L, 1);
	real y = lua_tonumber(L, 2);
	real z = lua_tonumber(L, 3);

	color = 0xFFFF0000;
	ptr(vec(x, y, z), vec(x + 0.5, y, z), 0.025);
	color = 0xFF00FF00;
	ptr(vec(x, y, z), vec(x, y + 0.25, z), 0.025);
	color = 0xFF0000FF;
	ptr(vec(x, y, z), vec(x, y, z + 0.125), 0.025);
	return 0;
}
// ------------------------------------------------
// 渲染
// ------------------------------------------------
extern void rayrender();
static int rayrender(lua_State* L)
{
	rayrender();
	return 0;
}

static int scanlinerender(lua_State* L)
{
	int n = lua_gettop(L);
	for (int i = 0; i < n; i++)
		luaparam[i] = lua_tonumber(L, 1 + i);

	return 0;
}

// ------------------------------------------------
// 贝壳形态学
// ------------------------------------------------
std::vector<vec>	dirstack;
std::vector<real>	weightlist;

extern void edgeax(const VECLIST& e, vec& ux, vec& uy, vec& uz);
extern void edgeax(const VECLIST& e, vec& ux, vec& uy, const vec& uz);

std::vector<coord3>	coordstack;

real gwtimes = 1;
bool bmirror = false;
vec gCurPos = vec::ZERO;
vec gUpperDir = vec::UY;
int growtime = 0;
int growpos = -1;

// -------------------------------------------------
void edgeax(const VECLIST& e, vec& ux, vec& uy, vec& uz)
{
	vec o = getedgecenter(e);
	uz = (o - e[0].p).normcopy();
	uy = getedgenorm(estack.back());
	ux = uz.cross(uy);
}
void edgeax2(VECLIST& e, vec& ux, vec& uy, vec& uz)
{
	if (coordstack.empty())
		edgeax(e, ux, uy, uz);
	else
	{
		ux = coordstack.back().ux;
		uy = coordstack.back().uy;
		uz = coordstack.back().uz;
	}
}
vec getedgenorm2(const VECLIST& e)
{
	if (coordstack.empty())
		return getedgenorm(e);
	return coordstack.back().uy;
}
void extrudeedge2(VECLIST& e1, real d)
{
	if (coordstack.empty())
		return extrudeedge(e1, d);

	if (e1.size() < 3)
		return;
	vec norm = getedgenorm2(e1);
	vec dv = norm * d;
	if (weightlist.size() == e1.size())
	{
		for (int i = 0; i < e1.size(); i++)
		{
			e1[i].p += dv * weightlist[i];
			e1[i].ind = -1;
		}
	}
	else
	{
		for (int i = 0; i < e1.size(); i++)
		{
			e1[i].p += dv;
			e1[i].ind = -1;
		}
	}
}
void rotedge2(const VECLIST& e, real ang, const vec& ax, VECLIST& eo)
{
	vec o = getedgecenter(e);
	for (int i = 0; i < e.size(); i++)
	{
		if (bmirror)
		{
			eo.PUSH((e[i].p - o).rotcopy(i < e.size() / 2 ? ang : -ang, ax) + o);
		}
		else
			eo.PUSH((e[i].p - o).rotcopy(ang, ax) + o);
	}
}
// ------------------------------------------------
static int pushe(lua_State* L)
{
	if (estack.empty())
		estack.push_back(VECLIST());
	else
		estack.push_back(estack.back());

	return 0;
}
static int pope(lua_State* L)
{
	int n = 1;
	if (lua_gettop(L) == 1)
		n = lua_tonumber(L, 1);
	for (int i = 0; i < n; i++)
		estack.pop_back();

	return 0;
}
static int closeedge(lua_State* L)
{
	if (lua_gettop(L) == 1)
	{
		bclosed = lua_tonumber(L, 1);
	}
	else
		closeedge(estack.back());


	return 0;
}
static int setpos(lua_State* L)
{
	gCurPos.x = lua_tonumber(L, 1);
	gCurPos.y = lua_tonumber(L, 2);
	gCurPos.z = lua_tonumber(L, 3);

	return 0;
}
static int setepos(lua_State* L)
{
	gCurPos = getedgecenter(estack.back());
	return 0;
}
static int pushcoord(lua_State* L)
{
	coordstack.push_back(coordstack.back());
	return 0;
}
static int calccoord(lua_State* L)
{
	if (lua_gettop(L) == 6)
	{
		vec ux, uy, uz;
		ux.x = lua_tonumber(L, 1);
		ux.y = lua_tonumber(L, 2);
		ux.z = lua_tonumber(L, 3);
		ux.norm();

		uy.x = lua_tonumber(L, 4);
		uy.y = lua_tonumber(L, 5);
		uy.z = lua_tonumber(L, 6);
		uy.norm();

		uz = uy.cross(ux).normcopy();
	}
	return 0;
}
static int coorduz(lua_State* L)
{
	real s = lua_tonumber(L, 1);
	int i = s > 1 ? s : s * (estack.back().size() - 1);
	VECLIST& e = estack.back();
	vec o = (e[0].p == e[e.size() - 1].p) ? getedgecenter(e) : (e[0].p + e[e.size() - 1].p) / 2;

	vec uz = estack.back()[i].p - o;
	uz.norm();
	coordstack.back().uz = uz;
	coordstack.back().uy = getedgenorm2(estack.back());
	coordstack.back().ux = uz.cross(coordstack.back().uy).normcopy();
	coordstack.back().uy = coordstack.back().ux.cross(coordstack.back().uz);

	return 0;
}
static int coordux(lua_State* L)
{
	real s = lua_tonumber(L, 1);
	int i = s > 1 ? s : s * (estack.back().size() - 1);
	VECLIST& e = estack.back();
	vec o = (e[0].p == e[e.size() - 1].p) ? getedgecenter(e) : (e[0].p + e[e.size() - 1].p) / 2;

	vec ux = estack.back()[i].p - o;
	ux.norm();
	coordstack.back().ux = ux;
	coordstack.back().uy = getedgenorm2(estack.back());
	coordstack.back().uz = coordstack.back().uy.cross(ux).normcopy();
	coordstack.back().uy = coordstack.back().ux.cross(coordstack.back().uz);

	return 0;
}
static int popcoord(lua_State* L)
{
	int n = 1;
	if (lua_gettop(L) == 1)
		n = lua_tonumber(L, 1);
	for (int i = 0; i < n; i++)
		coordstack.pop_back();
	return 0;
}
static int mirror(lua_State* L)
{
	bmirror = lua_tonumber(L, 1);
	return 0;
}
static int setdir(lua_State* L)
{
	vec dir = gUpperDir;
	if (lua_gettop(L) == 3)
	{
		dir.x = lua_tonumber(L, 1);
		dir.y = lua_tonumber(L, 2);
		dir.z = lua_tonumber(L, 3);
		dir.norm();
	}
	else if (lua_gettop(L) == 2)
	{
		int v1 = lua_tonumber(L, 1);
		int v2 = lua_tonumber(L, 2);
		dir = estack.back()[v2] - estack.back()[v1];
		dir.norm();
	}
	else
	{
		dir = getedgenorm(estack.back());
	}
	dirstack.push_back(dir);
	return 0;
}
static int popdir(lua_State* L)
{
	dirstack.pop_back();
	return 0;
}
static int roundedge(lua_State* L)
{
	float r = lua_tonumber(L, 1);
	float sig = lua_tonumber(L, 2);
	float ang0 = -PI / 2;
	if (lua_gettop(L) >= 3)
		ang0 = lua_tonumber(L, 3);
	vec up = gUpperDir;
	if (!dirstack.empty())
		up = dirstack.back();
	//PRINT("ang0 " << ang0)
	VECLIST e;
	roundedge(e, gCurPos, up, r, sig, ang0);
	estack.push_back(e);
	return 0;
}
static int arcedge(lua_State* L)
{
	float r = lua_tonumber(L, 1);
	int sig = lua_tonumber(L, 2);
	float ang0 = lua_tonumber(L, 3);
	float dang = lua_tonumber(L, 4);
	vec up = gUpperDir;
	if (!dirstack.empty())
		up = dirstack.back();
	vec dx, dy;
	v2vxvy(up, dx, dy);
	roundedgex(estack.back(), gCurPos, dx, dy, r, sig, dang, ang0);
	return 0;
}
static int curvedge(lua_State* L)
{
	float r = lua_tonumber(L, 1);
	float sig = lua_tonumber(L, 2);
	vec up = gUpperDir;
	if (!dirstack.empty())
		up = dirstack.back();
	vec side = vec::UZ.cross(up).normcopy();
	vec p1 = gCurPos - side * (r / 2);
	vec p2 = gCurPos + side * (r / 2);

	for (int i = 0; i < sig; i++)
	{
		vec p = blend(p1, p2, i / (sig - 1.0f));
		estack.back().push_back(p);
	}
	return 0;
}

static int colori(lua_State* L)
{
	int c = lua_tonumber(L, 1);
	color = c;
	return 0;
}

static int rgb(lua_State* L)
{
	int r = lua_tonumber(L, 1);
	int g = lua_tonumber(L, 2);
	int b = lua_tonumber(L, 3);
	color = RGB(r, g, b);
	return 0;
}
static int hsl(lua_State* L)
{
	real h = lua_tonumber(L, 1);
	real s = lua_tonumber(L, 2);
	real l = lua_tonumber(L, 3);

	COLOR_HSL thsl;
	thsl.hue = h * 255;
	thsl.saturation = s * 255;
	thsl.luminance = l * 255;
	color = HSLtoRGB(&thsl);

	return 0;
}
static int addver(lua_State* L)
{
	if (estack.empty())
		estack.push_back(VECLIST());

	if (verliststackpos >= estack.size())
		return 0;

	vertex v;
	v.p = vec3(lua_tonumber(L, 1), lua_tonumber(L, 2), lua_tonumber(L, 3));
	estack[verliststackpos].push_back(v);
	return 0;
}
static int extrudeedgeex(lua_State* L)
{
	float d = lua_tonumber(L, 1);
	float s = lua_tonumber(L, 2);
	extrudeedgeex(estack.back(), d, s);
	return 0;
}
static int extrudeedge(lua_State* L)
{
	float d = lua_tonumber(L, 1);

	VECLIST& e1 = estack.back();
	vec norm = getedgenorm2(e1);
	vec dv = norm * d;
	if (weightlist.size() == e1.size())
	{
		for (int i = 0; i < e1.size(); i++)
		{
			e1[i].p += dv * blend(0, 1, weightlist[i]);
			e1[i].ind = -1;
		}
	}
	else
	{
		for (int i = 0; i < e1.size(); i++)
		{
			e1[i].p += dv;
			e1[i].ind = -1;
		}
	}

	return 0;
}
static int moveedge(lua_State* L)
{
	if (lua_gettop(L) == 3)
	{
		float x = lua_tonumber(L, 1);
		float y = lua_tonumber(L, 2);
		float z = lua_tonumber(L, 3);

		VECLIST& e = estack.back();
		if (weightlist.size() == e.size())
		{
			VECLIST oe;
			moveedge(estack.back(), vec(x, y, z), oe);
			for (int i = 0; i < e.size() - 1; i++)
			{
				e[i].p = blend(e[i].p, oe[i].p, weightlist[i]);
			}
			e.back().p = e.front().p;
		}
		else
			moveedge(estack.back(), vec(x, y, z));
	}
	else if (lua_gettop(L) == 1)
	{
		float d = lua_tonumber(L, 1);
		VECLIST& e = estack.back();
		if (weightlist.size() == e.size())
		{
			VECLIST oe;
			moveedge(estack.back(), dirstack.back() * d, oe);
			for (int i = 0; i < e.size() - 1; i++)
			{
				e[i].p = blend(e[i].p, oe[i].p, weightlist[i]);
			}
			e.back().p = e.front().p;
		}
		else
			moveedge(estack.back(), dirstack.back() * d);
	}
	return 0;
}
static int rotedge(lua_State* L)
{
	float ang = lua_tonumber(L, 1);

	VECLIST& e = estack.back();
	VECLIST oe;
	rotedge2(e, ang, dirstack.back(), oe);

	if (weightlist.size() == e.size())
	{
		for (int i = 0; i < e.size(); i++)
			e[i].p = blend(e[i].p, oe[i].p, weightlist[i]);
	}
	else
		e = oe;

	return 0;
}
static int rotedgemirror(lua_State* L)
{
	VECLIST& e = estack.back();
	int len = e.size();

	//real fcenter = lua_tonumber(L, 1);
	//int center = fcenter > 1 ?	fcenter : len * fcenter;
	int center = len / 2;

	float ang1 = lua_tonumber(L, 1);
	float ang2 = lua_tonumber(L, 2);

	real pw = 1;
	if (lua_gettop(L) >= 3)
		pw = lua_tonumber(L, 3);

	//PRINT("len=" << len);
	VECLIST oe = e;
	{
		vec n = getedgenorm2(e);
		vec o = len % 2 == 0 ? (e[center - 1].p + e[center].p) / 2 : e[center].p;
		for (int offset = 0; offset < len / 2; offset++)
		{
			real ang = blend(ang1, ang2, (offset + 1) / real(len / 2), pw);
			int ind = (center - offset - 1);
			if (ind >= 0)
			{
				oe[ind] = ((e[ind].p - o).rotcopy(-ang, n) + o);
				//PRINT("rotedgemirror1 " << ind << " ang=" << ang);
			}
		}
		for (int offset = 1; offset <= len / 2; offset++)
		{
			real ang = blend(ang1, ang2, offset / real(len / 2), pw);
			int ind = (center + offset);
			if (ind < len)
			{
				oe[ind] = ((e[ind].p - o).rotcopy(ang, n) + o);
				//PRINT("rotedgemirror2 " << ind << " ang=" << ang);
			}
		}
	}
	if (weightlist.size() == len)
	{
		for (int i = 0; i < len; i++)
			e[i].p = blend(e[i].p, oe[i].p, weightlist[i]);
	}
	else
		e = oe;

	return 0;
}
static int rotonedge(lua_State* L)
{
	float ang = lua_tonumber(L, 1);
	float s = lua_tonumber(L, 2);

	VECLIST& e = estack.back();
	VECLIST& re = estack[estack.size() + s];
	if (re.size() == e.size())
	{
		VECLIST oe;
		vec ro = getedgecenter(re);
		vec n = getedgenorm2(re);
		for (int i = 0; i < e.size(); i++)
		{
			vec ax = (n.cross(re[i].p - ro)).normcopy();
			oe.PUSH((e[i] - re[i]).rotcopy(ang, ax) + re[i]);
		}

		if (weightlist.size() == e.size())
		{
			for (int i = 0; i < e.size(); i++)
				e[i].p = blend(e[i].p, oe[i].p, weightlist[i]);
		}
		else
			e = oe;
	}
	return 0;
}
static int yawedge(lua_State* L)
{
	float ang = lua_tonumber(L, 1);
	vec enorm = getedgenorm2(estack.back());
	rotedge(estack.back(), ang, enorm);

	coordstack.back().ux.rot(ang, enorm);
	coordstack.back().uz.rot(ang, enorm);
	return 0;
}
static int pitchedge(lua_State* L)
{
	float ang = lua_tonumber(L, 1);
	VECLIST& e = estack.back();
	vec ux, uy, uz;
	edgeax2(e, ux, uy, uz);
	if (weightlist.size() == e.size())
	{
		VECLIST oe;
		rotedge(e, ang, ux, oe);
		for (int i = 0; i < e.size(); i++)
		{
			e[i].p = blend(e[i].p, oe[i].p, weightlist[i]);
		}
	}
	else
	{
		rotedge(e, ang, ux);
	}
	coordstack.back().uy.rot(ang, ux);
	coordstack.back().uz.rot(ang, ux);
	return 0;
}
static int rolledge(lua_State* L)
{
	float ang = lua_tonumber(L, 1);
	VECLIST& e = estack.back();
	vec ux, uy, uz;
	edgeax2(e, ux, uy, uz);
	rotedge(e, ang, uz);
	coordstack.back().uy.rot(ang, uz);
	coordstack.back().ux.rot(ang, uz);
	return 0;
}
void wscaleedge(VECLIST& e, real sx, real sz)
{
	ASSERT(e.size() > 2)
		bool bwgt = weightlist.size() == e.size();
	vec o;
	if (bwgt)
	{
		int n = (e[0].p == e[e.size() - 1].p) ? e.size() - 1 : e.size();
		vec oo = getedgecenter(e, n);
		for (int i = 0; i < n; i++)
		{
			o = o + (e[i].p - oo) * weightlist[i];
		}
		o = oo + o / n;
	}
	else
		o = getedgecenter(e);


	vec ux, uy, uz;
	edgeax2(e, ux, uy, uz);
	for (int i = 0; i < e.size(); i++)
	{
		vertex& v = e[i];
		vec d = v.p - o;
		vec dx = ux * d.dot(ux);
		vec dz = uz * d.dot(uz);
		if (bwgt)
		{
			real sx2 = blend(1, sx, weightlist[i]);
			real sz2 = blend(1, sz, weightlist[i]);
			d = dx * sx2 + dz * sz2;
		}
		else
		{
			d = dx * sx + dz * sz;
		}
		v.p = o + d;
		v.ind = -1;
	}
}
void wscaleedge2(VECLIST& e, real s)
{
	ASSERT(e.size() > 2);
	bool bwgt = weightlist.size() == e.size();
	vec o = getedgecenter(e);

	int len = e.size();
	int sig = len / gwtimes;
	int n = len / sig;
	for (int j = 0; j < n; j++)
		for (int i = 0; i < sig / 2; i++)
		{
			vertex& v1 = e[j * sig + i];
			vertex& v2 = e[j * sig + sig - 1 - i];
			if (bwgt)
			{
				vec tp = v1.p;
				v1.p = blend(v1.p, v2.p, s * weightlist[i]);
				v2.p = blend(v2.p, tp, s * weightlist[i]);
			}
			else
			{
				vec tp = v1.p;
				v1.p = blend(v1.p, v2.p, s);
				v2.p = blend(v2.p, tp, s);
			}
			v1.ind = -1;
			v2.ind = -1;
		}
}
static int scaleedge(lua_State* L)
{
	if (lua_gettop(L) == 1)
	{
		float s = lua_tonumber(L, 1);
		wscaleedge(estack.back(), s, s);
	}
	else if (lua_gettop(L) == 2)
	{
		float sx = lua_tonumber(L, 1);
		float sz = lua_tonumber(L, 2);

		wscaleedge(estack.back(), sx, sz);
	}
	return 0;
}
static int scaleedgemirror(lua_State* L)
{
	VECLIST& e = estack.back();
	int len = e.size();

	int center = len / 2;
	/*real fcenter = lua_tonumber(L, 1);
	if (fcenter > 1)
		center = fcenter;
	else*/
	//center = len * center;
	float s1 = lua_tonumber(L, 1);
	float s2 = lua_tonumber(L, 2);
	real pw = 1;
	if (lua_gettop(L) >= 3)
		pw = lua_tonumber(L, 3);

	//PRINT("len=" << len);
	VECLIST oe = e;
	{
		vec n = getedgenorm2(e);
		vec o = len % 2 == 0 ? (e[center - 1].p + e[center].p) / 2 : e[center].p;
		for (int offset = 0; offset < len / 2; offset++)
		{
			real s = blend(s1, s2, (offset + 1) / real(len / 2), pw);
			int ind = (center - offset - 1);
			if (ind >= 0)
			{
				oe[ind] = ((e[ind].p - o) * s + o);
				//PRINT("rotedgemirror1 " << ind << " ang=" << ang);
			}
			//o = e[ind].p;
		}
		//o = len % 2 == 0 ? (e[center - 1] + e[center]) / 2 : e[center];
		for (int offset = 1; offset <= len / 2; offset++)
		{
			real s = blend(s1, s2, offset / real(len / 2), pw);
			int ind = (center + offset);
			if (ind < len)
			{
				oe[ind] = ((e[ind].p - o) * s + o);
				//PRINT("rotedgemirror2 " << ind << " ang=" << ang);
			}
			//o = e[ind].p;
		}
	}
	if (weightlist.size() == len)
	{
		for (int i = 0; i < len; i++)
			e[i].p = blend(e[i].p, oe[i].p, weightlist[i]);
	}
	else
		e = oe;


	return 0;
}
static int scaleeedge2(lua_State* L)
{
	if (lua_gettop(L) == 1)
	{
		float s = lua_tonumber(L, 1);
		wscaleedge2(estack.back(), s);
	}
	return 0;
}
static int newedge(lua_State* L)
{
	estack.push_back(VECLIST());
	verliststackpos = estack.size() - 1;
	return 0;
}
static int edge(lua_State* L)
{
	VECLIST e;
	int num = lua_gettop(L);
	for (int i = 0; i < num; i++)
	{
		int index = lua_tonumber(L, i + 1);
		if (index >= 0 && index < gsubmesh->vertices.size())
		{
			e.push_back(gsubmesh->vertices[index]);
		}
	}
	estack.push_back(e);
	return 0;
}
static int setedge(lua_State* L)
{
	VECLIST& e = estack.back();

	vec o = getedgecenter(e);

	vec n = getedgenorm2(e);
	vec ux = (e[0].p - o).normcopy();
	vec uz = n.cross(ux);

	real dang = (PI * 2) / real(e.size() - 1);
	int num = MIN(e.size(), lua_gettop(L));
	for (int i = 0; i < num; i++)
	{
		real s = lua_tonumber(L, i + 1);

		vertex& v = e[i];
		real ang = i * dang;
		v.p = (ux * cos(ang) + uz * sin(ang)) * s + o;
	}
	e.back() = e.front();
	return 0;
}

static int weight(lua_State* L)
{
	weightlist.clear();
	if (0 == lua_gettop(L))
		return 0;
	real w = lua_tonumber(L, 1);
	VECLIST& e = estack.back();
	for (int i = 0; i < e.size(); i++)
	{
		weightlist.push_back(w);
	}
	return 0;
}
static int weightblend(lua_State* L)
{
	real s1 = lua_tonumber(L, 1);
	real s2 = lua_tonumber(L, 2);
	real power = lua_tonumber(L, 3);

	VECLIST& e = estack.back();
	for (int i = 0; i < e.size(); i++)
	{
		real ai = i / real(e.size() - 1);
		real w = blendn(s1, s2, ai, gwtimes, power);

		if (i < weightlist.size())
			weightlist[i] *= w;
		else
			weightlist.push_back(w);
	}
	return 0;
}
static int weightdiv(lua_State* L)
{
	real dv = lua_tonumber(L, 1);
	real w1 = lua_tonumber(L, 2);
	real w2 = lua_tonumber(L, 3);
	real power = 1;
	if (lua_gettop(L) == 4)
		power = lua_tonumber(L, 4);
	VECLIST& e = estack.back();
	for (int i = 0; i < e.size(); i++)
	{
		real ai = i / real(e.size() - 1);
		real w;
		{
			if (ai < dv)
			{
				w = ((dv - ai) / dv);
				w = pow(fabs(sin(w * PI)), power);
				w = blend(w1, w2, w);
			}
			else
			{
				w = ((ai - dv) / (1 - dv));
				w = pow(fabs(sin(w * PI)), power);
				w = blend(w1, w2, w);
			}
		}
		if (i < weightlist.size())
			weightlist[i] *= w;
		else
			weightlist.push_back(w);
	}
	return 0;
}
static int weightblendx(lua_State* L)
{
	real s1 = lua_tonumber(L, 1);
	real s2 = lua_tonumber(L, 2);
	real pow = lua_tonumber(L, 3);

	VECLIST& e = estack.back();
	vec o = getedgecenter(e);
	vec ux, uy, uz;
	edgeax2(e, ux, uy, uz);

	real r = (e[0] - o).len();
	for (int i = 0; i < e.size(); i++)
	{
		vec d = e[i].p - o;
		real a = fabs(d.dot(ux)) / (r);
		real w = blend(s1, s2, a, pow);
		if (i < weightlist.size())
			weightlist[i] *= w;
		else
			weightlist.push_back(w);
	}
	return 0;
}
static int weightblendz(lua_State* L)
{
	real s1 = lua_tonumber(L, 1);
	real s2 = lua_tonumber(L, 2);
	real pow = lua_tonumber(L, 3);

	VECLIST& e = estack.back();
	vec o = getedgecenter(e);
	vec ux, uy, uz;
	edgeax2(e, ux, uy, uz);

	real r = (e[0] - o).len();
	for (int i = 0; i < e.size(); i++)
	{
		vec d = e[i].p - o;
		real a = fabs(d.dot(uz)) / (r);
		real w = blend(s1, s2, a, pow);
		if (i < weightlist.size())
			weightlist[i] *= w;
		else
			weightlist.push_back(w);
	}
	return 0;
}
static int weightblendmirror(lua_State* L)
{
	VECLIST& e = estack.back();
	int len = e.size();

	int center = len / 2;
	float s1 = lua_tonumber(L, 1);
	float s2 = lua_tonumber(L, 2);
	real pw = 1;
	if (lua_gettop(L) >= 3)
		pw = lua_tonumber(L, 3);

	//PRINT("len=" << len);
	VECLIST oe = e;
	{
		vec n = getedgenorm2(e);
		vec o = len % 2 == 0 ? (e[center - 1].p + e[center].p) / 2 : e[center].p;
		for (int offset = 0; offset < len / 2; offset++)
		{
			real s = blend(s1, s2, (offset + 1) / real(len / 2), pw);
			int ind = (center - offset - 1);
			if (ind >= 0)
			{
				real w = s;
				if (ind < weightlist.size())
					weightlist[ind] *= w;
				else
					weightlist.push_back(w);
			}
		}
		for (int offset = 1; offset <= len / 2; offset++)
		{
			real s = blend(s1, s2, offset / real(len / 2), pw);
			int ind = (center + offset);
			if (ind < len)
			{
				real w = s;
				if (ind < weightlist.size())
					weightlist[ind] *= w;
				else
					weightlist.push_back(w);
			}
		}
	}
	if (weightlist.size() == len)
	{
		for (int i = 0; i < len; i++)
			e[i].p = blend(e[i].p, oe[i].p, weightlist[i]);
	}
	else
		e = oe;


	return 0;
}
static int wtimes(lua_State* L)
{
	gwtimes = 1;
	if (lua_gettop(L) == 1)
		gwtimes = lua_tonumber(L, 1);

	return 0;
}
static int radedge(lua_State* L)
{
	VECLIST& e = estack.back();
	vec o = getedgecenter(e);
	vec ux, uy, uz;
	edgeax2(e, ux, uy, uz);

	real r = lua_tonumber(L, 1);

	r = r * (e[0] - o).len();

	for (int i = 0; i < e.size(); i++)
	{
		real ai = i / real(e.size() - 1);
		real ang = ai * PI * 2 - PI / 2;
		if (weightlist.size() == e.size())
			e[i].p = blend(e[i].p, o + ux * (r * cos(ang)) + uz * (r * sin(ang)), weightlist[i]);
		else
			e[i].p = o + ux * (r * cos(ang)) + uz * (r * sin(ang));
	}
	return 0;
}
static int face(lua_State* L)
{
	if (lua_gettop(L) == 2)
	{
		int s1 = lua_tonumber(L, 1);
		int s2 = lua_tonumber(L, 2);
		face(estack[estack.size() + s1], estack[estack.size() + s2]);
	}
	else
	{
		face(estack[estack.size() - 2], estack.back());
	}
	return 0;
}
static int faceo(lua_State* L)
{
	if (lua_gettop(L) == 3)
	{
		float x = lua_tonumber(L, 2);
		float y = lua_tonumber(L, 3);
		float z = lua_tonumber(L, 4);
		face(estack.back(), vec(x, y, z));
	}
	else if (lua_gettop(L) == 1)
	{
		float d = lua_tonumber(L, 2);
		face(estack.back(), getedgecenter(estack.back()) + getedgenorm2(estack.back()) * d);
	}
	else
	{
		float d = lua_tonumber(L, 2);
		face(estack.back(), getedgecenter(estack.back()));
	}
	return 0;
}
static int face12(lua_State* L)
{
	if (lua_gettop(L) == 3)
	{
		int s1 = lua_tonumber(L, 1);
		int s2 = lua_tonumber(L, 2);
		face12(estack[estack.size() + s1], estack[estack.size() + s2]);
	}
	else
	{
		face12(estack[estack.size() - 2], estack.back());
	}
	return 0;
}
static int subedge(lua_State* L)
{
	int start = lua_tonumber(L, 1);
	int sublen = lua_tonumber(L, 2);
	VECLIST e;
	subedge(estack.back(), start, sublen, e);
	estack.back() = e;
	return 0;
}
static int div(lua_State* L)
{
	int start = lua_tonumber(L, 1);
	real s2 = lua_tonumber(L, 2);
	int sublen = s2 < 1 ? s2 * estack.back().size() : s2;

	VECLIST e1, e2;
	subedge2(estack.back(), start, sublen, e1, e2);
	//PRINT("DIV:" << e1.size() << "," << e2.size());
	estack.push_back(e2);
	estack.push_back(e1);
	return 0;
}

static int linkedge(lua_State* L)
{
	int ls = lua_tonumber(L, 1);
	bool binv = 0;
	if (lua_gettop(L) == 2)
		binv = lua_tonumber(L, 2);
	linkedge(estack[estack.size() - 1], estack[estack.size() + ls], binv);
	return 0;
}

static int unionedge(lua_State* L)
{
	int s1 = lua_tonumber(L, 1);
	int s2 = lua_tonumber(L, 2);
	int start = lua_tonumber(L, 3);
	int len = lua_tonumber(L, 4);
	VECLIST e;
	unionedge(estack[estack.size() + s1], estack[estack.size() + s2], start, len, e);
	estack.back() = e;
	return 0;
}
static int insertver(lua_State* L)
{
	int ind = lua_tonumber(L, 1);
	{
		float x = lua_tonumber(L, 2);
		float y = lua_tonumber(L, 3);
		float z = lua_tonumber(L, 4);

		estack.back().insert(estack.back().begin() + ind, vec(x, y, z));
	}
	return 0;
}
static int removever(lua_State* L)
{
	int ind = lua_tonumber(L, 1);
	if (ind < estack.back().size())
		estack.back().erase(estack.back().begin() + ind);
	return 0;
}
static int commonvert(lua_State* L)
{
	int b = lua_tonumber(L, 1);
	gcommonvertex = b;
	return 0;
}
static int searchcomvert(lua_State* L)
{
	int b = lua_tonumber(L, 1);
	gsearchcomvertex = b;
	gcommonvertex = b;
	return 0;
}
static int rendermod(lua_State* L)
{
	int a = lua_tonumber(L, 1);
	renderstate = a;
	return 0;
}
static int msgbox(lua_State* L)
{
	std::string str = lua_tostring(L, 1);
	MSGBOX(str.c_str());
	return 0;
}
static int getchar(lua_State* L)
{
	getchar();
	return 0;
}

static int invnorm(lua_State* L)
{
	int b = lua_tonumber(L, 1);
	binvnorm = b;
	return 0;
}

static int doublevnum(lua_State* L)
{
	int n = 0;
	if (lua_gettop(L) == 1)
		n = lua_tonumber(L, 1);

	doublevnum(estack.back(), n);

	return 0;
}
static int moveedgeex(lua_State* L)
{
	int n = lua_gettop(L);
	std::vector<real> slist;
	for (int i = 0; i < n; i++)
	{
		slist.push_back(lua_tonumber(L, 1));
	}
	moveedgeex(estack.back(), slist);

	return 0;
}

static int clearsm(lua_State* L)
{
	resetsm();

	return 0;
}
static int clearstack(lua_State* L)
{
	estack.clear();

	return 0;
}

//------------------------------------------
// 参数
//------------------------------------------
static int setparam(lua_State* L)
{
	int n = lua_gettop(L);
	for (int i = 0; i < n; i++)
		luaparam[i] = lua_tonumber(L, 1 + i);
	return 0;
}
static int setfloatmap(lua_State* L)
{
	std::string key = lua_tostring(L, 1);
	float val = (float)lua_tonumber(L, 2);
	PRINT("setfloat" << key << "=" << val);
	Global::floatmap[key] = val;
	return 0;
}
static int getfloatmap(lua_State* L)
{
	std::string key = lua_tostring(L, 1);
	if (Global::floatmap.find(key) == Global::floatmap.end())
	{
		PRINT("!getfloat:" << key);
		lua_pushnumber(L, 0);
		return 1;
	}
	float val = Global::floatmap[key];
	PRINT("setfloat " << key << "=" << val);
	lua_pushnumber(L, val);
	return 1;
}
static int setintmap(lua_State* L)
{
	std::string key = lua_tostring(L, 1);
	int val = (int)lua_tointeger(L, 2);
	PRINT("setint " << key << "=" << val);
	Global::intmap[key] = val;
	return 0;
}
static int setstringmap(lua_State* L)
{
	std::string key = lua_tostring(L, 1);
	std::string val = lua_tostring(L, 2);
	PRINT("setstring " << key << "=" << val);
	Global::stringmap[key] = val;
	return 0;
}
static int setvec3map(lua_State* L)
{
	std::string key = lua_tostring(L, 1);
	float x = (float)lua_tonumber(L, 2);
	float y = (float)lua_tonumber(L, 3);
	float z = (float)lua_tonumber(L, 4);
	PRINT("setvec3 " << key << "=" << x << "," << y << "," << z);
	Global::vec3map[key] = vec3(x, y, z);
	return 0;
}
static int getvec3map(lua_State* L)
{
	std::string key = lua_tostring(L, 1);
	if (Global::vec3map.find(key) == Global::vec3map.end())
	{
		PRINT("!getvec3:" << key);
		lua_pushnumber(L, 0);
		return 1;
	}
	vec3 v = Global::vec3map[key];
	PRINTV3(v);
	lua_pushnumber(L, v.x);
	lua_pushnumber(L, v.y);
	lua_pushnumber(L, v.z);
	return 3;
}
static int movepointn(lua_State* L)
{
	PRINT("movepointn")
		// LUA -> CC
		int  ind = lua_gettop(L);
	lua_pushnil(L);
	PRINT("movepointn 1_" << ind);
	pointn p;
	while (lua_next(L, 1))
	{
		PRINT("movepointn 22");
		std::string  k = lua_tostring(L, -2);
		PRINT("movepointn 222");
		float v = lua_tonumber(L, -1);

		p[k] = (int)v;
		lua_pop(L, 1);
	}
	PRINT("movepointn 2");
	// CC -> LUA
	int index = lua_tonumber(L, 2); PRINT(index);
	real delta = lua_tonumber(L, 3);

	lua_newtable(L);
	lua_pushstring(L, "x");
	lua_pushnumber(L, index == 0 ? p[0] + delta : p[0]);
	lua_settable(L, -3);

	lua_pushstring(L, "y");
	lua_pushnumber(L, index == 1 ? p[1] + delta : p[1]);
	lua_settable(L, -3);

	lua_pushstring(L, "z");
	lua_pushnumber(L, index == 2 ? p[2] + delta : p[2]);
	lua_settable(L, -3);
	PRINT("movepointn 2")
		return 1;
}
static int cclambda(lua_State* L)
{
	std::string s = lua_tostring(L, 1);
	Global::lambda = s;
	return 0;
}
//------------------------------------------
// api
//------------------------------------------
using lua_hdr_t = std::function<void()>;
std::map<std::string, lua_hdr_t> luahdrmap;
static int api(lua_State* L)
{
	int n = lua_gettop(L);

	ASSERT(n > 0);

	std::string funname = lua_tostring(L, 1);

	std::stringstream sstr;
	for (int i = 1; i < n; i++)
	{
		luaparam[i] = lua_tonumber(L, 1 + i);
		if (i > 1) sstr << ",";
		sstr << luaparam[i];
	}
	PRINT("api: " << funname << " (" << sstr.str() << ")");

	if (luahdrmap.find(funname) != luahdrmap.end())
	{
		luahdrmap[funname]();
	}

	return 0;
}
//------------------------------------------
// cell
//------------------------------------------
const int cvSIZEX = 1100;
const int cvSIZEY = 600;
struct cell_t
{
	cell_t() {}
	cell_t(real x)
	{
		fdat = x;
	}
	real fdat = 0;
	std::string strdat;
	std::string script;

	std::string tostring()
	{
		/*if (script != "")
		{
			dolua(script);
		}*/
		if (strdat != "")
			return strdat;
		if (fdat == int(fdat))
			return to_string(int(fdat));
		return to_string(fdat);
	}
};
std::map<int, cell_t> sheet;

//-----------------------------------
inline std::string getcellstr(int i)
{
	if (sheet.find(i) == sheet.end())
		return "";
	cell_t& cd = sheet[i];
	return cd.tostring();
}
static int celli(lua_State* L)
{
	int n = lua_gettop(L);
	if (n == 1)
	{
		int pos = lua_tonumber(L, 1);
		int val = sheet[pos].fdat;
		lua_pushnumber(L, val);
		return 1;
	}
	else if (n == 2)
	{
		int pos = lua_tonumber(L, 1);
		int val = lua_tonumber(L, 2);
		if (sheet.find(pos) == sheet.end())
			sheet[pos] = cell_t(val);
		else
			sheet[pos].fdat = val;
	}
	return 0;
}
static int cells(lua_State* L)
{
	int n = lua_gettop(L);
	if (n == 1)
	{
		int pos = lua_tonumber(L, 1);
		std::string strdat = sheet[pos].strdat;
		lua_pushstring(L, strdat.c_str());
		return 1;
	}
	else if (n == 2)
	{
		int pos = lua_tonumber(L, 1);
		std::string strdat = lua_tostring(L, 2);
		if (sheet.find(pos) == sheet.end())
			sheet[pos] = cell_t();
		sheet[pos].strdat = strdat;
	}
	return 0;
}
static int cellscript(lua_State* L)
{
	int pos = lua_tonumber(L, 1);
	string script = lua_tostring(L, 2);
	if (sheet.find(pos) == sheet.end())
		sheet[pos] = cell_t();
	sheet[pos].script = script;

	return 0;
}
static int gcurcell = 0;
static int curcell(lua_State* L)
{
	lua_pushnumber(L, gcurcell);
	return 1;
}

/*
//------------------------------------------
// cvwindow
//------------------------------------------
using namespace cv;
int gradnum = 50;
std::vector<KeyPoint> keyPoints1;
std::vector<KeyPoint> keyPoints2;
Mat image1;
Mat image2;

static void testcv1()
{
	{
		image1 = imread("C:\\Users\\18858\\Documents\\LAB\\ZEXE\\TESTIMG\\1.jpg");
		Ptr<FeatureDetector> fast = FastFeatureDetector::create(30);
		// feature point detection
		fast->detect(image1, keyPoints1);
		PRINTV(keyPoints1.size());
	}
	{
		image2 = imread("C:\\Users\\18858\\Documents\\LAB\\ZEXE\\TESTIMG\\2.jpg");
		Ptr<FeatureDetector> fast = FastFeatureDetector::create(30);
		// feature point detection
		fast->detect(image2, keyPoints2);
		PRINTV(keyPoints2.size());
	}
	//drawKeypoints(image1, keyPoints1, image1, Scalar(255,0,0), DrawMatchesFlags::DRAW_OVER_OUTIMG);
	//drawKeypoints(image2, keyPoints2, image2, Scalar(255, 0, 0), DrawMatchesFlags::DRAW_OVER_OUTIMG);
	//imshow("FAST feature1", image1);
	//imshow("FAST feature2", image2);

	{
		auto orb = ORB::create();
		Mat descriptors_tem, descriptors_test;
		orb->compute(image1, keyPoints1, descriptors_tem);
		orb->compute(image2, keyPoints2, descriptors_test);

		auto matcher = DescriptorMatcher::create(DescriptorMatcher::MatcherType::BRUTEFORCE_HAMMING);
		vector<DMatch> matches;
		matcher->match(descriptors_tem, descriptors_test, matches, Mat());
		Mat result;
		drawMatches(image1, keyPoints1, image2, keyPoints2, matches, result, Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
		imshow("result", result);
	}
}
std::vector<cv::Vec4i> lines;
void drawDetectedLines(cv::Mat& image, cv::Scalar color = cv::Scalar(255, 255, 255))
{
	std::vector<cv::Vec4i>::const_iterator it = lines.begin();
	while (it != lines.end())
	{
		cv::Point pt1((*it)[0], (*it)[1]);
		cv::Point pt2((*it)[2], (*it)[3]);
		cv::line(image, pt1, pt2, color);
		it++;
	}
}

int cvtestline()
{
	cv::Mat image = cv::imread("image_1.jpg");
	cv::Mat canny_img;
	cv::Canny(image, canny_img, 350, 400, 3);

	//cv::imshow("Canny Image", canny_img);

	{//闭操作，先膨胀后腐蚀，可消除小黑点
		Mat kernel = getStructuringElement(MORPH_RECT, Size(7, 7));
		morphologyEx(canny_img, canny_img, MORPH_CLOSE, kernel);
	}
	cv::imshow("Canny Image", canny_img);
	//进行霍夫线变换
	cv::HoughLinesP(canny_img, //输入图像，为单通道二进制图像
		lines, //检测到的线段矢量
		1, //以像素为单位的距离精度
		CV_PI / 180, //以弧度为单位的角度精度
		100, //累加平面的阈值参数
		100, //最低线段的长度
		5);//允许将同一行点与点之间连接起来的最大距离
	PRINTV(lines.size())
	drawDetectedLines(image, cv::Scalar(55, 55, 255));

	cv::imshow("result", image);

	cvWaitKey();
}
int cvtestcurve()
{
	cv::Mat image = cv::imread("image_1.jpg");
	imshow("image", image);
	//cv::Mat contours;
	//cv::Canny(image, contours, 350, 400, 3);
	//cv::imshow("Canny Image", contours);

	Mat canny_img;
	Canny(image, canny_img, 55, 110, 3);
	//cv::imshow("Canny Image", canny_img);

	{//闭操作，先膨胀后腐蚀，可消除小黑点
		Mat kernel = getStructuringElement(MORPH_RECT, Size(7, 7));
		morphologyEx(canny_img, canny_img, MORPH_CLOSE, kernel);
	}
	imshow("canny_img", canny_img);

	Mat dst = Mat(canny_img.rows, canny_img.cols, CV_8UC3, Scalar(0, 0, 0));

	vector<vector<Point> >contours;
	findContours(canny_img, contours, RETR_TREE, CHAIN_APPROX_NONE);

	//getchar();
	drawContours(dst, contours, -1, Scalar(0, 255, 0), 1);
	cv::imshow("dst", dst);

	cvWaitKey();
}
static void cvinitwindow()
{
	if (cvframe.empty())
	{
		cvframe = cv::Mat(cv::Size(cvSIZEX, cvSIZEY), CV_8UC3);
		cvui::init("Form1", 20);
		//rectangle(cvframe, cv::Rect(0, 0, 800, 800), cv::Scalar(50, 50, 50), -1, 8, 0);
	}
	sheet.clear();
	timers.clear();
	cvimages.clear();
}
static void cvwindow()
{
	//cvtestline();

	if (cvframe.empty())
	{
		cvinitwindow();
		return;
	}

	while (true)
	{// window loop
		//rectangle(cvframe, cv::Rect(0, 0, cvSIZEX, cvSIZEY), cv::Scalar(0, 0, 0), -1, 8, 0);

		//if(renderstate == -2)
		{// 网格
			int width = cvSIZEX;
			int height = cvSIZEY;
			int sz = gradnum;

			for (int i = 0; i < height; i += sz)
				cv::line(cvframe, cv::Point(0, i), cv::Point(width, i), cv::Scalar(128, 128, 128));
			for (int i = 0; i < width; i += sz)
				cv::line(cvframe, cv::Point(i, 0), cv::Point(i, height), cv::Scalar(128, 128, 128));
		}

		dolua("form.lua");

		framecnt++;

		for (auto it : timers)
		{
			it.second.update();
		}

		{// sheet
			std::map<int, cell_t>::iterator it = sheet.begin();
			while (it != sheet.end())
			{
				int pos = it->first - 1;
				int wordlen = it->second.tostring().length();
				cv::Rect rect = cv::Rect(100 * (pos % 10), 25 * int(pos / 10), _MAX(60, 10 * wordlen), 24);
				cvui::rect(cvframe, rect.x, rect.y, rect.width, rect.height, 0x808080);
				cvui::text(cvframe, rect.x + 10, rect.y + 5, it->second.tostring());

				if (cvui::mouse(cvui::CLICK) && rect.contains(cvui::mouse()))
				{
					if (!it->second.script.empty())
					{
						gcurcell = it->first;
						dolua(it->second.script);
					}
				}
				++it;
			}
		}

		for (auto it : cvimages)
		{
			cvui::image(cvframe, it.p.x, it.p.y, it.mat);
		}

		cv::imshow("Form1", cvframe);

		cvui::update();

		if (cv::waitKey(100) == 27)
			break;

		if (cv::getWindowProperty("Form1", 0) < 0)
		{
			break;
		}
	}

	PRINT("cvwindow quit");

	cvDestroyWindow("Form1");
	cvframe.release();
}
static int initui(lua_State* L)
{
	cvinitwindow();

	return 0;
}
static int showui(lua_State* L)
{
	cvwindow();

	return 0;
}
static int cvimg(lua_State* L)
{
	real x = lua_tonumber(L, 1);
	real y = lua_tonumber(L, 2);
	string filename = lua_tostring(L, 3);
	cv::Mat mat = cv::imread(filename);
	if (!mat.empty())
	{
		img_t img;
		img.mat = mat;
		img.p = point_t(x, y);
		cvimages.push_back(img);
	}
	return 0;
}
*/
//------------------------------------------
// DNA
//------------------------------------------
struct trigger_t
{
	int time;
	int pos;
	std::string funcode;
};
std::vector<trigger_t> triggerlist;
void testtrigger(lua_State* L, int pos)
{
	PRINT("testtrigger growtime=" << growtime << " pos=" << pos);
	for (auto i : triggerlist)
	{
		PRINT("testtrigger time=" << i.time << " pos=" << i.pos);

		if (growtime == i.time && pos == i.pos)
		{
			growpos = pos;
			luaL_dostring(L, i.funcode.c_str());
			PRINT("testtrigger " << i.funcode)
		}
	}
}
static int active(lua_State* L)
{
	std::string f = lua_tostring(L, 1);
	int t = lua_tonumber(L, 2);

	int s = -1;
	if (lua_gettop(L) == 3)
		s = lua_tonumber(L, 3);

	trigger_t tg;
	tg.pos = s;
	tg.time = t;
	tg.funcode = f;
	triggerlist.push_back(tg);

	PRINT("active: " << tg.funcode);
	return 0;
}
static int grow(lua_State* L)
{
	int n = 1;
	if (lua_gettop(L) == 1)
		n = lua_tonumber(L, 1);
	for (int i = 0; i < n; i++)
	{
		growtime++;

		testtrigger(L, -1);

		if (estack.empty())
			return 0;

		VECLIST& e = estack.back();
		for (int i = 0; i < e.size(); i++)
		{
			testtrigger(L, i);
		}
	}
	return 0;
}

//------------------------------------------
// fieldrender
//------------------------------------------
namespace POLY_FIELD_RENDER {
	extern void renderstack();
}
static int fieldrenderpoly(lua_State* L)
{
	Global::floatmap["D"] = lua_tonumber(L, 1);
	Global::floatmap["SCALE"] = lua_tonumber(L, 2);

	POLY_FIELD_RENDER::renderstack();
	return 0;
}

namespace CONNECT_FIELD_RENDER {
	extern void renderstack();
}
static int fieldrenderpoint(lua_State* L)
{
	Global::floatmap["D"] = lua_tonumber(L, 1);
	Global::floatmap["SCALE"] = lua_tonumber(L, 2);

	CONNECT_FIELD_RENDER::renderstack();
	return 0;
}
namespace PIPE_FLOW_FIELD_RENDER {
	extern void add_centerpoint(crvec p, float r, int line);
	extern void render_pipe(submesh& sm);
	extern void clear();
}
static int addct(lua_State* L)
{
	real x = lua_tonumber(L, 1);
	real y = lua_tonumber(L, 2);
	real z = lua_tonumber(L, 3);
	real r = lua_tonumber(L, 4);
	real ind = lua_tonumber(L, 5);
	PIPE_FLOW_FIELD_RENDER::add_centerpoint(vec(x, y, z), r, ind);
	return 0;
}
static int fieldrender(lua_State* L)
{
	PIPE_FLOW_FIELD_RENDER::render_pipe(SUBMESH);
	PIPE_FLOW_FIELD_RENDER::clear();
	return 0;
}

//------------------------------------------
// models
//------------------------------------------
static int starterrain(lua_State* L)
{
	real sz = lua_tonumber(L, 1);
	mountain(0, sz);
	return 0;
}

//------------------------------------------
// read/write models
//------------------------------------------
static int readobj(lua_State* L)
{
	std::string filename = lua_tostring(L, 1);
	real s = lua_tonumber(L, 2);
	loadobj(filename, s);
	return 0;
}
static int saveobj(lua_State* L)
{
	std::string filename = lua_tostring(L, 1);
	savesubmesh3DS(filename.c_str(), SUBMESH);
	return 0;
}

//------------------------------------------
static int dolua(lua_State* L)
{
	const std::string& luascript = lua_tostring(L, 1);
	//PRINT("dolua:" << luascript);

	/* run the script */
	if (luascript.find(".lua") != std::string::npos)
	{
		if (luaL_dofile(L, luascript.c_str()))
		{
			PRINT("Lua dofile err: " << (lua_tostring(L, -1)) << "\r\n");
		}
	}
	else
	{
		if (luaL_dostring(L, luascript.c_str()))
		{
			PRINT("Lua string err: " << (lua_tostring(L, -1)));
		}
	}
	return 0;
}

//------------------------------------------
// 测试
//------------------------------------------
extern void maintest();
static int maintest(lua_State* L)
{
	int n = lua_gettop(L);
	for (int i = 0; i < n; i++)
		luaparam[i] = lua_tonumber(L, 1 + i);

	//maintest();
	return 0;
}
extern void shape();
static int shape(lua_State* L)
{
	int n = lua_gettop(L);
	for (int i = 0; i < n; i++)
		luaparam[i] = lua_tonumber(L, 1 + i);

	shape();
	return 0;
}
extern void test();
static int test(lua_State* L)
{
	int n = lua_gettop(L);
	for (int i = 0; i < n; i++)
		luaparam[i] = lua_tonumber(L, 1 + i);

	test();
	return 0;
}
extern void do_cmdstring();
static int realphg(lua_State* L)
{
	do_cmdstring();

	return 0;
}
extern void flow();
static int flow(lua_State* L)
{
	int n = lua_gettop(L);
	for (int i = 0; i < n; i++)
		luaparam[i] = lua_tonumber(L, 1 + i);

	flow();
	return 0;
}

//extern void scene();
static int scene(lua_State* L)
{
	int n = lua_gettop(L);
	for (int i = 0; i < n; i++)
		luaparam[i] = lua_tonumber(L, 1 + i);

	//scene();
	return 0;
}
extern void scivis();
static int scivis(lua_State* L)
{
	int n = lua_gettop(L);
	for (int i = 0; i < n; i++)
		luaparam[i] = lua_tonumber(L, 1 + i);

	scivis();
	return 0;
}
static int mach(lua_State* L)
{
	int n = lua_gettop(L);
	for (int i = 0; i < n; i++)
		luaparam[i] = lua_tonumber(L, 1 + i);

	//mach();
	return 0;
}
static int geom(lua_State* L)
{
	int n = lua_gettop(L);
	for (int i = 0; i < n; i++)
		luaparam[i] = lua_tonumber(L, 1 + i);

	geom();
	return 0;
}
extern void deform();
static int deform(lua_State* L)
{
	int n = lua_gettop(L);
	for (int i = 0; i < n; i++)
		luaparam[i] = lua_tonumber(L, 1 + i);

	deform();
	return 0;
}
//------------------------------------------
// scene managerment
//------------------------------------------
static int createent(lua_State* L)
{
	entity_t e;
	e.id = lua_tonumber(L, 1);
	e.luascript = lua_tostring(L, 2);
	e.scale = 10;
	if (lua_gettop(L) >= 3)
		e.scale = lua_tonumber(L, 3);
	if (lua_gettop(L) >= 6)
	{
		real x = lua_tonumber(L, 4);
		real y = lua_tonumber(L, 5);
		real z = lua_tonumber(L, 6);
		e.pos = vec(x, y, z);
	}
	if (lua_gettop(L) >= 9)
	{
		real x = lua_tonumber(L, 7);
		real y = lua_tonumber(L, 8);
		real z = lua_tonumber(L, 9);

		e.dir = vec(x, y, z);
	}
	entlist.push_back(e);
	return 0;
}
/*
//------------------------------------------
// form ui
//------------------------------------------
const pnt CVCENTER(1280 / 2, 800 / 2);
const real CVSCALE = 10.0f;
static int button(lua_State* L)
{
	std::string str = lua_tostring(L, 1);
	int x = lua_tonumber(L, 2);
	int y = lua_tonumber(L, 3);

	ASSERT(!cvframe.empty())

	if (cvui::button(cvframe, x, y, str))
	{
		if (lua_gettop(L) >= 4)
		{
			std::string action = lua_tostring(L, 4);
			if(!action.empty())
				dolua(action);
		}
	}
	return 0;
}
//------------------------------------------
int thresholds[10] = { 0 };
static void on_trackbar(int, void*)
{
	PRINT("on_trackbar");
}
static int trackbar(lua_State* L)
{
	//PRINT("trackbar");
	std::string str1 = lua_tostring(L, 1);
	int id = lua_tonumber(L, 2);
	int count = lua_tonumber(L, 3);
	cv::createTrackbar(str1, "Form1", &thresholds[id], count, on_trackbar);
	return 0;
}

//------------------------------------------
// CUT SUBMESH
//------------------------------------------
namespace AUTOCVPOINT
{
	int screensz = 500; // 500
	cv::Point screenct = cv::Point(1280 * 0.25, 800 * 0.25); // 1280 * 0.5, 800 * 0.5

	vec2 origin;
	real scale = 1.0f;

	void automatch(const vec2& aa, const vec2& bb)
	{
		screensz = LUA_FLOAT(SCREENSZ, 500);
		screenct = cv::Point(LUA_FLOAT(SCREENX, 1280 * 0.25), LUA_FLOAT(SCREENY, 800 * 0.25));

		origin = (aa + bb) / 2;
		real sz = (bb - aa).len();
		scale = screensz / sz;
	}
	void cvpoint(crvec2 p, int index, int r)
	{
		cv::Point pos = screenct + cv::Point((p.x - origin.x) * scale, (p.y - origin.y) * scale);
		cvui::text(cvframe, pos.x, pos.y, std::to_string(index), 0.5, 0xFF00FF00);

		cv::circle(cvframe,
			pos, 1,
			cv::Scalar(255, 255, 255), r);
	}
}
static int cvgradnum(lua_State* L)
{
	gradnum = lua_tonumber(L, 1);
	return 0;
}
static int cvcircle(lua_State* L)
{
	int x = lua_tonumber(L, 1);
	int y = lua_tonumber(L, 2);
	int r = lua_tonumber(L, 3);

	ASSERT(!cvframe.empty())

	cv::circle(cvframe,
		cv::Point(x, y), r,
		cv::Scalar(255, 255, 255), 1);
	return 0;
}
static int cvpset(lua_State* L)
{
	int x = lua_tonumber(L, 1);
	int y = lua_tonumber(L, 2);
	int sz = 4;
	if (lua_gettop(L) >= 3)
		sz = lua_tonumber(L, 3);
	ASSERT(!cvframe.empty())

	cv::circle(cvframe,
		cv::Point(x, y), 1,
		cv::Scalar(255, 255, 255), sz);
	return 0;
}
static int cvline(lua_State* L)
{
	int x1 = lua_tonumber(L, 1);
	int y1 = lua_tonumber(L, 2);
	int x2 = lua_tonumber(L, 3);
	int y2 = lua_tonumber(L, 4);

	ASSERT(!cvframe.empty())

	cv::line(cvframe,
		cv::Point(x1, y1), cv::Point(x2, y2),
		cv::Scalar(255, 255, 255), 1);
	return 0;
}
*/
//------------------------------------------
// API
//------------------------------------------
//LUA_FUN_BEGIN_NOPPRAM(_jsonauto)
//std::string str = lua_tostring(L, 1);
////PRINTV(str);
//JSONAUTO::loadjson(str);
//LUA_FUN_END
//
//LUA_FUN_BEGIN(_jsonautotest)
//JSONAUTO::test();
//LUA_FUN_END


/*
inline void cvline2di(const pnt& p1, const pnt& p2, int size, int cor = 0xFFFFFFFF)
{
	cv::line(cvframe,
		cv::Point(p1.x, p1.y), cv::Point(p2.x, p2.y),
		cv::Scalar(255, 255, 255), 1);
}
bool cvdrawsubmesh2d(const submesh& sm, int view, real scale, const vec2& o)
{
	PRINT("cvdrawsubmesh2d " << sm.tris.size())
	for (int i = 0; i < sm.tris.size(); i++)
	{
		int t1, t2, t3;
		{
			t1 = sm.tris[i].vertexIndex[0];
			t2 = sm.tris[i].vertexIndex[1];
			t3 = sm.tris[i].vertexIndex[2];
		}

		const vertex& p1 = sm.vertices[t1];
		const vertex& p2 = sm.vertices[t2];
		const vertex& p3 = sm.vertices[t3];
		if (view == 0)
		{
			pnt pp1 = pnt::fromvec((p1.p.xy() * scale + o));
			pnt pp2 = pnt::fromvec((p2.p.xy() * scale + o));
			pnt pp3 = pnt::fromvec((p3.p.xy() * scale + o));
			cvline2di(pp1, pp2, 1, color);
			cvline2di(pp2, pp3, 1, color);
			cvline2di(pp1, pp3, 1, color);
		}
		else if (view == 1)
		{
			pnt pp1 = pnt::fromvec((p1.p.xz() * scale + o));
			pnt pp2 = pnt::fromvec((p2.p.xz() * scale + o));
			pnt pp3 = pnt::fromvec((p3.p.xz() * scale + o));
			cvline2di(pp1, pp2, 1, color);
			cvline2di(pp2, pp3, 1, color);
			cvline2di(pp1, pp3, 1, color);
		}
		else if (view == 2)
		{
			pnt pp1 = pnt::fromvec((p1.p.yz() * scale + o));
			pnt pp2 = pnt::fromvec((p2.p.yz() * scale + o));
			pnt pp3 = pnt::fromvec((p3.p.yz() * scale + o));
			cvline2di(pp1, pp2, 1, color);
			cvline2di(pp2, pp3, 1, color);
			cvline2di(pp1, pp3, 1, color);
		}
	}
	return false;
}
*/
//LUA_FUN_BEGIN(_cvdrawsm)
//	cvdrawsubmesh2d(*gsubmesh, luaparam[0], luaparam[1], vec2(luaparam[2], luaparam[3]));
//LUA_FUN_END

LUA_FUN_BEGIN(_loadsectormap)
WAYPOINT::loadsectormap();
LUA_FUN_END

//------------------------------------------
void registerapi(lua_State* L)
{
	//LUA_REGIST(_jsonauto);
	//LUA_REGIST(_jsonautotest);

//	LUA_REGIST(_cvdrawsm);

	//MESHBOOL::main();
}
//------------------------------------------
void reset()
{// RESET
	gCurPos = vec::ZERO;
	gUpperDir = vec::UY;
	estack.clear();

	dirstack.clear();
	//verstack.clear();
	coordstack.clear();
	weightlist.clear();

	gwtimes = 1;
	binvnorm = 0;
	gcommonvertex = 0;
	gsearchcomvertex = 0;

	growtime = 0;
	growpos = -1;
	bclosed = true;

	memset(luaparam, 0, sizeof(luaparam));
}

//------------------------------------------
// init LUA
//------------------------------------------
//scope ScePHG {
	scope prim_uv_projector {
		extern void LUA_PRIM_UV_API(lua_State*);
	}
//}
scope CURVE_FIT {
	extern void LUA_FIT_API(lua_State*);
}

lua_State* L = 0;
void intlua()
{
	PRINT("init lua");
	reset();

	DEF_SCALES(1); CAM_POS = vec::CENTER;
	/*-------- initialize Lua --------*/
	L = luaL_newstate();

	/* load Lua base libraries */
	luaL_openlibs(L);
	lua_register(L, "rnd", rnd);
	lua_register(L, "bnd", bnd);
	lua_register(L, "atan2", atan2);
	lua_register(L, "pow", pow);

	lua_register(L, "color", colori);
	lua_register(L, "rgb", rgb);
	lua_register(L, "hsl", hsl);
	lua_register(L, "psz", pixsize);
	lua_register(L, "mod", rendermod);

	lua_register(L, "dolua", dolua);

	// message
	lua_register(L, "prt", print);
	lua_register(L, "msg", msgbox);
	lua_register(L, "msgbox", msgbox);
	lua_register(L, "getchar", getchar);

	// param
	lua_register(L, "param", setparam);
	lua_register(L, "setfloat", setfloatmap);
	lua_register(L, "getfloat", getfloatmap);
	lua_register(L, "setint", setintmap);
	lua_register(L, "setstring", setstringmap);
	lua_register(L, "setvec3", setvec3map);
	lua_register(L, "getvec3", getvec3map);
	lua_register(L, "movepointn", movepointn);
	lua_register(L, "lambda", cclambda);
	
	// enity
	lua_register(L, "ent", createent);

	// model
	lua_register(L, "readobj", readobj);
	lua_register(L, "saveobj", saveobj);

	//------------------------------------------
	// 表格脚本
	//------------------------------------------
	lua_register(L, "cell", celli);
	lua_register(L, "celli", celli);
	lua_register(L, "cells", cells);
	lua_register(L, "cellscript", cellscript);
	lua_register(L, "curcell", curcell);

	//------------------------------------------
	// 绘制
	//------------------------------------------
	lua_register(L, "pix", pixel);
	lua_register(L, "pixi", pixeli);
	lua_register(L, "pst", pset);

	lua_register(L, "tri", triang);

	lua_register(L, "pln", plane);
	lua_register(L, "coorddummy", coorddummy);
	lua_register(L, "pyramid", pyramid);
	lua_register(L, "sphere", sphere);
	lua_register(L, "cube", cube);

	lua_register(L, "rayrender", rayrender);
	lua_register(L, "scanlinerender", scanlinerender);

	//------------------------------------------
	// 多边形
	//------------------------------------------
	lua_register(L, "ver", addver);
	lua_register(L, "trii", triangi);

	LUA_POLY_API(L);

	prim_uv_projector::LUA_PRIM_UV_API(L);
	CURVE_FIT::LUA_FIT_API(L);

	//------------------------------------------
	// 植物贝壳形态学
	//------------------------------------------

	/* 系统 */

	lua_register(L, "newe", newedge);
	lua_register(L, "push", pushe);
	lua_register(L, "pop", pope);

	lua_register(L, "pushc", pushcoord);
	lua_register(L, "popc", popcoord);
	lua_register(L, "coord", calccoord);
	lua_register(L, "uz", coorduz);
	lua_register(L, "ux", coordux);
	lua_register(L, "mor", mirror);

	lua_register(L, "pushd", setdir);
	lua_register(L, "popd", popdir);

	lua_register(L, "comv", commonvert);
	lua_register(L, "scomv", searchcomvert);
	lua_register(L, "inv", invnorm);


	/* 构造 */

	lua_register(L, "round", roundedge);
	lua_register(L, "arc", arcedge);
	//lua_register(L, "edge", edge);

	/* 权重 */

	lua_register(L, "wgt", weight);
	lua_register(L, "wbnd", weightblend);
	lua_register(L, "wdiv", weightdiv);
	lua_register(L, "wbndx", weightblendx);
	lua_register(L, "wbndz", weightblendz);
	lua_register(L, "wbndm", weightblendmirror);
	lua_register(L, "wtim", wtimes);

	/* 变换 */

	lua_register(L, "ext", extrudeedge);
	lua_register(L, "ext2", extrudeedgeex);
	lua_register(L, "mov", moveedge);
	lua_register(L, "mov2", moveedgeex);

	lua_register(L, "scl", scaleedge);
	lua_register(L, "sclm", scaleedgemirror);
	lua_register(L, "scl2", scaleeedge2);

	lua_register(L, "rot", rotedge);
	lua_register(L, "rotm", rotedgemirror);
	lua_register(L, "rote", rotonedge);
	lua_register(L, "yaw", yawedge);
	lua_register(L, "pit", pitchedge);
	lua_register(L, "rol", rolledge);

	lua_register(L, "radi", radedge);

	/* 拓扑 */

	lua_register(L, "div", div);
	lua_register(L, "cls", closeedge);
	lua_register(L, "sub", subedge);
	lua_register(L, "link", linkedge);
	lua_register(L, "union", unionedge);
	lua_register(L, "double", doublevnum);
	lua_register(L, "pos", setpos);

	/* 绘制 */

	lua_register(L, "face", face);
	lua_register(L, "faceo", faceo);

	lua_register(L, "clearsm", clearsm);
	lua_register(L, "clearsk", clearstack);

	//------------------------------------------
	// 场造型
	//------------------------------------------
	lua_register(L, "terrain", starterrain); // 山体高度场
	lua_register(L, "fieldrenderpoly", fieldrenderpoly);
	lua_register(L, "fieldrenderpoint", fieldrenderpoint);

	lua_register(L, "addct", addct);
	lua_register(L, "fieldrender", fieldrender);

	//------------------------------------------
	// 测试
	//------------------------------------------
	lua_register(L, "main", maintest);
	lua_register(L, "shape", shape);
	lua_register(L, "scivis", scivis);
	lua_register(L, "mach", mach);
	lua_register(L, "geom", geom);
	lua_register(L, "deform", deform);
	lua_register(L, "test", test);
	lua_register(L, "flow", flow);
	lua_register(L, "scene", scene);
	lua_register(L, "realphg", realphg);

	//------------------------------------------
	// cv ui
	//------------------------------------------
	/*lua_register(L, "img", cvimg);

	lua_register(L, "cvgrad", cvgradnum);

	lua_register(L, "cvp", cvpset);
	lua_register(L, "cvl", cvline);
	lua_register(L, "cvcircle", cvcircle);

	lua_register(L, "initui", initui);
	lua_register(L, "showui", showui);

	lua_register(L, "button", button);
	lua_register(L, "trackbar", trackbar);*/

	//------------------------------------------
	// API
	//------------------------------------------
	registerapi(L);

	/* cleanup Lua */
	//lua_close(L);
}

// --------------------------------------------
// call lua
// --------------------------------------------
void _push(real v)
{
	lua_pushnumber(L, v);
}
void _push(const std::string& str)
{
	lua_pushstring(L, str.c_str());
}

void push() {}

template<typename T>
void push(T&& arg) {
	_push(std::forward<T>(arg));
}

template<typename T, typename ... Args>
void push(T&& arg, Args&& ... other) {
	_push(std::forward<T>(arg));
	push(std::forward<Args>(other)...);
}

void get(real& v)
{
	v = lua_tonumber(L, -1);
}
void get(std::string& str)
{
	str = lua_tostring(L, -1);
}

template<typename ... Args, typename T>
void fun0(const char* fun, T& ret, Args&& ... args)
{
	int top = lua_gettop(L);

	lua_getglobal(L, fun);

	while (true)
	{
		if (!lua_isfunction(L, -1)) {

			ERRORMSG("Lua fun err: not find function: " << fun);
			break;
		}

		const int params = sizeof...(args);
		push(std::forward<Args>(args)...);

		if (lua_pcall(L, params, 1, 0))
		{
			ERRORMSG("Lua fun err: " << (lua_tostring(L, -1)));
			break;
		}

		get(ret);

		lua_settop(L, top);

		break;
	}
}
void fun(const char* fun, const std::string& paramstr, std::string& ret)
{
	int top = lua_gettop(L);

	lua_getglobal(L, fun);

	while (true)
	{
		if (!lua_isfunction(L, -1)) {

			ERRORMSG("Lua fun err: not find function: " << fun);
			break;
		}
		int params = 0;
		size_t lstpos = 0;
		while (lstpos < paramstr.length())
		{
			size_t pos = paramstr.find(" ", lstpos);
			if (pos == std::string::npos)
			{
				pos = paramstr.length();
			}
			std::string str = paramstr.substr(lstpos, pos - lstpos);
			if (str.empty())
				break;

			char c = str.c_str()[0];
			if (c >= '0' && c <= '9')
			{// 数字
				_push(atoi(str.c_str()));
				params++;
			}
			else
			{// 字符串
				_push(str.c_str());
				params++;
			}

			lstpos = pos + 1;
		}

		if (lua_pcall(L, params, 1, 0))
		{
			ERRORMSG("Lua fun err: " << (lua_tostring(L, -1)));
			break;
		}

		ret = lua_tostring(L, -1);
		break;
	}

	lua_settop(L, top);
}

void registapi(crstr funname, lua_hdr_t hdr)
{
	if (!L)
	{
		intlua();
	}

	PRINT("registapi:" << funname);

	luahdrmap[funname] = hdr;

	lua_register(L, "api", api);
}
bool dolua(const std::string& luascript)
{
	PRINT("dolua : {" << luascript << " ...");
	stringstream ss; ss << "}done! (";
	int start = ::GetTickCount();
	if (!L)
	{
		intlua();
	}
	if (luascript.find(".lua") != std::string::npos)
	{
		if (0 != luaL_dofile(L, luascript.c_str()))
		{
			ERRORMSG("Lua fun err: " << (lua_tostring(L, -1)));
			return false;
		}
	}
	else
	{
		if (luaL_dostring(L, luascript.c_str()))
		{
			ERRORMSG("Lua fun err: " << (lua_tostring(L, -1)));
			return false;
		}
	}
	ss << ::GetTickCount() - start << "ms)";
	printf(ss.str().c_str()); ::OutputDebugStringA(ss.str().c_str());

	return true;
}
