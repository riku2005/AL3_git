#pragma once

#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class Player {

public:

	void Initialize(ViewProjection* viewProjection,const Vector3& pos);

	void Update();

	void Draw();

private:
	Vector3 velocity_ {};
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	
	ViewProjection* viewProjection_ = nullptr;

};