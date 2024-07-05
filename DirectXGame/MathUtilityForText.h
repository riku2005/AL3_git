#pragma once
#include "Vector3.h"

//Vector3の足し算
Vector3& operator+=(Vector3& lhv,const Vector3& rhv);

// アフィン変換行列の作成
//Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate);

// 行列の掛け算
//Matrix4x4 MatrixMultiply(Matrix4x4& m1, Matrix4x4& m2);

