#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Application.h"
#include "../Manager/Camera.h"
#include "../Utility/AsoUtility.h"
#include "../Object/RockManager.h"
#include "../Object/Grid.h"
#include "../Object/SpaceDome.h"
#include "../Object/Ship/PlayerShip.h"
#include "../Object/Ship/NPC.h"
#include "../Object/Stage.h"
#include "GameScene.h"

GameScene::GameScene(void)
{
}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{

	//// カメラモード：フリーカメラ
	//SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FREE);

	// グリッド線の初期化
	grid_ = new Grid();
	grid_->Init();

	// プレイヤーの初期化
	playerShip_ = new PlayerShip();
	playerShip_->Init();

	// NPC機の初期化
	npc_ = new NPC();
	npc_->Init();

	// スペースドームの初期化
	spaceDome_ = new SpaceDome(playerShip_->GetTransform());
	spaceDome_->Init();

	// カメラモード：追従
	Camera* camera = SceneManager::GetInstance().GetCamera();
	camera->SetFollow(&playerShip_->GetTransform());
	camera->ChangeMode(Camera::MODE::FOLLOW_SPRING);

	// スペースデブリ
	rockManager_ = new RockManager(playerShip_->GetTransform());
	rockManager_->Init();

	// ステージ
	stage_ = new Stage();
	stage_->Init();

	// 自機破壊演出時間
	stepShipDestroy_ = 0.0f;

	// 赤の強度（初期値は最大）
	redIntensity_ = 255;

	// 減らす数
	delta_ = -5;

}

void GameScene::Update(void)
{

	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(
			SceneManager::SCENE_ID::EVENT);
	}

	// グリッド線の更新
	grid_->Update();

	// スペースドームの更新
	spaceDome_->Update();

	// プレイヤーの更新
	playerShip_->Update();

	// NPC機の更新
	npc_->Update();

	// スペースデブリの更新
	rockManager_->Update();

	// ステージの更新
	stage_->Update();

	// 衝突判定
	Collision();

	// プレイヤーとの距離を求める
	VECTOR vec = VSub(npc_->GetTransform().pos, playerShip_->GetTransform().pos);
	int length = AsoUtility::Magnitude(vec);

	// 非アクティブ状態でもゲームループを動作し続ける
	SetAlwaysRunFlag(true);

}

void GameScene::Draw(void)
{

	// デバッグ
	VECTOR pos = stage_->GetBossTransform().pos;
	DrawSphere3D(pos, DIS_EVENT_AREA, 20, 0xff0000, 0xff0000, false);

	// グリッド線の描画
	grid_->Draw();

	// スカイドーム(背景や不透明を最初に描画)
	spaceDome_->Draw();

	// スペースデブリの描画
	rockManager_->Draw();

	// ステージの描画
	stage_->Draw();

	// キャラクター系の描画
	playerShip_->Draw();

	// NPC機の描画
	npc_->Draw();

	// スクリーン座標に直す
	auto pos2D = ConvWorldPosToScreenPos(npc_->GetTransform().pos);

	// プレイヤーとの距離を求める
	VECTOR vec = VSub(npc_->GetTransform().pos, playerShip_->GetTransform().pos);
	int length = AsoUtility::Magnitude(vec);

	// 距離の描画
	DrawFormatString(pos2D.x + 30, pos2D.y - 30, 0xff0000, "%d m", length);

	// 吹きだしのビルボード
	pos2D = ConvWorldPosToScreenPos(playerShip_->GetTransform().pos);
	DrawRotaGraph(pos2D.x + 200, pos2D.y - 200, 1.0f, 0.0f,
		static_cast<int>(ResourceManager::GetInstance().Load(ResourceManager::SRC::SPEECH_BALLOON).handleId_), true, false);

	// 追って！のメッセージ
	std::string msg = "追って！";
	SetFontSize(28);
	int len = (int)strlen(msg.c_str());
	int width = GetDrawStringWidth(msg.c_str(), len);
	DrawFormatString(pos2D.x + width + 25, pos2D.y - 225, 0x000000, msg.c_str());
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	SetFontSize(16);

	// プレイヤーとNPCの距離が離れすぎたら描画する
	if (length >= 1200)
	{
		// 画面を赤くする
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, redIntensity_);
		DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, GetColor(255, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// 強度を変化させる
		redIntensity_ += delta_;

		// 強度が最大または最小に達したら変化量を反転させる
		if (redIntensity_ <= 0 || redIntensity_ >= 255)
		{
			delta_ = -delta_;
		}

		// アラート文字の描画
		DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2,
			1.0f, 0.0f,
			static_cast<int>(ResourceManager::GetInstance().Load(ResourceManager::SRC::ALERT_MSG).handleId_), true);
	}
}

void GameScene::Release(void)
{

	// グリッド線の開放
	grid_->Release();
	delete grid_;

	// スペースドームの開放
	spaceDome_->Release();
	delete spaceDome_;

	// プレイヤーの開放
	playerShip_->Release();
	delete playerShip_;

	// NPC機の開放
	npc_->Release();
	delete npc_;

	// スペースデブリの開放
	rockManager_->Release();
	delete rockManager_;

	// ステージの開放
	stage_->Release();
	delete stage_;

}



void GameScene::Collision(void)
{

	// 自機とステージの当たり判定
	if (playerShip_->IsDestroy())
	{
		// 一定時間経過後、リスタート
		stepShipDestroy_ += SceneManager::GetInstance().GetDeltaTime();
		if (stepShipDestroy_ > TIME_RESTART)
		{
			SceneManager::GetInstance().ChangeScene(
				SceneManager::SCENE_ID::GAME);
		}
	}
	else
	{
		// ダンジョン(岩)
		auto info = MV1CollCheck_Sphere(stage_->GetModelIdStage(), -1,
			playerShip_->GetTransform().pos, PlayerShip::COLLISION_RADIUS);
		if (info.HitNum >= 1)
		{
			playerShip_->Destroy();
		}
		// 当たり判定結果ポリゴン配列の後始末をする
		MV1CollResultPolyDimTerminate(info);

		// プレイヤーとの距離を求める
		VECTOR vec = VSub(npc_->GetTransform().pos, playerShip_->GetTransform().pos);
		int length = AsoUtility::Magnitude(vec);

		// プレイヤーとNPCの距離が離れすぎたら自機を爆発させる
		if (length >= 3000)
		{
			playerShip_->Destroy();
		}
	}

	// イベントシーン突入エリアとの衝突判定
	VECTOR diff = VSub(
		stage_->GetBossTransform().pos,
		playerShip_->GetTransform().pos);
	float disPow = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
	if (disPow < DIS_EVENT_AREA * DIS_EVENT_AREA)
	{
		SceneManager::GetInstance().ChangeScene(
			SceneManager::SCENE_ID::EVENT);
	}

}