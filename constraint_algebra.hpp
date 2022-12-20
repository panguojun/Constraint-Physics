/*
*								约束代数
*						
*						【特征向量空间】是最核心的理念
*						分工：
*						PHG负责代数化公式部分
*						LUA负责复杂性部分
*						CC负责基础实现以及数值计算部分
*						约束跟人工智能领域很相似
*/
namespace constraint_algebra
{
	enum{
		CONSTRAINT = 4, // 约束
		VECTOR = 5,		// 向量	
		CST_Q = 5		// 约束节点
	};
	// -----------------------------
	// 约束（lua实现的）
	// V2 = V1/C*C`*C
	// -----------------------------
#define CST	lua_cst	
#define VEC	string
#define FUN_NAME string
	struct lua_cst
	{
		string name;
		string cs; // constaint name in lua
		string cd; // coordnate name
		string cdtrans = "nil"; // 坐标系本身的变换

		// selection force
		FUN_NAME d_f;
		vector<string> d_params;
		string d_force;

		// 由于存在导数概念，这里记录阶数
		int order = 1; 

		// 把向量投影到约束空间内 V/C
		void prj_vec(VEC& ov, const VEC& v)
		{
			dolua(ov + "= world_cst(\'" + name + "\'," + v + "," + cs + "," + cd + ");");
		}
		// 约束的导数/梯度，derivative
		void der_cst(const CST& c)
		{
			(*this) = c;
			order = c.order+1;
		}
		// 约束乘法
		void mul_vec(const VEC& ov, const VEC& v) const
		{
			if (order == 1)// 把向量还原回世界空间 V * C
				dolua(ov + "= cst_world(" + v + "," + cs + "," + cd + ")");
			else if(order == 2)// 移动向量 V * C`,在全局下无法线性合并，故而需要使用MOVE函数
				dolua(ov + "= move(" + v + "," + cs + "," + cd + "," + cdtrans + "," + d_f + "," + d_params.front() + ")");
		}
	};
	std::map<string, lua_cst> cstmap;
	// -----------------------------
	// setup
	// -----------------------------
	string tostring(const vector<string>& params)
	{
		string ret;
		for (auto& s : params)
		{
			if (!ret.empty())
				ret += ",";
			ret += s;
		}
		return ret;
	}
	bool get_fun(string& name, vector<string>& params, code& cd)
	{
		ASSERT(params.empty());
		while (!cd.eoc())
		{
			auto c = cd.cur();
			if (c == '('){
				params.emplace_back();
			}
			else if (c == '\'') {
				while(true) {
					cd.next();
					if (cd.cur() == '\'') break;
					params.back() += cd.cur();
				} 				
				
				PRINTV(params.back())
				cd.next();
				continue;
			}
			else if (c == ','){
				//PRINTV(params.back())
				params.emplace_back();
			}
			else if (c == ')'){
				ASSERT(isname(name.front()));
			//	PRINT(name << "(" << (params.empty() ? "" : tostring(params)) << ")");
				return true;
			}
			else{
				if(params.empty())
					name += c;
				else
					params.back() += c;
			}
			cd.next();
		}
		return false;
	}
	bool get_force(string& force, code& cd)
	{
		while (!cd.eoc())
		{
			auto c = cd.cur();
			if (c == '-'){
				ASSERT('>' == cd.next()); 
				cd.next();
				{// -> force to move
					int type = get(cd);
					getval(cd, type);
					force = cd.strstack.back();
				}
				return true;
			}
			cd.next();
		}
		return false;
	}
	void setup(NODE* tree)
	{
		KV_IT;
		var v;
		
		// vector
		KEY_VAL("v"){
			v.type = VECTOR;
			v.sval = it->second;
		}
		// constraint
		CHEKCK_KEY("cs")
		{
			lua_cst c;
			KEY_VAL("cs")
			{
				c.name = tree->name;
				c.cs = it->second;
			}
			KEY_VAL("gui")
			{
				// 关联CS gui
				dolua(c.cs + ".gui = " + it->second);
			}
			KEY_VAL("cd")
			{
				vector<string> elem;
				STR::split(it->second, elem, ",");
				c.cd = elem.front();
				if(elem.size() > 1)
					c.cdtrans = elem.back();
				// 关联CS CD
				dolua(c.cs + ".cd = " + c.cd);
			}
			KEY_VAL("f")
			{
				code cd(it->second.c_str());
				get_fun(c.d_f, c.d_params, cd);
				get_force(c.d_force, cd);
				v.type = CONSTRAINT;
				v.sval = tree->name;

				// 关联CS target
				dolua(c.cs + ".target = " + c.d_params.front());
				
			}
			cstmap[tree->name] = c;
		}
		ADD_VAR(tree->name.c_str(), v);

		// children
		for (auto it : tree->children) {
			setup(it.second);
		}
	}
	// 自定义运算符
	bool iscalc_func(opr c)
	{
		return iscalc0(c) || c == '->';
	}
	uchar rank_opr_func(opr c) { 
		if(c == '->')
			return rank['*'];
		return rank[(char)c];
	}
	void subtree_func(const char* pchar, var& v)
	{
		string str(pchar);
		if (str.find("<") != string::npos) {
			string expr = str.substr(str.find("<") + 1, str.find(">") - 1);
			v.type = CST_Q;
			v.sval = expr;
		}
	}
	// --------------------------------------
	_API(dumpv)
	{
		var& a = GET_PARAM(1);
		dolua("dumpv(" + a.sval + ")");
		return 0;
	}
	// --------------------------------------
	void REG_APIs()
	{
		// 自定义运算符
		iscalc = iscalc_func;
		rank_opr = rank_opr_func;
		subtree = subtree_func;

		CALC([](code& cd, opr o, int args)->var {
			if (o == '`')
			{
				var c;
				c.type = CONSTRAINT;
				var& a = PHG_VALSTACK(1);
				PRINT("(`)" << a.sval);
				lua_cst cst;
				cst.der_cst(cstmap[a.sval]);
				cst.name = a.sval + "`";
				c.sval = cst.name;

				cstmap[cst.name] = cst;
				PHG_VALPOP(1);
				return c;
			}
			if (o == '->')
			{
				var c;
				var& b = PHG_VALSTACK(1);
				var& a = PHG_VALSTACK(2);
				PRINT("(->)" << a.sval << "," << b.sval);
				PHG_VALPOP(2);
				return c;
			}
			if (o == '*')
			{
				ASSERT_RET(ROOT);
				var& b = PHG_VALSTACK(1);
				var& a = PHG_VALSTACK(2);
				PHG_PRINT("(*)" << a.sval << "," << b.sval);
				var c; 
				if (a.type == VECTOR && b.type == CONSTRAINT)
				{
					c.type = VECTOR;
					c.sval = a.sval + "_1";
					cstmap[b.sval].mul_vec(c.sval, a.sval);
				}
				else if(a.type == VECTOR && b.type == CST_Q)
				{// V*<C1,C2,C3>
					PRINT("VECTOR * CST_Q")
					string vname = a.sval + "_1";
					c.type = VECTOR;
					c.sval = vname;
					vector<string> elem;
					STR::split(b.sval, elem, ",");
					for (auto& bb : elem)
					{
						cstmap[bb].prj_vec(vname, a.sval);
						{
							lua_cst cst;
							cst.der_cst(cstmap[bb]);
							cst.name = a.sval + "`";
							cstmap[cst.name] = cst;
							cst.mul_vec(vname, vname);
						}
						cstmap[bb].mul_vec(vname, vname);
						a.sval = vname;
					}
				}
				PHG_VALPOP(2);
				return c;
			}
			if (o == '/')
			{
				ASSERT_RET(ROOT);
				var& b = PHG_VALSTACK(1);
				var& a = PHG_VALSTACK(2);
				PHG_PRINT("(/)" << a.sval << "," << b.sval);
				var c;
				if (a.type == VECTOR && b.type == CONSTRAINT)
				{
					c.type = VECTOR;
					c.sval = a.sval + "_1";
					cstmap[b.sval].prj_vec(c.sval, a.sval);
				}
				PHG_VALPOP(2);
				return c;
			}
			return 0;
		});

		// API
		_REG_API(dumpv, dumpv);
	}
}