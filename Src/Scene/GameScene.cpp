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

	//// �J�������[�h�F�t���[�J����
	//SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FREE);

	grid_ = new Grid();
	grid_->Init();

	playerShip_ = new PlayerShip();
	playerShip_->Init();

	spaceDome_ = new SpaceDome(playerShip_->GetTransform());
	spaceDome_->Init();

	// �J�������[�h�F�Ǐ]
	Camera* camera = SceneManager::GetInstance().GetCamera();
	camera->SetFollow(&playerShip_->GetTransform());
	camera->ChangeMode(Camera::MODE::FOLLOW_SPRING);

	// �X�y�[�X�f�u��
	rockManager_ = new RockManager(playerShip_->GetTransform());
	rockManager_->Init();

	// �X�e�[�W
	stage_ = new Stage();
	stage_->Init();

	// ���@�j�󉉏o����
	stepShipDestroy_ = 0.0f;

}

void GameScene::Update(void)
{

	// �V�[���J��
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

	// �Փ˔���
	Collision();

}

void GameScene::Draw(void)
{

	// �f�o�b�O
	VECTOR pos = stage_->GetBossTransform().pos;
	DrawSphere3D(pos, DIS_EVENT_AREA, 20, 0xff0000, 0xff0000, false);

	grid_->Draw();

	// �X�J�C�h�[��(�w�i��s�������ŏ��ɕ`��)
	spaceDome_->Draw();

	// �X�y�[�X�f�u��
	rockManager_->Draw();

	// �X�e�[�W
	stage_->Draw();

	// �L�����N�^�[�n
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

	// ���@�ƃX�e�[�W�̓����蔻��
	if (playerShip_->IsDestroy())
	{
		// ��莞�Ԍo�ߌ�A���X�^�[�g
		stepShipDestroy_ += SceneManager::GetInstance().GetDeltaTime();
		if (stepShipDestroy_ > TIME_RESTART)
		{
			SceneManager::GetInstance().ChangeScene(
				SceneManager::SCENE_ID::GAME);
		}
	}
	else
	{
		// �_���W����(��)
		auto info = MV1CollCheck_Sphere(stage_->GetModelIdStage(), -1,
			playerShip_->GetTransform().pos, PlayerShip::COLLISION_RADIUS);
		if (info.HitNum >= 1)
		{
			playerShip_->Destroy();
		}
		// �����蔻�茋�ʃ|���S���z��̌�n��������
		MV1CollResultPolyDimTerminate(info);
	}

	// �C�x���g�V�[���˓��G���A�Ƃ̏Փ˔���
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