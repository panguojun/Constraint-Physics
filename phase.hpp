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
	real angle;

	phase() {}
	phase(real _angle)
	{
		angle = _angle;
	}
	void rot(real _angle)
	{
		angle = _angle;
	}
	phase operator + (const phase& c) const
	{
		phase rc;
		rc.angle = angle + c.angle;
		return rc;
	}
	phase operator - (const phase& c) const
	{
		phase rc;
		rc.angle = angle - c.angle;
		return rc;
	}
	// 在坐标系下定义一个向量
	friend vec2 operator * (crvec2 p, const phase& c)
	{
		number_math::complex cx(1, c.angle);
		return p * cx.x;
	}
	phase operator * (const phase& c) const
	{
		phase rc;
		rc.angle = angle + c.angle;
		return rc;
	}
	friend vec2 operator / (crvec2 p, const phase& c)
	{
		number_math::complex cx(1, -c.angle);
		return p * cx.x;
	}
	phase operator / (const phase& c) const
	{
		phase rc;
		rc.angle = angle - c.angle;
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
	void dump() const
	{
		PRINTV(angle);
	}
};
const phase phase::ONE = phase();