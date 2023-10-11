#include <DxLib.h>
#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Object/SpaceDome.h"
#include "../Object/RockManager.h"
#include "../Object/Ship/PlayerShip.h"
#include "../Object/Ship/BossShip.h"
#include "BattleScene.h"

BattleScene::BattleScene(void)
{

	rockManager_ = nullptr;
	playerShip_ = nullptr;
	bossShip_ = nullptr;
	spaceDome_ = nullptr;

}

BattleScene::~BattleScene(void)
{
}

void BattleScene::Init(void)
{

	auto& manager = SceneManager::GetInstance();
	playerShip_ = new PlayerShip();
	playerShip_->Init();
	const Transform& trans = playerShip_->GetTransform();

	// 背景(スカイドーム)
	spaceDome_ = new SpaceDome(trans);
	spaceDome_->Init();
	// カメラを追従モードにする
	manager.GetCamera()->SetFollow(&trans);
	manager.GetCamera()->ChangeMode(Camera::MODE::FOLLOW_SPRING);
	rockManager_ = new RockManager(trans);
	rockManager_->Init();
	// ボス
	bossShip_ = new BossShip();
	bossShip_->Init();

}

void BattleScene::Update(void)
{

	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
	bossShip_->Update();
	playerShip_->Update();
	rockManager_->Update();
	spaceDome_->Update();

}

void BattleScene::Draw(void)
{

	// 不透明
	spaceDome_->Draw();
	rockManager_->Draw();
	bossShip_->Draw();
	playerShip_->Draw();

}

void BattleScene::Release(void)
{

	playerShip_->Release();
	delete playerShip_;
	bossShip_->Release();
	delete bossShip_;
	spaceDome_->Release();
	delete spaceDome_;
	rockManager_->Release();
	delete rockManager_;

}