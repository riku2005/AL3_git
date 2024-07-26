#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Player.h"
#include "DebugCamera.h"
#include "Skydome.h"
#include "MapChipField.h"
#include "CameraController.h"
#include "Enemy.h"
#include "DeathParticles.h"
#include <list>
#include <vector>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	bool IsFinished() const {return finished_;}

private: // メンバ変数

	enum class Phase {
		kPlay,
		kDeath,
	};

	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	uint32_t textureHandle_ = 0;

	//3Dモデル
	Model* model_ = nullptr;
	Model* modelBlock_ = nullptr;
	Model* modelEnemy_ = nullptr;
	Model* modelDeathParticle_ = nullptr;
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	//自キャラ
	Player* player_ = nullptr;

	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;

	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	//天球
	Skydome* skydome_ = nullptr;

	Model* modelSkydome_ = nullptr;

	//マップチップフィールド
	MapChipField* mapChipField_ = nullptr;

	CameraController* cameraController = nullptr;

	Enemy* enemy_ = nullptr;

	std::list<Enemy*> enemies_;

	bool finished_ = false;
	Phase phase_;

	DeathParticles* deathParticles_ = nullptr;

	void ChangePhase();

	void GenerateBlocks();

	void UpdateCamera();

	void UpdateBlocks();

	void CheckAllCollisions();

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
