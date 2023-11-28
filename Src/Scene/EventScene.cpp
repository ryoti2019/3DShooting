#include <DxLib.h>
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/Camera.h"
#include "../Utility/AsoUtility.h"
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

	isBreak_ = false;

	breakScreen_ =
		MakeScreen(
			Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);

	MakebreakScreen();

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

	if (shotEvent_->GetState() == ShotEvent::STATE::BLAST)
	{
		isBreak_ = true;
	}

}

void EventScene::Draw(void)
{

	if (isBreak_)
	{
		SetDrawScreen(breakScreen_);
		ClearDrawScreen();
		SceneManager::GetInstance().GetCamera()->SetBeforeDraw();
	}

	spaceDome_->Draw();
	bossShip_->Draw();
	shotEvent_->Draw();

	// ��ʂ������� 
	//ClearDrawScreen();

	//DrawPolygon2D(vertexs_.data(), (int)vertexs_.size() / 3, breakScreen_, true);

	VECTOR prePos = { 0.0f,0.0f,0.0f };
	for (auto& v : vertexs_)
	{
		DrawLine(prePos.x, prePos.y, v.pos.x, v.pos.y, 0xff0000);
		prePos = v.pos;
	}

	//breakScreen_ =
	//	MakeScreen(
	//		Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);

	if (isBreak_)
	{
		// �|���S���̈ړ�
		for (int i = 0; i < vertexs_.size(); i += 3)
		{

			// �X�ɉ����x�I�ɏd�͂�������
			//gravityPow_ += SceneManager::GRAVITY / SceneManager::DEFAULT_FPS;
			gravityPow_ += 1 / SceneManager::DEFAULT_FPS;

			// Z����]�s��
			MATRIX rotz = MGetIdent();

			// ��ʂ̉E���̃|���S���͉E�΂߂Ɉړ�
			if (vertexs_[i + 1].pos.x > Application::SCREEN_SIZE_X / 2)
			{

				// X���W
				vertexs_[i].pos.x += POLYGON_X_POW;
				vertexs_[i + 1].pos.x += POLYGON_X_POW;
				vertexs_[i + 2].pos.x += POLYGON_X_POW;

				// Y���W
				vertexs_[i].pos.y -= POLYGON_Y_POW;
				vertexs_[i + 1].pos.y -= POLYGON_Y_POW;
				vertexs_[i + 2].pos.y -= POLYGON_Y_POW;

				rotz = MGetRotZ(AsoUtility::Deg2RadF(1));

			}

			// ��ʂ̍����̃|���S���͍��΂߂Ɉړ�
			if (vertexs_[i + 1].pos.x < Application::SCREEN_SIZE_X / 2)
			{

				// X���W
				vertexs_[i].pos.x -= POLYGON_X_POW;
				vertexs_[i + 1].pos.x -= POLYGON_X_POW;
				vertexs_[i + 2].pos.x -= POLYGON_X_POW;

				// Y���W
				vertexs_[i].pos.y -= POLYGON_Y_POW;
				vertexs_[i + 1].pos.y -= POLYGON_Y_POW;
				vertexs_[i + 2].pos.y -= POLYGON_Y_POW;

				rotz = MGetRotZ(AsoUtility::Deg2RadF(-1));

			}

			// Y���W�ɏd�͂�������
			vertexs_[i].pos = VAdd(vertexs_[i].pos, VScale({ 0.0f, 1.0f, 0.0f }, gravityPow_));
			vertexs_[i + 1].pos = VAdd(vertexs_[i + 1].pos, VScale({ 0.0f, 1.0f, 0.0f }, gravityPow_));
			vertexs_[i + 2].pos = VAdd(vertexs_[i + 2].pos, VScale({ 0.0f, 1.0f, 0.0f }, gravityPow_));

			// Z����]������
			// ���S���W�����߂�
			VECTOR centerPos;

			// �O�_�̒��S���W�����߂�
			centerPos = VAdd(vertexs_[i].pos, VAdd(vertexs_[i + 1].pos,  vertexs_[i + 2].pos));
			centerPos = VScale(centerPos, 1.0f / 3.0f);

			// ���΍��W�����
			VECTOR localPosA = VSub(vertexs_[i].pos,centerPos);
			VECTOR localPosB = VSub(vertexs_[i + 1].pos,centerPos);
			VECTOR localPosC = VSub(vertexs_[i + 2].pos,centerPos);

			// ���΍��W�̉�]
			localPosA = VTransform(localPosA, rotz);
			localPosB = VTransform(localPosB, rotz);
			localPosC = VTransform(localPosC, rotz);

			// ���[���h���W�ɕϊ�
			vertexs_[i].pos = VAdd(centerPos, localPosA);
			vertexs_[i + 1].pos = VAdd(centerPos, localPosB);
			vertexs_[i + 2].pos = VAdd(centerPos, localPosC);

		}

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

void EventScene::SetIsBreak(bool isBreak)
{
	isBreak_ = isBreak;
}

void EventScene::MakebreakScreen(void)
{

	const int cx = Application::SCREEN_SIZE_X / 2;
	const int cy = Application::SCREEN_SIZE_Y / 2;

	const float LENGTH = 1200.0f;
	VECTOR baseV = { 0.0f, -LENGTH, 0.0f };
	VECTOR sPos;
	VECTOR ePos;
	float radRot = 0.0f;
	MATRIX mat;

	// �p�x�̃��[�v(0.0f�`6.28f)
	while (radRot <= DX_TWO_PI_F)
	{

		// �w�肳�ꂽZ���̉�]�s������
		mat = MGetRotZ(radRot);

		// baseV��Y�������w�肵�A
		// Z����]�������邱�ƂŁA���v�̐j�̕����𓾂��
		sPos = VTransform(baseV, mat);

		// Z���̉�]�ʂ𑝂₷
		float degRot = static_cast<float>(10 + GetRand(20));
		radRot += degRot * DX_PI_F / 180.0f;

		// �ēx�AbaseV��Y�������w�肵�A
		// Z����]�������邱�ƂŁA���v�̐j�̕����𓾂��
		mat = MGetRotZ(radRot);
		ePos = VTransform(baseV, mat);

		sPos.x = max(0.0f, sPos.x);
		sPos.y = max(0.0f, sPos.y);
		sPos.x = min(Application::SCREEN_SIZE_X-1, sPos.x);
		sPos.y = min(Application::SCREEN_SIZE_Y-1, sPos.y);

		ePos.x = max(0.0f, ePos.x);
		ePos.y = max(0.0f, ePos.y);
		ePos.x = min(Application::SCREEN_SIZE_X-1, ePos.x);
		ePos.y = min(Application::SCREEN_SIZE_Y-1, ePos.y);

		VERTEX2D vertex;

		VECTOR cPos = { 0.0f, 0.0f, 0.0f };

		vertex.pos = { (float)cx, (float)cy, 0.0f };
		vertex.u = (float)cx / Application::SCREEN_SIZE_X;
		vertex.v = (float)cy / Application::SCREEN_SIZE_Y;
		vertex.dif = GetColorU8(255, 255, 255, 255);
		vertex.rhw = 1.0f;
		vertexs_.emplace_back(vertex);
		cPos = VAdd(cPos, vertex.pos);

		vertex.pos = { sPos.x, sPos.y, 0.0f };
		vertex.u = sPos.x / Application::SCREEN_SIZE_X;
		vertex.v = sPos.y / Application::SCREEN_SIZE_Y;
		vertex.dif = GetColorU8(255, 255, 255, 255);
		vertex.rhw = 1.0f;
		vertexs_.emplace_back(vertex);
		cPos = VAdd(cPos, vertex.pos);

		vertex.pos = { ePos.x, ePos.y, 0.0f };
		vertex.u = ePos.x / Application::SCREEN_SIZE_X;
		vertex.v = ePos.y / Application::SCREEN_SIZE_Y;
		vertex.dif = GetColorU8(255, 255, 255, 255);
		vertex.rhw = 1.0f;
		vertexs_.emplace_back(vertex);
		cPos = VAdd(cPos, vertex.pos);

		polygonCenterPos_.emplace_back(VScale(cPos, 1.0f / 3.0f));

	}

}
