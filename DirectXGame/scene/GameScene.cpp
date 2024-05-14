#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"

GameScene::GameScene() {}

GameScene::~GameScene() {	//デストラクタ
	delete sprite_;
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {

//2D
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	textureHandle_ = TextureManager::Load("mario.png");
	sprite_ = Sprite::Create(textureHandle_,{100,50});

//3Dモデル
	model_ = Model::Create();
	worldTransform_.Initialize();
	viewProjection_.Initialize();

//サウンド
	soundDataHandle_ = audio_->LoadWave("fanfare.wav");
	audio_->PlayWave(soundDataHandle_);
	voiceHandle_ = audio_->PlayWave(soundDataHandle_,true);

//デバッグカメラ
	debugCamera_ = new DebugCamera(1280,720);

//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
//軸方向表示が参照するビュープロジェクションを指定する
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
}

void GameScene::Update() {

//スプライトの今の座標を取得
	Vector2 position = sprite_ ->GetPosition();
//座標を{2,1}移動
	position.x += 2.0f;
	position.y += 1.0f;
//移動した座標をスプライトに反映
	sprite_->SetPosition(position);

//スペースキーを押した瞬間音声停止
	if (input_->TriggerKey(DIK_SPACE)) {
		audio_->StopWave(voiceHandle_);
	}

//デバッグカメラの更新
	debugCamera_->Update();
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
	///3Dモデル描画
	model_->Draw(worldTransform_,debugCamera_->GetViewProjection(),textureHandle_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	sprite_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();


#pragma endregion
}
