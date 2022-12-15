/*********************************************************************
*						相位 / 相位坐标系						
*				是一种定义在时间维度上的坐标系
*				跟坐标系有着类似的形式
*/

// *******************************************************************
//  th
// C    Phase
// *******************************************************************
struct phase
{
	static const phase ONE;
	number_math::complex cxn;

	phase() {}
	phase(const phase& c)
	{
	}
	phase(real ang)
	{
	}
	void rot(real ang)
	{
	}
	phase operator + (const phase& c) const
	{
		phase rc;
		return rc;
	}
	phase operator - (const phase& c) const
	{
		phase rc;
		return rc;
	}
	// 在坐标系下定义一个向量
	friend vec2 operator * (crvec2 p, const phase& c)
	{
		return p;
	}
	phase operator * (const phase& c) const
	{
		phase rc;
		return rc;
	}
	friend vec2 operator / (crvec2 p, const phase& c)
	{
		return p;
	}
	phase operator / (const phase& c) const
	{
		phase rc;
		return rc;
	}
	void reverse()
	{
		(*this) = ONE / (*this);
	}
	phase reversed() const
	{
		return ONE / (*this);
	}
	static phase gradcoord(const phase& c1, const phase& c2)
	{
		return c1.reversed() * c2;
	}
	real dot(crvec2 v) const
	{
		return 0;
	}
	void dump() const
	{
	}
};
const phase phase::ONE = phase();