#include "Enemy.h"
#include "MathUtilityForText.h"
#include <cassert>
#include <numbers>

void Enemy::Initialize(Model*model,ViewProjection* viewProjection, const Vector3& position) {

	//NULLポインタチェック
	assert(model);
	model_ = model;

	//ワールド変数の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> * 3.0f / 2.0f;

	viewProjection_ = viewProjection;

	velocity_ = {-kWalkSpeed,0,0};
	walkTimer = 0.0f;
}

void Enemy::Update() {

	worldTransform_.translation_ += velocity_;

	//回転アニメーション
	walkTimer += 1.0f / 60.0f;
	worldTransform_.rotation_.x = std::sin(std::numbers::pi_v<float> * 2.0f * walkTimer / kWalkMotionTime);

	worldTransform_.UpdateMatrix();

}

void Enemy::Draw() {

	model_->Draw(worldTransform_,*viewProjection_);

}