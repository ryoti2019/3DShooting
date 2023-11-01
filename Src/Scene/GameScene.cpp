#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Object/RockManager.h"
#include "../Object/Grid.h"
#include "../Object/SpaceDome.h"
#include "../Object/Ship/PlayerShip.h"
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

	grid_ = new Grid();
	grid_->Init();

	playerShip_ = new PlayerShip();
	playerShip_->Init();

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

	grid_->Update();

	spaceDome_->Update();

	playerShip_->Update();

	rockManager_->Update();

	stage_->Update();

	// 衝突判定
	Collision();

}

void GameScene::Draw(void)
{

	// デバッグ
	VECTOR pos = stage_->GetBossTransform().pos;
	DrawSphere3D(pos, DIS_EVENT_AREA, 20, 0xff0000, 0xff0000, false);

	grid_->Draw();

	// スカイドーム(背景や不透明を最初に描画)
	spaceDome_->Draw();

	// スペースデブリ
	rockManager_->Draw();

	// ステージ
	stage_->Draw();

	// キャラクター系
	playerShip_->Draw();

}

void GameScene::Release(void)
{

	grid_->Release();
	delete grid_;

	spaceDome_->Release();
	delete spaceDome_;

	playerShip_->Release();
	delete playerShip_;

	rockManager_->Release();
	delete rockManager_;

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