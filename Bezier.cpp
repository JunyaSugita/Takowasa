#include "Bezier.h"

//•âŠ®ŠÖ”
const Vector3 lerp(const Vector3& start, const Vector3& end, const float t) {
	return start * (1.0f - t) + end * t;
};

Vector3 Bezier(Vector3 start, Vector3 end, float time, Vector3 p1)
{
	Vector3 p3 = lerp(start, p1, time);
	Vector3 p4 = lerp(p1, end, time);

	Vector3 nowPos = lerp(p3, p4, time);

	return nowPos;
}
