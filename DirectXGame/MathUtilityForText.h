﻿#pragma once
#include "GameScene.h"

//代入演算子オーバーロード
//Vector3の足し算
Vector3& operator+=(Vector3& lhv,const Vector3& rhv);

//行列の掛け算
Matrix4x4 MatrixMultiply(Matrix4x4& m1,Matrix4x4& m2);

float EaseInOut(float x1,float x2,float t);
float Lerp(float x1,float x2,float t);
Vector3 Lerp(const Vector3& v1,const Vector3& v2,float t);