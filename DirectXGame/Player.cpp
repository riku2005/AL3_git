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
	//左右加速
	if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
		
		//左移動中の右入力
		if (velocity_.x < 0.0f) {
			//速度と逆方向に入力中は急ブレーキ
			velocity_.x *= (1.0f - kAttenuation);
		}

		acceleration.x += kAcceleration;

		if (lrDirection_ != LRDirection::kRight) {
			lrDirection_ = LRDirection::kRight;

			//旋回開始時の角度
			turnFirstRotationY_ = worldTransform_.rotation_.y;
			//旋回タイマー
			turnTimer_ = 0.7f;
		}

	}
	else if (Input::GetInstance()->PushKey(DIK_LEFT)) {

		//右移動中の左入力
		if (velocity_.x > 0.0f) {
			//速度と逆方向に入力中は急ブレーキ
			velocity_.x *= (1.0f - kAttenuation);
		}

		acceleration.x -= kAcceleration;

		if (lrDirection_ != LRDirection::kLeft) {
			lrDirection_ = LRDirection::kLeft;

			//旋回開始時の角度
			turnFirstRotationY_ = worldTransform_.rotation_.y;
			//旋回タイマー
			turnTimer_ = 0.7f;
		}

	}
		
	//加速/減速
	velocity_.x += acceleration.x;
	velocity_.y += acceleration.y;
	velocity_.z += acceleration.z;

	//最大速度制限
	velocity_.x = std::clamp(velocity_.x,-kLimitRunSpeed,kLimitRunSpeed);
	
	if (acceleration.x >= 0.01f || acceleration.x <= -0.01f) {
		acceleration.x = 0;
	}

	if (turnTimer_ > 0.0f) {
		//タイマーのカウントダウン
		turnTimer_ -= 1.0f / 60.0f;
	}

		//左右の自キャラ角度テーブル
	float destinationRotationYTable[] = {
		std::numbers::pi_v<float> / 2.0f,
		std::numbers::pi_v<float> * 3.0f / 2.0f
		};
		//状態に応じた角度を取得する
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		//自キャラの角度を設定する
		worldTransform_.rotation_.y = destinationRotationY;
	}else {
		velocity_.x *= (1.0f - kAttenuation);
	}

	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;

	if (onGround_) {

	}

	worldTransform_. TransferMatrix();
	worldTransform_.UpdateMatrix();

}

void Player::Draw() {

	model_->Draw(worldTransform_,*viewProjection_);

}
