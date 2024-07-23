#include "MathUtilityForText.h"
#include <cmath>
#include <numbers>

//単項演算子オーバーロード
Vector3 operator+(const Vector3& v){return v;}
Vector3 operator-(const Vector3& v){return Vector3(-v.x,-v.y,-v.z);}

//Vector3の足し算
Vector3& operator+=(Vector3& lhv, const Vector3& rhv) {
	lhv.x += rhv.x;
	lhv.y += rhv.y;
	lhv.z += rhv.z;
	return lhv;
}

//Vrctor3の引き算
Vector3& operator-=(Vector3& lhv, const Vector3& rhv) {
	lhv.x -= rhv.x;
	lhv.y -= rhv.y;
	lhv.z -= rhv.z;
	return lhv;
}

//Vector3の掛け算
Vector3& operator*=(Vector3& v,float s) {
	v.x *= s;
	v.y *= s;
	v.z *= s;
	return v;
}

//Vector3の割り算
Vector3& operator/=(Vector3& v,float s) {
	v.x /= s;
	v.y /= s;
	v.z /= s;
	return v;
}

//2項演算子オーバーロード
const Vector3 operator+(const Vector3& v1, const Vector3& v2) {
	Vector3 temp(v1);
	return temp += v2;
}

const Vector3 operator-(const Vector3& v1, const Vector3& v2) {
	Vector3 temp(v1);
	return temp -= v2;
}

const Vector3 operator*(const Vector3& v, float s) {
	Vector3 temp(v);
	return temp *= s;
}

const Vector3 operator*(float s,const Vector3& v){return v * s;}

const Vector3 operator/(const Vector3& v, float s) {
	Vector3 temp(v);
	return temp /= s;
}

Matrix4x4 MakeIdentityMatrix() {
	static const Matrix4x4 result{1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f};
	return result;
}

Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 result{scale.x,0.0f,0.0f,0.0f,0.0f,scale.y,0.0f,0.0f,0.0f,0.0f,scale.z,0.0f,0.0f,0.0f,0.0f,1.0f};
	return result;
}

Matrix4x4 MakeRotateXMatrix(float theta) {
	float sin = std::sin(theta);
	float cos = std::cos(theta);
	Matrix4x4 result{1.0f,0.0f,0.0f,0.0f,0.0f,cos,sin,0.0f,0.0f,-sin,cos,0.0f,0.0f,0.0f,0.0f,1.0f};
	return result;
}

Matrix4x4 MakeRotateYMatrix(float theta) {
	float sin = std::sin(theta);
	float cos = std::cos(theta);
	Matrix4x4 result{cos,0.0f,-sin,0.0f,0.0f,1.0f,0.0f,0.0f,sin,0.0f,cos,0.0f,0.0f,0.0f,0.0f,1.0f};
	return result;
}

Matrix4x4 MakeRotateZMatrix(float theta) {
	float sin = std::sin(theta);
	float cos = std::cos(theta);
	Matrix4x4 result{cos,sin,0.0f,0.0f,-sin,cos,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f};
	return result;
}

Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
	Matrix4x4 result{1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,translate.x,translate.y,translate.z,1.0f};
	return result;
}

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate){
	Matrix4x4 matScale = MakeScaleMatrix(scale);
	Matrix4x4 matRotX = MakeRotateXMatrix(rot.x);
	Matrix4x4 matRotY = MakeRotateYMatrix(rot.y);
	Matrix4x4 matRotZ = MakeRotateZMatrix(rot.z);
	Matrix4x4 matRot = matRotZ * matRotX * matRotY;
	Matrix4x4 matTrans = MakeTranslateMatrix(translate);
	Matrix4x4 matTransform = matScale * matRot * matTrans;
	return matTransform;
}

Matrix4x4& operator*=(Matrix4x4& lhm, const Matrix4x4& rhm) {
	Matrix4x4 result{};

	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 4; j++) {
			for (size_t k = 0; k < 4; k++) {
				result.m[i][j] += lhm.m[i][k] * rhm.m[k][j];
			}
		}
	}
	lhm = result;
	return lhm;
}

Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result = m1;
	return result *= m2;
}

float Lerp(float x1,float x2,float t){return(1.0f - t)*x1+t*x2;}

float EaseIn(float x1,float x2,float t){
	float easedT = t*t;
	return Lerp(x1,x2,easedT);
}

float EaseOut(float x1, float x2, float t) {
	float easedT = 1.0f - std::powf(1.0f - t,3.0f);
	return Lerp(x1,x2,easedT);
}

float EaseInOut(float x1, float x2, float t) {
	float easedT = -(std::cosf(std::numbers::pi_v<float> * t)-1.0f)/2.0f;
	return Lerp(x1,x2,easedT);
}

Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) {
	return Vector3(Lerp(v1.x,v2.x,t),Lerp(v1.y,v2.y,t),Lerp(v1.z,v2.z,t));
}

