#pragma once
#include "Model.h"
#include "WorldTransform.h"

//天球
class Skydome {

public:

	void Initialize(Model* model,ViewProjection* viewProjection);


	void Update();


	void Draw();

private:
	//ワールド変換データ
	WorldTransform worldTransform_;

	//モデル
	Model* model_ = nullptr;

	ViewProjection* viewProjection_ = nullptr;

};