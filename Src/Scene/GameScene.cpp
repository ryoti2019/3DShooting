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

	//// �J�������[�h�F�t���[�J����
	//SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FREE);

	// �O���b�h���̏�����
	grid_ = new Grid();
	grid_->Init();

	// �v���C���[�̏�����
	playerShip_ = new PlayerShip();
	playerShip_->Init();

	// NPC�@�̏�����
	npc_ = new NPC();
	npc_->Init();

	// �X�y�[�X�h�[���̏�����
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

	// �Ԃ̋��x�i�����l�͍ő�j
	redIntensity_ = 255;

	// ���炷��
	delta_ = -5;

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

	// �O���b�h���̍X�V
	grid_->Update();

	// �X�y�[�X�h�[���̍X�V
	spaceDome_->Update();

	// �v���C���[�̍X�V
	playerShip_->Update();

	// NPC�@�̍X�V
	npc_->Update();

	// �X�y�[�X�f�u���̍X�V
	rockManager_->Update();

	// �X�e�[�W�̍X�V
	stage_->Update();

	// �Փ˔���
	Collision();

	// �v���C���[�Ƃ̋��������߂�
	VECTOR vec = VSub(npc_->GetTransform().pos, playerShip_->GetTransform().pos);
	int length = AsoUtility::Magnitude(vec);

	// ��A�N�e�B�u��Ԃł��Q�[�����[�v�𓮍삵������
	SetAlwaysRunFlag(true);

}

void GameScene::Draw(void)
{

	// �f�o�b�O
	VECTOR pos = stage_->GetBossTransform().pos;
	DrawSphere3D(pos, DIS_EVENT_AREA, 20, 0xff0000, 0xff0000, false);

	// �O���b�h���̕`��
	grid_->Draw();

	// �X�J�C�h�[��(�w�i��s�������ŏ��ɕ`��)
	spaceDome_->Draw();

	// �X�y�[�X�f�u���̕`��
	rockManager_->Draw();

	// �X�e�[�W�̕`��
	stage_->Draw();

	// �L�����N�^�[�n�̕`��
	playerShip_->Draw();

	// NPC�@�̕`��
	npc_->Draw();

	// �X�N���[�����W�ɒ���
	auto pos2D = ConvWorldPosToScreenPos(npc_->GetTransform().pos);

	// �v���C���[�Ƃ̋��������߂�
	VECTOR vec = VSub(npc_->GetTransform().pos, playerShip_->GetTransform().pos);
	int length = AsoUtility::Magnitude(vec);

	// �����̕`��
	DrawFormatString(pos2D.x + 30, pos2D.y - 30, 0xff0000, "%d m", length);

	// ���������̃r���{�[�h
	pos2D = ConvWorldPosToScreenPos(playerShip_->GetTransform().pos);
	DrawRotaGraph(pos2D.x + 200, pos2D.y - 200, 1.0f, 0.0f,
		static_cast<int>(ResourceManager::GetInstance().Load(ResourceManager::SRC::SPEECH_BALLOON).handleId_), true, false);

	// �ǂ��āI�̃��b�Z�[�W
	std::string msg = "�ǂ��āI";
	SetFontSize(28);
	int len = (int)strlen(msg.c_str());
	int width = GetDrawStringWidth(msg.c_str(), len);
	DrawFormatString(pos2D.x + width + 25, pos2D.y - 225, 0x000000, msg.c_str());
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	SetFontSize(16);

	// �v���C���[��NPC�̋��������ꂷ������`�悷��
	if (length >= 1200)
	{
		// ��ʂ�Ԃ�����
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, redIntensity_);
		DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, GetColor(255, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// ���x��ω�������
		redIntensity_ += delta_;

		// ���x���ő�܂��͍ŏ��ɒB������ω��ʂ𔽓]������
		if (redIntensity_ <= 0 || redIntensity_ >= 255)
		{
			delta_ = -delta_;
		}

		// �A���[�g�����̕`��
		DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2,
			1.0f, 0.0f,
			static_cast<int>(ResourceManager::GetInstance().Load(ResourceManager::SRC::ALERT_MSG).handleId_), true);
	}
}

void GameScene::Release(void)
{

	// �O���b�h���̊J��
	grid_->Release();
	delete grid_;

	// �X�y�[�X�h�[���̊J��
	spaceDome_->Release();
	delete spaceDome_;

	// �v���C���[�̊J��
	playerShip_->Release();
	delete playerShip_;

	// NPC�@�̊J��
	npc_->Release();
	delete npc_;

	// �X�y�[�X�f�u���̊J��
	rockManager_->Release();
	delete rockManager_;

	// �X�e�[�W�̊J��
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

		// �v���C���[�Ƃ̋��������߂�
		VECTOR vec = VSub(npc_->GetTransform().pos, playerShip_->GetTransform().pos);
		int length = AsoUtility::Magnitude(vec);

		// �v���C���[��NPC�̋��������ꂷ�����玩�@�𔚔�������
		if (length >= 3000)
		{
			playerShip_->Destroy();
		}
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