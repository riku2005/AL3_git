#include "CameraController.h"
#include "Player.h"
#include <algorithm>

//初期化
void CameraController::Initialize() {
		viewProjection_.Initialize();
}

//更新
void CameraController::Update() {

}

void CameraController::Reset() {

	//追従対象のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_ ->GetWorldTransform();
	//追従対象とオフセットからカメラの座標を計算
	viewProjection_.translation_ = targetWorldTransform.translation_ + targetOffset_;

}