#include "Player.h"
#include "Input.h"
#include <cassert>
#include <numbers>
#include <algorithm>

void Player::Initialize(ViewProjection*viewProjection,const Vector3& position) {

//NULLポインタチェック
//assert(model);
worldTransform_.Initialize();
worldTransform_.translation_ = position;

worldTransform_.rotation_.y = std::numbers::pi_v<float>/2.0f;

//引数の内容をメンバ変数に記録
model_=Model::CreateFromOBJ("player",true);
viewProjection_ = viewProjection;

}

void Player::Update() {

	//移動入力
	//左右移動操作
	if (Input::GetInstance()->PushKey(DIK_RIGHT) ||
		Input::GetInstance()->PushKey(DIK_LEFT)){

	//左右加速
	Vector3 acceleration = {};

	if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
		
		acceleration.x += kAcceleration;

	}
	else if (Input::GetInstance()->PushKey(DIK_LEFT)) {

		acceleration.x -= kAcceleration;

	}
		
	//加速/減速
	velocity_ += acceleration;

	worldTransform_.translation_ += velocity_;

	}

	worldTransform_. TransferMatrix();
	worldTransform_.UpdateMatrix();

}

void Player::Draw() {

	model_->Draw(worldTransform_,*viewProjection_);

}
