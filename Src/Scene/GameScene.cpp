#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Object/RockManager.h"
#include "../Object/Grid.h"
#include "../Object/SpaceDome.h"
#include "../Object/PlayerShip.h"
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

}

void GameScene::Update(void)
{

	grid_->Update();

	spaceDome_->Update();

	playerShip_->Update();

	rockManager_->Update();

}

void GameScene::Draw(void)
{

	grid_->Draw();

	// スカイドーム(背景や不透明を最初に描画)
	spaceDome_->Draw();

	// スペースデブリ
	rockManager_->Draw();

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

}
