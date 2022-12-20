/*********************************************************************
*						��λ / ��λ����ϵ						
*				��һ�ֶ�����ʱ��ά���ϵ�����ϵ
*				������ϵ�������Ƶ���ʽ
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
	// ������ϵ�¶���һ������
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