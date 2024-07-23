#include "GameScene.h"
#include "MathUtilityForText.h"
#include "TextureManager.h"
#include <cassert>
#include <cstdint>
#include "CameraController.h"

GameScene::GameScene() {}

GameScene::~GameScene() {

	for (Enemy* enemy : enemies_) {
		delete enemy;
	}

	delete model_;
	delete player_;
	
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		 for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
			worldTransformBlock = nullptr;
    }
}
		worldTransformBlocks_.clear();

	delete debugCamera_;
	delete skydome_;
	delete modelSkydome_;
	delete mapChipField_;
	delete cameraController;
	delete modelBlock_;
	delete modelEnemy_;

}

void GameScene::Initialize() {

//2D
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	//textureHandle_ = TextureManager::Load("block.jpg");

//3Dモデル
	model_ = Model::Create();
	modelBlock_  = Model::CreateFromOBJ("block");
	modelEnemy_ = Model::CreateFromOBJ("enemy");
	worldTransform_.Initialize();
	viewProjection_.Initialize();
	//自キャラの生成
	player_ = new Player();


	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	//天球の生成
	//skydome_ = new Skydome();

	//天球の初期化
	//modelSkydome_ = Model::CreateFromOBJ("sphere",true);
	//skydome_->Initialize(modelSkydome_,&viewProjection_);

	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/map.csv");

	//自キャラの初期化
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(3,18);
	player_ ->Initialize(&viewProjection_,playerPosition);
	player_->SetMapChipField(mapChipField_);

	GenerateBlocks();

	cameraController = new CameraController;
	cameraController->Initialize();
	cameraController->SetTarget(player_);
	cameraController->Reset();

	CameraController::Rect cameraArea = {12.0f,100 - 12.0f,6.0f,6.0f};
	cameraController->SetMovableArea(cameraArea);

	Enemy* newEnemy = new Enemy();
	Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(14,18);
	newEnemy->Initialize(modelEnemy_,&viewProjection_,enemyPosition);

	enemies_.push_back(newEnemy);

	}

void GameScene::Update() {

	player_ ->Update();

	for (Enemy* enemy : enemies_) {
		enemy->Update();
	}

	cameraController->Update();

	#ifdef _DEBUG
	if (input_->TriggerKey(DIK_SPACE)) {
		if (isDebugCameraActive_ == true)
			isDebugCameraActive_ = false;
		else
			isDebugCameraActive_ = true;
	}
	#endif

	// カメラ処理
	if (isDebugCameraActive_) {
		// デバッグカメラの更新
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
		// ビュープロジェクション行列の更新と転送
		viewProjection_.matView = cameraController->GetViewProjection().matView;
		viewProjection_.matProjection = cameraController->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	}


	// 縦横ブロック更新
	for (std::vector<WorldTransform*> worldTransformBlockTate : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlockYoko : worldTransformBlockTate) {
			if (!worldTransformBlockYoko)
				continue;

			// アフィン変換行列の作成
			worldTransformBlockYoko->UpdateMatrix();
		}
	}

	CheckAllCollisions();

	debugCamera_->Update();
	//skydome_->Update();

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	player_ ->Draw();

	for (Enemy* enemy : enemies_) {
		enemy->Draw();
	}

	//縦横ブロック描画
	for (std::vector<WorldTransform*> worldTransformBlockTate : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlockYoko : worldTransformBlockTate) {
			if (!worldTransformBlockYoko)
				continue;

			modelBlock_->Draw(*worldTransformBlockYoko, viewProjection_);
		}
	}

	//skydome_->Draw();

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>


	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::GenerateBlocks() {

	// 要素数
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	// 要素数を変更する
	// 列数を設定（縦方向のブロック数）
	worldTransformBlocks_.resize(numBlockVirtical);

	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
    // 1列の要素数を設定（横方向のブロック数）
    worldTransformBlocks_[i].resize(numBlockHorizontal);
	}

	//ブロックの生成
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}

	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
			for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
				if (mapChipField_->GetMapChipTypeByIndex(j,i) == MapChipType::kBlock) {
					WorldTransform* worldTransform = new WorldTransform();
					worldTransform->Initialize();
					worldTransformBlocks_[i][j] = worldTransform;
					worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j,i);
				}
			}
	}


}

void GameScene::CheckAllCollisions() {
	//判定対象1と2の座標
	AABB aabb1,aabb2;

#pragma region 自キャラと敵キャラの当たり判定
	{
		//自キャラの座標
		aabb1 = player_->GetAABB();

		//自キャラと敵弾全ての当たり判定
		for (Enemy* enemy : enemies_) {
			//敵弾の座標
			aabb2 = enemy->GetAABB();

			//AABB同士の交差判定
			if (IsCollision(aabb1, aabb2)) {
				//自キャラの衝突時コールバックを呼び出す
				player_->OnCollision(enemy);
				//敵弾の衝突時コールバックを呼び出す
				enemy->OnCollision(player_);
			}
		}

	}
	#pragma endregion
}
