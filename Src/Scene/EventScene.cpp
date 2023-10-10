#include <DxLib.h>
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Object/SpaceDome.h"
#include "../Object/BossShip.h"
#include "EventScene.h"

EventScene::EventScene(void)
{
}

EventScene::~EventScene(void)
{
}

void EventScene::Init(void)
{

	// カメラモード・位置を変える
	SceneManager::GetInstance().GetCamera()->ChangeMode(
		Camera::MODE::FIXED_POINT);
	// 背景(スカイドーム)
	spaceDome_ = new SpaceDome(spaceDomeTran_);
	spaceDome_->Init();
	bossShip_ = new BossShip();
	bossShip_->Init();

}

void EventScene::Update(void)
{

	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(
			SceneManager::SCENE_ID::EVENT);
	}
	spaceDome_->Update();
	bossShip_->Update();
}

void EventScene::Draw(void)
{
	spaceDome_->Draw();
	bossShip_->Draw();
}

void EventScene::Release(void)
{
	spaceDome_->Release();
	delete spaceDome_;
	bossShip_->Release();
	delete bossShip_;
}