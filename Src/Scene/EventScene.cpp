#include <DxLib.h>
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Application.h"
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

	isBreak_ = true;

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

	// ���S���W
	VECTOR centerPos;
	centerPos.x = Application::SCREEN_SIZE_X / 2;
	centerPos.y = Application::SCREEN_SIZE_Y / 2;

	// �X�N���[���T�C�Y�����傫�߂̐�(�x�N�g��)
	VECTOR vec;
	vec.y = -1000 - centerPos.y;

	// ���̃x�N�g������]������
	float radRot = GetRand(10.0f) + 10.0f;
	MATRIX mat = MGetRotZ(radRot);
	VECTOR epos = VTransform(vec, mat);

	// ���W����ʃT�C�Y���o�Ȃ��悤�ɂ���
	VECTOR sPos = { 0.0f,0.0f,0.0f };
	if (sPos.y <= 0)
	{
		sPos.y = 0;
	}
	if (sPos.y >= Application::SCREEN_SIZE_Y)
	{
		sPos.y = Application::SCREEN_SIZE_Y;
	}

	// ���̂R�̍��W�����ꂼ�꒸�_���W�A�P�|���S���Ƃ���
	for (auto vertex : vertexs_)
	{
		vertex = { sPos.x, sPos.y, 0.0f };
		vertex.u = sPos.x / Application::SCREEN_SIZE_X;
		vertex.v = sPos.y / Application::SCREEN_SIZE_Y;
		vertex.dif = GetColorU8(255, 255, 255, 255);
		vertex.rhw = 1.0f;
		vertexs_.emplace_back(vertex);
	}

	breakScreen_ =
		MakeScreen(
			Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);


	if (isBreak_)
	{
		SetDrawScreen(breakScreen_);
		ClearDrawScreen();
		SceneManager::GetInstance().GetCamera()->SetBeforeDraw();
	}

	if (isBreak_)
	{
		SetDrawScreen(DX_SCREEN_BACK);
		DrawPolygon2D(
			vertexs_.data(),
			(int)vertexs_.size() / 3,
			breakScreen_, true);
	}

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