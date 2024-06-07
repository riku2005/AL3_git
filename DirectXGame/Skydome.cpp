#include "Skydome.h"
#include <cassert>

void Skydome::Initialize(Model* model,ViewProjection* viewProjection) {

	//NULLポインタチェック
	assert(model);
	worldTransform_.Initialize();

	viewProjection_ = viewProjection;
	model_ = model;

}

void Skydome::Update(){
	worldTransform_.TransferMatrix();
}

void Skydome::Draw(){
	model_->Draw(worldTransform_,*viewProjection_);
}
