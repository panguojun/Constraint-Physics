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
	// ������ϵ�¶���һ������
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