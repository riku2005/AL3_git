#pragma once

#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "MathUtilityForText.h"

class Player {

public:

	//左右
	enum class LRDirection {
		kRight,
		kLeft,
	};

	void Initialize(ViewProjection*viewProjection,const Vector3& position);

	void Update();

	void Draw();

private:

	static inline const float kAcceleration = 0.01f;
	static inline const float kAttenuation = 0.01f;
	static inline const float kLimitRunSpeed = 2.0f;

	Vector3 velocity_{};

	LRDirection lrDirection_ = LRDirection::kRight;

	//旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;

	//旋回タイマー
	float turnTimer_ = 0.0f;

	//旋回時間<秒>
	static inline const float kTimeTurn = 0.3f;

	//接地状態フラグ
	bool onGround_ = true;
	//着地フラグ
	//bool landing = false;

	//重力加速度(下方向)
	static inline const float kGravityAcceleration = 0.05f;
	//最大落下速度(下方向)
	static inline const float kLimitFallSpeed = 0.2f;
	//ジャンプ初速(上方向)
	static inline const float kJumpAcceleration = 0.7f;

	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	
	ViewProjection* viewProjection_ = nullptr;

};