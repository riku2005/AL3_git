#define NOMINMAX
#include "Player.h"
#include "DebugText.h"
#include "Input.h"
#include "MathUtilityForText.h"
#include "MapChipField.h"
#include "DirectXCommon.h"
#include "Easing.h"
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
	InputMove();

	//衝突情報を初期化
	CollisionMapInfo collisionMapInfo = {};
	collisionMapInfo.move = velocity_;
	collisionMapInfo.landing = false;
	collisionMapInfo.hitWall = false;

	//マップ衝突チェック
	CheckMapCollision(collisionMapInfo);
	
	//移動
	worldTransform_.translation_ += collisionMapInfo.move;

	//天井接触による落下開始
	if (collisionMapInfo.ceiling) {
		velocity_.y = 0;
	}

	//壁接触による減速
	if (collisionMapInfo.hitWall) {
		velocity_.x *= (1.0f - kAttenuationWall);
	}

	//接地判定
	UpdateOnGround(collisionMapInfo);
	//旋回制御
	AnimateTurn();
	//行列計算
	worldTransform_.UpdateMatrix();

}

void Player::Draw() {

	model_->Draw(worldTransform_,*viewProjection_);

}

Vector3 Player::GetWorldPosition() {
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

AABB Player::GetAABB() {
	Vector3 worldPos = GetWorldPosition();
	AABB aabb;
	aabb.min = {worldPos.x - kWidth / 2.0f,worldPos.y - kHeight / 2.0f,worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f,worldPos.y + kHeight / 2.0f,worldPos.z + kWidth / 2.0f};
	return aabb;
}

void Player::OnCollision(const Enemy* enemy) {
	(void)enemy;
	isDead_ = true;
	
}

void Player::InputMove() {

	//左右移動操作
	if(onGround_){
		if (Input::GetInstance()->PushKey(DIK_RIGHT) ||
			Input::GetInstance()->PushKey(DIK_LEFT)){

			//左右加速
			Vector3 acceleration = {};
			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
		
				//左移動中の右入力
				if (velocity_.x < 0.0f) {
					//速度と逆方向に入力中は急ブレーキ
					velocity_.x *= (1.0f - kAttenuation);
				}

				acceleration.x += kAcceleration / 60.0f;

				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;

					//旋回開始時の角度
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					//旋回タイマー
					turnTimer_ = kTimeTurn;
				}
			}
			else if (Input::GetInstance()->PushKey(DIK_LEFT)) {

				//右移動中の左入力
				if (velocity_.x > 0.0f) {
					//速度と逆方向に入力中は急ブレーキ
					velocity_.x *= (1.0f - kAttenuation);
				}
				
				acceleration.x -= kAcceleration / 60.0f;

				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft;

					//旋回開始時の角度
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					//旋回タイマー
					turnTimer_ = kTimeTurn;
				}

			}
		
			//加速/減速
			velocity_.x += acceleration.x;
			velocity_.y += acceleration.y;
			velocity_.z += acceleration.z;

			//最大速度制限
			velocity_.x = std::clamp(velocity_.x,-kLimitRunSpeed,kLimitRunSpeed);
		}else{
			velocity_.x *= (1.0f - kAttenuation);
		}
	
		if (std::abs(velocity_.x) <= 0.0001f){
			velocity_.x = 0.0f;
		}
	
		if(Input::GetInstance()->PushKey(DIK_UP)){
			velocity_ += Vector3(0,kJumpAcceleration / 60.0f,0);
			velocity_.x += 0;
			velocity_.y += kJumpAcceleration;
			velocity_.z += 0;

			velocity_.y = std::max(velocity_.y,-kLimitFallSpeed);
		}
	}
		else {
			//落下速度
			velocity_ += Vector3(0, -kGravityAcceleration / 60.0f,0);
			velocity_.x += 0;
			velocity_.y += -kGravityAcceleration;
			velocity_.z += 0;

			//落下速度制限
			velocity_.y = std::max(velocity_.y,-kLimitFallSpeed);
		}
}
	void Player::CheckMapCollision(CollisionMapInfo& info){
		CheckMapCollisionUp(info);
		CheckMapCollisionDown(info);
		CheckMapCollisionRight(info);
		CheckMapCollisionLeft(info);
	}

	void Player::CheckMapCollisionUp(CollisionMapInfo& info){
		
		if(info.move.y <= 0){
			return;
			}
		std::array<Vector3,kNumCorner>positionsNew;

		for(uint32_t i = 0;i < positionsNew.size();++i){
			
			positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move,static_cast<Corner>(i));
			
			}

		MapChipType mapChipType;
		MapChipType mapChipTypeNext;

		//真上の当たり判定を狙う
		bool hit = false;
		//左上点の判定
		MapChipField::IndexSet indexSet;
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
		mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex,indexSet.yIndex);
		mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex,indexSet.yIndex + 1);

		if(mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock){
			hit = true;
			}

		//右上点の判定
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
		mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex,indexSet.yIndex);
		mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex,indexSet.yIndex + 1);
			
			if(mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock){
				hit = true;
				}
		
			if (hit) {
				MapChipField::IndexSet indexSetNow;

				indexSetNow = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(0, +kHeight / 2.0f,0));

				if (indexSetNow.yIndex != indexSet.yIndex) {
					indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(0, +kHeight / 2.0f,0));
					MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex,indexSet.yIndex);
					info.move.y = std::max(0.0f,rect.bottom - worldTransform_.translation_.y - (kHeight / 2.0f + kBlank));
						info.ceiling = true;
				}

		}

	}

	void Player::CheckMapCollisionDown(CollisionMapInfo& info) {
		if (info.move.y >= 0) {
			return;
		}
		std::array<Vector3,kNumCorner>positionsNew;

		for (uint32_t i = 0; i < positionsNew.size(); ++i) {
			positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move,static_cast<Corner>(i));
		}

		MapChipType mapChipType;
		MapChipType mapChipTypeNext;
		bool hit = false;

		//左下
		MapChipField::IndexSet indexSet;
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
		mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex,indexSet.yIndex);
		mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex,indexSet.yIndex - 1);

		if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
			hit = true;
		}

		//右下
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
		mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex,indexSet.yIndex);
		mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex,indexSet.yIndex - 1);

		if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
			hit = true;
		}

		//ブロックにヒット？
		if (hit) {
			MapChipField::IndexSet indexSetNow;
			indexSetNow = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(0, -kHeight / 2.0f,0));
			if (indexSetNow.yIndex != indexSet.yIndex) {
				indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(0, -kHeight / 2.0f,0));
				MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex,indexSet.yIndex);
				info.move.y = std::min(0.0f,rect.top - worldTransform_.translation_.y + (kHeight / 2.0f + kBlank));
				info.landing = true;
			}
		}

	}

	void Player::CheckMapCollisionRight(CollisionMapInfo& info) {
		//右移動あり？
		if (info.move.x <= 0) {
			return;
		}

		std::array<Vector3,kNumCorner>positionsNew;

		for (uint32_t i = 0; i < positionsNew.size(); ++i) {
			positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move,static_cast<Corner>(i));
		}

		MapChipType mapChipType;
		MapChipType mapChipTypeNext;
		bool hit = false;

		//右上点の判定
		MapChipField::IndexSet indexSet;
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
		mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex,indexSet.yIndex);
		mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex - 1,indexSet.yIndex);
		if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
			hit = true;
		}

		//右下点の判定
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
		mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex,indexSet.yIndex);
		mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex - 1,indexSet.yIndex);
		if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
			hit = true;
		}

		//ブロックにヒット？
		
		if (hit) {
		//現在座標が壁の外か判定
			MapChipField::IndexSet indexSetNow;
			indexSetNow = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(+kWidth / 2.0f,0,0));
			if (indexSetNow.xIndex != indexSet.xIndex) {
				indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(+kWidth / 2.0f,0,0));
				MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex,indexSet.yIndex);
				info.move.x = std::max(0.0f,rect.left - worldTransform_.translation_.x - (kWidth / 2.0f + kBlank));
				info.hitWall = true;
			}

		}
		

	}
	

	void Player::CheckMapCollisionLeft(CollisionMapInfo& info) {

		//左移動あり？
		if (info.move.x >= 0) {
			return;
		}

		std::array<Vector3,kNumCorner>positionsNew;

		for (uint32_t i = 0; i < positionsNew.size(); ++i) {
			positionsNew[i]= CornerPosition(worldTransform_.translation_ + info.move,static_cast<Corner>(i));
		}

		MapChipType mapChipType;
		MapChipType mapChipTypeNext;
		bool hit = false;

		//左上点の判定
		MapChipField::IndexSet indexSet;
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
		mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex,indexSet.yIndex);
		mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex + 1,indexSet.yIndex);
		if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
			hit = true;
		}

		//左下点の判定
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
		mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex,indexSet.yIndex);
		mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex + 1,indexSet.yIndex);
		if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
			hit = true;
		}

		//ブロックにヒット？
		
		if (hit) {
		//現在座標が壁の外か判定
			MapChipField::IndexSet indexSetNow;
			indexSetNow = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(-kWidth / 2.0f,0,0));
			if (indexSetNow.xIndex != indexSet.xIndex) {
				indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(-kWidth / 2.0f,0,0));
				MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex,indexSet.yIndex);
				info.move.x = std::min(0.0f,rect.right - worldTransform_.translation_.x + (kWidth / 2.0f + kBlank));
				info.hitWall = true;
			}

		}

	}

	void Player::UpdateOnGround(const CollisionMapInfo& info) {
		if (onGround_) {
			//ジャンプ開始
			if (velocity_.y > 0.0f) {
				onGround_ = false;

			}else {
				std::array<Vector3,kNumCorner>positionsNew;

				for (uint32_t i = 0; i < positionsNew.size(); ++i) {
					CornerPosition(worldTransform_.translation_ + info.move,static_cast<Corner>(i));
				}
				bool ground = false;

				MapChipType mapChipType;

				//左下点の判定
				MapChipField::IndexSet indexSet;
				indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom] + Vector3(0,-kGroundSearchHeight,0));
				mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex,indexSet.yIndex);
					if (mapChipType == MapChipType::kBlock) {
						ground = true;
					}

				//右下点の判定
				indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom] + Vector3(0,-kGroundSearchHeight,0));
				mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex,indexSet.yIndex);
					if (mapChipType == MapChipType::kBlock) {
						ground = true;
					}
				//落下開始
					if (!ground) {
						DebugText::GetInstance()->ConsolePrintf("jump");
						onGround_ = false;
				    }

			}
		}
		else {
			//着地
			if (info.landing) {
				velocity_.x *= (1.0f - kAttenuationLanding);
				velocity_.y = 0.0f;

				DebugText::GetInstance()->ConsolePrintf("onGround");
					onGround_ = true;
			}
		}
	}

	void Player::AnimateTurn() {
		if (turnTimer_ > 0.0f) {
			//タイマーを進める
			turnTimer_ = std::max(turnTimer_ - (1.0f - 60.0f),0.0f);

			float destinationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f,std::numbers::pi_v<float> * 3.0f / 2.0f};

			float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];

			worldTransform_.rotation_.y = EaseInOut(destinationRotationY,turnFirstRotationY_,turnTimer_ / kTimeTurn);
			worldTransform_.rotation_.y = destinationRotationY;
		}
		else {
			velocity_.x *= (1.0f - kAttenuation);
		}
	}

	Vector3 Player::CornerPosition(const Vector3& center, Corner corner) {
		Vector3 offsetTable[] = {
			{+kWidth / 2.0f,-kHeight / 2.0f,0},
			{-kWidth / 2.0f,-kHeight / 2.0f,0},
			{+kWidth / 2.0f,+kHeight / 2.0f,0},
			{-kWidth / 2.0f,+kHeight / 2.0f,0}
		};
		return center + offsetTable[static_cast<uint32_t>(corner)];
	}

//}
