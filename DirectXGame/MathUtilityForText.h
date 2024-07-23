#pragma once
#include "AABB.h"
#include "Matrix4x4.h"
#include "Vector3.h"

//円周率
const float PI = 3.141592654f;

//単項演算子オーバーロード
Vector3 operator+(const Vector3 & v);
Vector3 operator-(const Vector3& v);

//代入演算子オーバーロード
Vector3& operator+=(Vector3& lhv,const Vector3& rhv);
Vector3& operator-=(Vector3& lhv,const Vector3& rhv);
Vector3& operator*=(Vector3& v,float s);
Vector3& operator/=(Vector3& v,float s);

//2項演算子オーバーロード
const Vector3 operator+(const Vector3& v1,const Vector3& v2);
const Vector3 operator-(const Vector3& v1,const Vector3& v2);
const Vector3 operator*(const Vector3& v,float s);
const Vector3 operator*(float s, const Vector3& v);
const Vector3 operator/(const Vector3& v,float s);

//単位行列の作成
Matrix4x4 MakeIdentityMatrix();
//スケーリング行列の作成
Matrix4x4 MakeScaleMatrix(const Vector3& scale);
//回転行列の作成
Matrix4x4 MakeRotateXMatrix(float theta);
Matrix4x4 MakeRotateYMatrix(float theta);
Matrix4x4 MakeRotateZMatrix(float theta);
//平行移動行列の作成
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

//アフィン変換行列の作成
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate);

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);


// 行列の掛け算
//Matrix4x4 MatrixMultiply(Matrix4x4& m1, Matrix4x4& m2);

//代入演算子オーバーロード
Matrix4x4& operator*=(Matrix4x4& lhm,const Matrix4x4& rhm);
//2項演算子オーバーロード
Matrix4x4 operator*(const Matrix4x4& m1,const Matrix4x4& m2);

float Lerp(float x1,float x2,float t);

float EaseIn(float x1,float x2,float t);

float EaseOut(float x1,float x2,float t);

float EaseInOut(float x1,float x2,float t);

Vector3 Lerp(const Vector3& v1,const Vector3& v2,float t);

bool IsCollision(const AABB& aabb1, const AABB& aabb2);

