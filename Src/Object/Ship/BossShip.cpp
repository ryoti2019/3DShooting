#include <EffekseerForDXLib.h>
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Utility/AsoUtility.h"
#include "../../Object/Ship/Turret.h"
#include "BossShip.h"

BossShip::BossShip(void)
{
}

BossShip::~BossShip(void)
{
}

void BossShip::Init(void)
{

	// �������n
	VECTOR pos = { 0.0f, 0.0f, 4000.0f };

	// ���f������̊�{���
	transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::BOSS_SHIP));

	float scale = 2.0f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = pos;
	transform_.quaRot = Quaternion::Euler(
		AsoUtility::Deg2RadF(20.0f),
		AsoUtility::Deg2RadF(210.0f),
		AsoUtility::Deg2RadF(0.0f)
	);
	transform_.quaRotLocal = Quaternion::Euler(
		AsoUtility::Deg2RadF(0.0f),
		AsoUtility::Deg2RadF(180.0f),
		AsoUtility::Deg2RadF(0.0f)
	);
	// ���f������̊�{���X�V
	transform_.Update();

	// �V�[���ɂ���ď�Ԃ�؂�ւ���
	SceneManager::SCENE_ID sceneId = SceneManager::GetInstance().GetSceneID();
	switch (sceneId)
	{
	case SceneManager::SCENE_ID::EVENT:
		ChangeState(STATE::EVENT);
		break;
	case SceneManager::SCENE_ID::BATTLE:
		// �����蔻��(�R���C�_)�쐬
		//int ret = MV1SetupCollInfo(transform_.modelId, -1, 1, 1, 1);
		// �~�������x�� ������
		MV1RefreshCollInfo(transform_.modelId, -1);
		ChangeState(STATE::BATTLE);
		break;
	}

	// �j��G�t�F�N�g
	effectDestroyResId_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::PLAYER_SHIP_EXPLOSION).handleId_;

	// �^���b�g�̑S�Ńt���O
	isAllDestroyedTurrets_ = false;

	// �t�F�[�h����
	stepFade_ = TIME_FADE;

	// �O��
	MakeTurret(
		{ 4.5f, 5.5f, 7.8f },
		{ 0.0f, 0.0f, AsoUtility::Deg2RadF(-18.0f) });
	MakeTurret(
		{ -4.5f, 5.5f, 7.8f },
		{ 0.0f, 0.0f, AsoUtility::Deg2RadF(18.0f) });

	// ��
	MakeTurret(
		{ 4.5f, 5.5f, 0.0f },
		{ AsoUtility::Deg2RadF(20.0f), AsoUtility::Deg2RadF(90.0f), 0.0f });
	MakeTurret(
		{ -4.5f, 5.5f, 0.0f },
		{ AsoUtility::Deg2RadF(20.0f), AsoUtility::Deg2RadF(-90.0f), 0.0f });

	// ���
	MakeTurret(
		{ 3.5f, 5.0f, -17.8f },
		{ 0.0f, AsoUtility::Deg2RadF(180.0f), AsoUtility::Deg2RadF(18.0f) });
	MakeTurret(
	{ -3.5f, 5.0f, -17.8f },
		{ 0.0f, AsoUtility::Deg2RadF(180.0f), AsoUtility::Deg2RadF(-18.0f) });

}
void BossShip::Update(void)
{

	VECTOR forward = transform_.GetForward();

	// �ړ�
	transform_.pos = VAdd(transform_.pos, VScale(forward, SPEED_MOVE));

	// ���f������̊�{���X�V
	transform_.Update();

	switch (state_)
	{
	case BossShip::STATE::NONE:
		break;
	case BossShip::STATE::EVENT:
		MoveForward();
		// �^���b�g
		UpdateTurret();
		break;
	case BossShip::STATE::BATTLE:
		Rotation();
		MoveForward();
		// �����蔻��(�R���C�_)�쐬
		//int ret = MV1SetupCollInfo(transform_.modelId, -1, 1, 1, 1);
		// �~�������x�� ������
		MV1RefreshCollInfo(transform_.modelId, -1);
		// �^���b�g
		UpdateTurret();
		if (isAllDestroyedTurrets_)
		{
			// �^���b�g���S�ł�����A�{�X���j�󂷂�
			ChangeState(STATE::DESTROY);
			return;
		}
		break;
	case BossShip::STATE::DESTROY:
		// �^���b�g
		UpdateTurret();
		// �j�󎞂̋@�̃t�F�[�h
		stepFade_ -= SceneManager::GetInstance().GetDeltaTime();
		if (stepFade_ < 0.0f)
		{
			ChangeState(STATE::END);
			return;
		}
		// �j�󎞂̃G�t�F�N�g
		stepExplosion_ -= SceneManager::GetInstance().GetDeltaTime();
		if (stepExplosion_ < 0.0f)
		{
			CreateRandomExplosion();
			stepExplosion_ = TIME_EXPLOSION;
		}
		break;
	case BossShip::STATE::END:
		break;
	}

}

void BossShip::Draw(void)
{

	switch (state_)
	{
	case BossShip::STATE::NONE:
		break;
	case BossShip::STATE::EVENT:
	case BossShip::STATE::BATTLE:
		MV1DrawModel(transform_.modelId);
		DrawTurret();
		break;
	case BossShip::STATE::DESTROY:
	{
		// ���Ԃɂ��F�̐��`���
		float diff = TIME_FADE - stepFade_;
		auto c = AsoUtility::Lerp(FADE_C_FROM, FADE_C_TO, (diff / TIME_FADE));
		// ���f���̃}�e���A�����擾
		int num = MV1GetMaterialNum(transform_.modelId);
		for (int i = 0; i < num; i++)
		{
			// ���f���̃f�B�t���[�Y�J���[��ύX
			MV1SetMaterialDifColor(transform_.modelId, i, c);
		}
		// ���f���̕`��
		MV1DrawModel(transform_.modelId);
		break;
	}
	case BossShip::STATE::END:
		break;
	}
}

