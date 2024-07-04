#include "Player.h"
#include "Input.h"
#include <cassert>
#include <numbers>
#include<algorithm>

void Player::Initialize(Model* model,ViewProjection*viewProjection,const Vector3& position) {

//NULLポインタチェック
assert(model);
worldTransform_.Initialize();

//引数の内容をメンバ変数に記録
model_=Model::CreateFromOBJ("player",true);
viewProjection_ = viewProjection;

//ワールド変数の初期化
worldTransform_.Initialize();
worldTransform_.translation_ = position;

worldTransform_.rotation_.y = std::numbers::pi_v<float>/2.0f;

}

void Player::Update() {

	worldTransform_. TransferMatrix();
	worldTransform_.UpdateMatrix();

}

void Player::Draw() {

	model_->Draw(worldTransform_,*viewProjection_);

}
