#pragma once
#include "Vector3.h"

//�⊮�֐�
const Vector3 lerp(const Vector3& start, const Vector3& end, const float t);

Vector3 Bezier(Vector3 start, Vector3 end, float time, Vector3 p1 = { 0,0,0 });



//class Bezier{};

