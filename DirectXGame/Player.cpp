#include "Player.h"
#include "Input.h"
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

	// 移動入力
    // 左右移動操作
    if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
    // 左右加速
    Vector3 acceleration = {};
    if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
        acceleration.x += kAcceleration;
    } else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
        acceleration.x -= kAcceleration;
    }
    // 加速／減速
    velocity_.x += acceleration.x;
    velocity_.y += acceleration.y;
    velocity_.z += acceleration.z;
}






	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;

	worldTransform_. TransferMatrix();
	worldTransform_. UpdateMatrix();
}

void Player::Draw() {

	model_->Draw(worldTransform_,*viewProjection_);

}
