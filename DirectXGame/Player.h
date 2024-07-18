#pragma once

#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class MapChipField;

class Player {

public:

	//左右
	enum class LRDirection {
		kRight,
		kLeft,
	};

	//角
	enum Corner {
		kRightBottom,
		kLeftBottom,
		kRightTop,
		kLeftTop,

		kNumCorner
	};

	void Initialize(ViewProjection*viewProjection,const Vector3& position);

	void Update();

	void Draw();

	//getter
	const WorldTransform& GetWorldTransform() const {return worldTransform_;}
	const Vector3& GetVelocity() const {return velocity_;}

private:

	static inline const float kAcceleration = 0.01f;
	static inline const float kAttenuation = 0.01f;
	static inline const float kLimitRunSpeed = 2.0f;
	//重力加速度(下方向)
	static inline const float kGravityAcceleration = 0.05f;
	//最大落下速度(下方向)
	static inline const float kLimitFallSpeed = 0.5f;
	//ジャンプ初速(上方向)
	static inline const float kJumpAcceleration = 0.7f;
	//旋回時間<秒>
	static inline const float kTimeTurn = 0.3f;
	static inline const float kAttenuationWall = 0.2f;
	static inline const float kattenuationLanding = 0.0f;
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;
	static inline const float kBlank = 0.04f;
	static inline const float kGroundSearchHeight = 0.06f;

	struct CollisionMapInfo {
		bool ceiling = false;
		bool landing = false;
		bool hitWall = false;
		Vector3 move;
	};

	Vector3 velocity_{};

	LRDirection lrDirection_ = LRDirection::kRight;

	//旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;

	//旋回タイマー
	float turnTimer_ = 0.0f;

	//接地状態フラグ
	bool onGround_ = true;
	
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	
	ViewProjection* viewProjection_ = nullptr;

	//マップチップフィールド
	MapChipField* mapChipField_ = nullptr;

	void InputMove();
	void CheckMapCollision(CollisionMapInfo& info);
	void CheckMapCollisionUp(CollisionMapInfo& info);
	void CheckMapCollisionDown(CollisionMapInfo& info);
	void CheckMapCollisionRight(CollisionMapInfo& info);
	void CheckMapCollisionLeft(CollisionMapInfo& info);
	void UpdateOnGround(CollisionMapInfo& info);
	void AnimateTurn();

	Vector3 CornerPosition(const Vector3& center,Corner corner);
};