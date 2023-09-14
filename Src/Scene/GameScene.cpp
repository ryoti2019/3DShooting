#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
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
	camera->ChangeMode(Camera::MODE::FOLLOW);

}

void GameScene::Update(void)
{

	grid_->Update();

	spaceDome_->Update();

	playerShip_->Update();

}

void GameScene::Draw(void)
{

	grid_->Draw();

	spaceDome_->Draw();

	playerShip_->Draw();

}

void GameScene::Release(void)
{

	grid_->Release();

	spaceDome_->Release();

	playerShip_->Release();

}
