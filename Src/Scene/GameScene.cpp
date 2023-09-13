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

	// カメラモード：フリーカメラ
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FREE);

	grid_ = new Grid;
	grid_->Init();

	spaceDome_ = new SpaceDome;
	spaceDome_->Init();

	playerShip_ = new PlayerShip;
	playerShip_->Init();

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
