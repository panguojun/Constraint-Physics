/* *****************************************************************
						寻路算法
						包括ASTAR算法
			一些寻路算法可能可以模拟人工智能？
			寻路算法可能跟物理的作用量，路径积分有联系
			 
// *****************************************************************/
struct ASTAR
{
	static const int SIZEX = IMAGESCALE;
	static const int SIZEY = IMAGESCALE;

	const int MAX_RAD = 150;		// 最大辐射阈值
	real blendfactor = 0.5;		// 场与距离H函数的混合值

	// nearest path
	using mov_t = std::pair<char, char>;
	struct node_t
	{
		int parent = -1;
		pointn p;
		real g_field;
		real g;
		real h_field;
		real h;
	};

//	PNG* png = 0, * field_png = 0;
	std::vector< node_t> openlist;
	std::vector< node_t> closelist;
	node_t work_node;
	std::vector< pointn> bestway;

	std::map<int, char> hismap;
	char gethismap(const pointn& p)
	{
		return hismap[p.toindex(SIZEX)];
	}
	void sethismap(const pointn& p, char v)
	{
		hismap[p.toindex(SIZEX)] = v;
	}

	// -----------------------------------------------------------------
	// 碰撞与辐射场测试
	// -----------------------------------------------------------------
	bool(*ptestcolor)(const pointn& p);
	bool(*ptestfieldcolor)(const pointn& p);
	real(*pgetfield)(const pointn& p);

	static bool testcolor(const pointn& p)
	{
		return 0;// png&& png->getcolor(x, y) == 0;
	}
	static bool testfieldcolor(const pointn& p)
	{
		return 0;// field_png&& field_png->getcolor(x, y) > MAX_RAD;
	}
	static real getfield(const pointn& p)
	{
		//if (!field_png)
			return 0;
		//return 0.1 * field_png->getcolor(x, y);
	}
	// -----------------------------------------------------------------
	inline real getdis(const pointn& p, const pointn& B) const
	{
		return (p-B).len();
	}
	inline real getH_field(const pointn& p, const pointn& B) const
	{
		return 0;
	}
	inline real getH(const pointn& p, const pointn& B)
	{
		real dis = getdis(p, B);
		return dis;
	}
	inline real sumH(real H_field, real H)
	{
		//return H_field;
		return blend(H, H_field, blendfactor);
		//return sqrt(H_field * H_field + H * H);
	}
	inline real sumG(real G_field, real G)
	{
		//return G_field;
		return blend(G, G_field, blendfactor);
		//return sqrt(G_field * G_field + G * G);
	}
	int findmin()
	{
		int minpos = -1;
		real min = 1e5;
		//PRINT("openlist=" << openlist.size());
		for (int i = 0; i < openlist.size(); i++)
		{
			const node_t& nd = openlist[i];
			real sum = sumH(nd.h_field, nd.h) + sumG(nd.g_field, nd.g);
			//PRINT("sum=" << sum);
			if (sum < min)
			{
				min = sum;
				minpos = i;
			}
		}
		return minpos;
	}
	vector<pointn> movedirs =
	{
		pointn(0, 1),
		pointn(0, -1),
		pointn(1, 0),
		pointn(-1, 0),

		pointn(-1, -1),
		pointn(-1, 1),
		pointn(1, -1),
		pointn(1, 1)
	};
	bool path(node_t& node, const pointn& B, int depth)
	{
		work_node.g = -1;

		if (depth > 500)
		{
			PRINT("depth!");
			work_node = node;
			return false;
		}

		//PSET(node.p[0], node.p[1], 1, 0xFFFFFFFF);
		//PRINTV(node.p[0])PRINTV(node.p[1])

		if (getdis(node.p, B) < 1)
		{
			PRINT("GOAL!");
			return true;
		}

		for (auto it : movedirs)
		{
			static pointn np;
			np= node.p + it;
			if (!np.check_inside(0, SIZEX))
			{
				continue;
			}
			{
				if (gethismap(np) != 0)
					continue;
				sethismap(np,1);
			}
			if ((*ptestcolor)(np) || (*ptestfieldcolor)(np))
			{
				PRINT("HIT!");
				continue;
			}

			static node_t n;
			{
				n.p = np;
				n.g = node.g + it.len();
				n.g_field = node.g_field + (*pgetfield)(np);
				n.h = getH(n.p, B);
				n.h_field = getH_field(np, B);

				n.parent = closelist.size() - 1;
				
				//pointi(n.p.x, n.p.y, 1, 0xFFFFFFFF);
				openlist.push_back(n);
			}
		}
		int bestind = findmin();
		if (bestind == -1)
		{
			PRINT("findpath failed！bestind=-1 depth=" << depth << " openlist.size()=" << openlist.size());
			closelist.clear();
			return false;
		}
		else
		{
			static node_t best; best = openlist[bestind];
			//pointi(best.p.x, best.p.y, 1, 0xFF808080);
			PSET(best.p[0], best.p[1], 1, 0xFFFFFFFF);

			openlist.erase(openlist.begin() + bestind);
			closelist.push_back(best);

			return path(best, B, depth + 1); // 尾部优化
		}
	}

	void findpath(const pointn& a, const pointn& b)
	{
		PRINT("-----------findpath------------");
		hismap.clear();
		node_t A;
		A.p = a;
		A.g = 0;
		A.g_field = 0;
		A.h = getH(A.p, b);
		A.h_field = getH_field(A.p, b);
		//point(A.p, 4, 0xFF00FF00);

		node_t B;
		B.p = b;
		//point(B.p, 4, 0xFF00FFFF);

		openlist.clear();
		closelist.clear();

		openlist.push_back(A);
		work_node = A;
		if (path(work_node, B.p, 0))
		{
			if (!closelist.empty())
			{
				int it = closelist.size() - 1;
				while (closelist[it].parent != -1)
				{
					//pointi(closelist[it].p.x, closelist[it].p.y, 1, 0xFF00ff00);
					bestway.push_back(closelist[it].p);
					it = closelist[it].parent;
				}
			}
		}
	}
	bool init()
	{
		/*if (!png)
		{
			png = new PNG();
			if (!(*png).load("C:\\Users\\18858\\Documents\\LAB\\ZEXE/test.png"))
			{
				PRINT("!LoadFromFileA(C:\\Users\\18858\\Documents\\LAB\\ZEXE/test.png)");
				return false;
			}
		}*/
		/*
		if (!field_png)
		{
			field_png = new PNG();
			if (!(*field_png).load("C:\\Users\\18858\\Documents\\LAB\\ZEXE/field.png"))
			{
				PRINT("!LoadFromFileA(C:\\Users\\18858\\Documents\\LAB\\ZEXE/field.png)");
				return false;
			}
		}*/

		ptestcolor = testcolor;
		ptestfieldcolor = testfieldcolor;
		pgetfield = getfield;

		bestway.clear();

		return true;
	}
	void test()
	{
		if (!init())
		{
			return;
		}
		findpath(pointn(62, 52), pointn(152, 119));
	}
};