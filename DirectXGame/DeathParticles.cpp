#define NOMINMAX

#include "DeathParticles.h"
#include "MathUtilityForText.h"
#include <cassert>

void DeathParticles::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {

	//NULLポインタチェック
	assert(model);

	model_ = model;

	//ワールド変数の初期化
	for (auto& WorldTransform : worldTransforms_) {
		WorldTransform.Initialize();
		WorldTransform.translation_ = position;
	}

	viewProjection_ = viewProjection;
	//objectColor_.Initialize();
	//color_ = {1,1,1,1};

}

void DeathParticles::Update() {

	counter_ += 1.0f / 60.0f;

	if (counter_ >= kDuration) {
		counter_ = kDuration;
		isFinished_ = true;
	}



}