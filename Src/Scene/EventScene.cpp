#include <DxLib.h>
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Object/SpaceDome.h"
#include "../Object/Ship/BossShip.h"
#include "../Object/Ship/ShotEvent.h"
#include "EventScene.h"

EventScene::EventScene(void)
{
}

EventScene::~EventScene(void)
{
}

void EventScene::Init(void)
{

	// �J�������[�h�E�ʒu��ς���
	SceneManager::GetInstance().GetCamera()->ChangeMode(
		Camera::MODE::FIXED_POINT);

	// �w�i(�X�J�C�h�[��)
	spaceDome_ = new SpaceDome(spaceDomeTran_);
	spaceDome_->Init();

	// �{�X���
	bossShip_ = new BossShip();
	bossShip_->Init();

	// �C�x���g�p�̒e
	shotEvent_ = new ShotEvent();
	VECTOR cameraPos = SceneManager::GetInstance().GetCamera()->GetPos();
	VECTOR bossPos = bossShip_->GetTransform().pos;

	// �{�X��͂���J�����ւ̕���
	VECTOR dir = VNorm(VSub(cameraPos, bossPos));
	shotEvent_->Create(bossPos, dir);

}

void EventScene::Update(void)
{

	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(
			SceneManager::SCENE_ID::BATTLE);
	}

	spaceDome_->Update();
	bossShip_->Update();
	shotEvent_->Update();

	// �e���I�����Ă���A�J�����̃V�F�C�N���[�h���I�������A
	// �o�g���V�[���ֈڍs����
	if (shotEvent_->GetState() == ShotEvent::STATE::END
		&& SceneManager::GetInstance().GetCamera()->
		GetMode() != Camera::MODE::SHAKE)
	{
		SceneManager::GetInstance().ChangeScene(
			SceneManager::SCENE_ID::BATTLE);
	}

}

void EventScene::Draw(void)
{

	spaceDome_->Draw();
	bossShip_->Draw();
	shotEvent_->Draw();

}

void EventScene::Release(void)
{

	spaceDome_->Release();
	delete spaceDome_;
	bossShip_->Release();
	delete bossShip_;
	shotEvent_->Release();
	delete shotEvent_;

}