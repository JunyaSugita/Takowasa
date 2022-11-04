#include "Bezier.h"

Vector3 Bezier(Vector3 start, Vector3 end, float time, Vector3 p1)
{
	Vector3 p3 = lerp(start, p1, time);
	Vector3 p4 = lerp(p1, end, time);

	Vector3 nowPos = lerp(p3, p4, time);

	return nowPos;
}
