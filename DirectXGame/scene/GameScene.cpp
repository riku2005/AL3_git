#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {

	delete model_;
	delete player_;
	
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		 for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
    }
}
		worldTransformBlocks_.clear();

	delete debugCamera_;

	delete skydome_;

	delete modelSkydome_;

}

void GameScene::Initialize() {

//2D
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	textureHandle_ = TextureManager::Load("rinku.jpg");

//3Dモデル
	model_ = Model::Create();
	modelBlock_  = Model::Create();
	worldTransform_.Initialize();
	viewProjection_.Initialize();
	//自キャラの生成
	player_ = new Player();
	//自キャラの初期化
	player_ ->Initialize(model_,textureHandle_,&viewProjection_);

	
	// 要素数
	const uint32_t kNumBlockVirtical = 10;
	const uint32_t kNumBlockHorizontal = 20;
	// ブロック1個分の横幅
	const float kBlockWidth = 2.0f;
	const float kBlockHeight = 2.0f;

	// 要素数を変更する
	// 列数を設定（縦方向のブロック数）
	worldTransformBlocks_.resize(kNumBlockVirtical);
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
    // 1列の要素数を設定（横方向のブロック数）
    worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}

	// キューブの生成
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}

	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
			for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
				if (j % 2 == (i%2)) {
					worldTransformBlocks_[i][j] = new WorldTransform();
					worldTransformBlocks_[i][j]->Initialize();
					worldTransformBlocks_[i][j]->translation_.x = kBlockWidth * j;
					worldTransformBlocks_[i][j]->translation_.y = kBlockHeight * i;
				} else {
					worldTransformBlocks_[i][j] = nullptr;
				}
			}
	}

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	//天球の生成
	skydome_ = new Skydome();

	//天球の初期化
	modelSkydome_ = Model::CreateFromOBJ("sphere",true);
	skydome_->Initialize(modelSkydome_,&viewProjection_);


	}

void GameScene::Update() {

	player_ ->Update();

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
		viewProjection_.UpdateMatrix();
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

	debugCamera_->Update();

	skydome_->Update();

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

	//縦横ブロック描画
	for (std::vector<WorldTransform*> worldTransformBlockTate : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlockYoko : worldTransformBlockTate) {
			if (!worldTransformBlockYoko)
				continue;

			modelBlock_->Draw(*worldTransformBlockYoko, viewProjection_);
		}
	}

	skydome_->Draw();

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