void BossShip::Release(void)
{

	for (auto& t : turrets_)
	{
		t->Release();
		delete t;
	}
	turrets_.clear();

	for (int playId : effectDestroyPlayIds_)
	{
		// �j��G�t�F�N�g��~
		StopEffekseer3DEffect(playId);
	}

}

Transform BossShip::GetTransform(void)
{
	return transform_;
}

bool BossShip::IsAlive(void) const
{
	return state_ == STATE::BATTLE;
}

int BossShip::GetModelIdBossShip(void)
{
	return transform_.modelId;
}

const std::vector<Turret*>& BossShip::GetTurrets(void) const
{
	return turrets_;
}

bool BossShip::IsDestroy(void) const
{
	return state_ == STATE::DESTROY;
}

bool BossShip::IsEnd(void) const
{
	return state_ == STATE::END;
}

BossShip::STATE BossShip::GetState(void)
{
	return state_;
}

void BossShip::ChangeState(STATE state)
{
	state_ = state;
}

void BossShip::MoveForward(void)
{

	// �G�̑O�������擾
	VECTOR forward = transform_.GetForward();

	// �ړ�
	transform_.pos = VAdd(transform_.pos, VScale(forward, SPEED_MOVE));

}

void BossShip::Rotation(void)
{
	// �����Ŏw�肳�ꂽ��]�ʁE�����A��]��������

	// �@�f�O���[�����W�A���ɕϊ�����
	float rad = AsoUtility::Deg2RadF(ROT_POW_DEG * SceneManager::GetInstance().GetDeltaTime());

	// �A���W�A�����N�H�[�^�j�I���ɕϊ�
	// (�Ƃ���֐��ŁA��]��RAD�Ɖ�]�����g�p)
	Quaternion rotPow = Quaternion::AngleAxis(rad, AsoUtility::AXIS_Y);

	// �B����쐬������]�ʂ��A���@�̉�]�ʂɉ�����(��������)
	transform_.quaRot = transform_.quaRot.Mult(rotPow);
}

void BossShip::UpdateTurret(void)
{

	int alived = 0;

	for (auto t : turrets_)
	{
		t->Update();

		if (t->IsAlive())
		{
			alived++;
		}

	}

	// �^���b�g���P�@���������Ă��Ȃ����
	if (alived == 0)
	{
		isAllDestroyedTurrets_ = true;
	}

}

void BossShip::DrawTurret(void)
{

	for (auto t : turrets_)
	{
		t->Draw();
	}

}

void BossShip::MakeTurret(VECTOR localPos, VECTOR localAddAxis)
{

	// �^���b�g�̎��̂��쐬
	Turret* turret = new Turret(transform_, localPos, localAddAxis);
	// ������
	turret->Init();
	turrets_.push_back(turret);

}

void BossShip::CreateRandomExplosion(void)
{

	// �ȉ~����Ƀ����_���ȍ��W���쐬����
	for (int i = 0; i < 2; i++)
	{

		// XYZ�̃����_���Ȋp�x
		float x = static_cast<float>(GetRand(360));
		float y = static_cast<float>(GetRand(360));
		float z = static_cast<float>(GetRand(360));

		// ���W�A���ɕϊ�
		x = AsoUtility::Deg2RadF(x);
		y = AsoUtility::Deg2RadF(y);
		z = AsoUtility::Deg2RadF(z);

		// �N�H�[�^�j�I���ɕϊ�
		Quaternion randRot = Quaternion::Euler(x, y, z);

		// �����ɕϊ�
		VECTOR dir = randRot.PosAxis({ 0.0f, 0.0f , 1.0f });

		// �����ɑ΂��āA�ȉ~��XYZ�̔��a��������L�΂�
		// �����ŁA���a�������_���ɂ���ƁA�ȉ~����ł͂Ȃ��A�ȉ~�̏�̍��W�ɂȂ�
		VECTOR pos = { 0.0f, 0.0f, 0.0f };
			   pos = VAdd(pos, VScale({ dir.x, 0.0f, 0.0f }, COLLISION_ELLIPSOID_RADIUS_X));
			   pos = VAdd(pos, VScale({ 0.0f, dir.y, 0.0f }, COLLISION_ELLIPSOID_RADIUS_Y));
			   pos = VAdd(pos, VScale({ 0.0f, 0.0f, dir.z }, COLLISION_ELLIPSOID_RADIUS_Z));

		// �������ꂽ���W�̓{�X��͂���̑��΍��W�ɂȂ��Ă���̂ŁA
		// ��]��������ŁA���[���h���W�ɒ���
		// ��]���
		Quaternion shipRot = transform_.quaRot;

		// ���΍��W����]
		VECTOR localRotPos = shipRot.PosAxis(pos);

		// ���[���h���W�ɕϊ�(�O�̂��߁ASCALE 1)
		pos = VAdd(transform_.pos, VScale(localRotPos, 1.0f));

		// �j��G�t�F�N�g����
		int playId = PlayEffekseer3DEffect(effectDestroyResId_);

		// �傫��
		SetScalePlayingEffekseer3DEffect(playId, 50.0f, 50.0f, 50.0f);

		// �ʒu
		SetPosPlayingEffekseer3DEffect(playId, pos.x, pos.y, pos.z);

		// �p�x
		VECTOR angles = transform_.quaRot.ToEuler();
		SetRotationPlayingEffekseer3DEffect(playId, angles.x, angles.y, angles.z);

		// ���I�z��ɒǉ�
		effectDestroyPlayIds_.push_back(playId);

	}

}