bool IsCollision(const AABB& aabb1, const AABB& aabb2) {
	return (aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) &&
		(aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) &&
		(aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z);
}

// アフィン変換行列の作成
/*Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate){

	Matrix4x4 ScallMat,
			  RotateMat,RotateMatX,RotateMatY,RotateMatZ,
			  TranslateMat,
			  returnMat;

	// スケール行列作成
	ScallMat = {scale.x, 0, 0, 0,
				0, scale.y, 0, 0,
				0, 0, scale.z, 0,
				0, 0, 0, 1};

	// XYZ回転行列作成
	RotateMatX = {1, 0, 0, 0,
				  0, cosf(rot.x), sinf(rot.x), 0,
				  0,-sinf(rot.x), cosf(rot.x), 0,
				  0, 0, 0, 1};

	RotateMatY = {cosf(rot.y), 0,-sinf(rot.y), 0,
				  0, 1, 0, 0,
				  sinf(rot.y), 0, cosf(rot.y), 0,
				  0, 0, 0, 1};

	RotateMatZ = {cosf(rot.z), sinf(rot.z), 0, 0,
				 -sinf(rot.z), cosf(rot.z), 0, 0,
				  0, 0, 1, 0,
				  0, 0, 0, 1};

	// XYZ回転行列の合成(Z*X*Y)
	RotateMat = MatrixMultiply(RotateMatZ, RotateMatX);
	// ↑の結果＊Y軸回転
	RotateMat = MatrixMultiply(RotateMat, RotateMatY);

	// 平行移動行列作成
	TranslateMat = {1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0,
					translate.x, translate.y, translate.z, 1};

	// スケール＊回転＊平行移動をワールド変換行列に
	returnMat = MatrixMultiply(ScallMat, RotateMat);
	returnMat = MatrixMultiply(returnMat, TranslateMat);

	return returnMat;
}

// 行列の掛け算
Matrix4x4 MatrixMultiply(Matrix4x4& m1, Matrix4x4& m2) {

	Matrix4x4 result;
	result.m[0][0] = m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0] + m1.m[0][3] * m2.m[3][0];
	result.m[0][1] = m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] + m1.m[0][2] * m2.m[2][1] + m1.m[0][3] * m2.m[3][1];
	result.m[0][2] = m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2] + m1.m[0][3] * m2.m[3][2];
	result.m[0][3] = m1.m[0][0] * m2.m[0][3] + m1.m[0][1] * m2.m[1][3] + m1.m[0][2] * m2.m[2][3] + m1.m[0][3] * m2.m[3][3];
	result.m[1][0] = m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] + m1.m[1][2] * m2.m[2][0] + m1.m[1][3] * m2.m[3][0];
	result.m[1][1] = m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1] + m1.m[1][2] * m2.m[2][1] + m1.m[1][3] * m2.m[3][1];
	result.m[1][2] = m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][2] + m1.m[1][2] * m2.m[2][2] + m1.m[1][3] * m2.m[3][2];
	result.m[1][3] = m1.m[1][0] * m2.m[0][3] + m1.m[1][1] * m2.m[1][3] + m1.m[1][2] * m2.m[2][3] + m1.m[1][3] * m2.m[3][3];
	result.m[2][0] = m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] + m1.m[2][2] * m2.m[2][0] + m1.m[2][3] * m2.m[3][0];
	result.m[2][1] = m1.m[2][0] * m2.m[0][1] + m1.m[2][1] * m2.m[1][1] + m1.m[2][2] * m2.m[2][1] + m1.m[2][3] * m2.m[3][1];
	result.m[2][2] = m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][2] + m1.m[2][2] * m2.m[2][2] + m1.m[2][3] * m2.m[3][2];
	result.m[2][3] = m1.m[2][0] * m2.m[0][3] + m1.m[2][1] * m2.m[1][3] + m1.m[2][2] * m2.m[2][3] + m1.m[2][3] * m2.m[3][3];
	result.m[3][0] = m1.m[3][0] * m2.m[0][0] + m1.m[3][1] * m2.m[1][0] + m1.m[3][2] * m2.m[2][0] + m1.m[3][3] * m2.m[3][0];
	result.m[3][1] = m1.m[3][0] * m2.m[0][1] + m1.m[3][1] * m2.m[1][1] + m1.m[3][2] * m2.m[2][1] + m1.m[3][3] * m2.m[3][1];
	result.m[3][2] = m1.m[3][0] * m2.m[0][2] + m1.m[3][1] * m2.m[1][2] + m1.m[3][2] * m2.m[2][2] + m1.m[3][3] * m2.m[3][2];
	result.m[3][3] = m1.m[3][0] * m2.m[0][3] + m1.m[3][1] * m2.m[1][3] + m1.m[3][2] * m2.m[2][3] + m1.m[3][3] * m2.m[3][3];

	return result;
}*/

