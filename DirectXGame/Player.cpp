#include "Player.h"
#include <cassert>
#include <numbers>

void Player::Initialize(ViewProjection* viewProjection,const Vector3& pos) {

//NULLポインタチェック
//assert(model);
worldTransform_.Initialize();
worldTransform_.translation_ = pos;

worldTransform_.rotation_.y = std::numbers::pi_v<float>/2.0f;

//引数の内容をメンバ変数に記録
model_=  Model::CreateFromOBJ("player",true);
//textureHandle_=textureHandle;
viewProjection_ = viewProjection;

}

void Player::Update() {

	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;

	worldTransform_. TransferMatrix();
	worldTransform_. UpdateMatrix();
}

void Player::Draw() {

	model_->Draw(worldTransform_,*viewProjection_);

}
