#pragma once
#include "ViewProjection.h"

//前方宣言
class Player;

//カメラコントローラー
class CameraController {

public:

	//矩形
	struct Ract {
		float left = 0.0f;
		float right = 1.0f;
		float bottom = 0.0f;
		float top = 1.0f;
	};


	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	void SetTarget(Player* target){target_ = target;}
	void Reset();

private:

	//ビュープロジェクション
	ViewProjection viewProjection_;
	Player* target_ = nullptr;

	//追従対象とカメラの座標の差
	Vector3 targetOffset_ = {0,0,-30.0f};


};