#pragma once

/// <summary>
/// 2次元ベクトル
/// </summary>
class Vector2 {
  public:
	float x; // x成分
	float y; // y成分

  public:
	// コンストラクタ
	Vector2();                          // 零ベクトルとする
	Vector2(float x, float y); // x成分, y成分 を指定しての生成

	//メンバ関数
	float GetLength() const;
	void  Normalized();
	Vector2  GetNormalize();
	float Dot(const Vector2& v) const;
	float Cross(const Vector2& v) const;

	Vector2  operator -();//vec同士の引き算
	Vector2& operator -=(const Vector2& other);//複合演算子(-)

	Vector2  operator +();
	Vector2& operator +=(const Vector2& other);//複合演算子(+)

	Vector2  operator *(const float& other);
	Vector2& operator *=(const float& other);//複合演算子(*)

	Vector2  operator /(const float& other);
	Vector2& operator /=(const float& other);//複合演算子(/)
};

//二項演算子オーバーロード
const Vector2 operator +(const Vector2& v1, const Vector2& v2);
const Vector2 operator -(const Vector2& v1, const Vector2& v2);
const Vector2 operator *(const Vector2& v, float s);
const Vector2 operator *(float s, const Vector2& v);
const Vector2 operator /(const Vector2& v, float s);

//補完関数
const Vector2 lerp(const Vector2& start, const Vector2& end, const float t);
