#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Utility/AsoUtility.h"
#include "PlayerShip.h"

PlayerShip::PlayerShip(void)
{
}

PlayerShip::~PlayerShip(void)
{
}

void PlayerShip::Init(void)
{

	// ���f������̊�{���
	transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::PLAYER_SHIP));
	float scale = 10.0f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = { 0.0f, 0.0f, 0.0f };
	transform_.quaRot = Quaternion::Euler(
		0.0f,
		AsoUtility::Deg2RadF(0.0f),
		0.0f
	);
	transform_.quaRotLocal = Quaternion();
	transform_.Update();

}

void PlayerShip::Update(void)
{

	auto& ins = InputManager::GetInstance();

	VECTOR moveDir = { 0.0f,0.0f,1.0f };

	// �������ꂽ�ړ��{�^���̕����Ɉړ�
	VECTOR direction = VNorm(rot_.PosAxis(moveDir)); // ��]������K�v������

	// �ړ���(���� * �X�s�[�h)
	VECTOR movePow = VScale(direction, SPEED_MOVE);

	// �ړ�(���W + �ړ���)
	// �J�����ʒu�ƃJ���������_
	transform_.pos = VAdd(transform_.pos, movePow);

	transform_.Update();

}

void PlayerShip::Draw(void)
{
	MV1DrawModel(transform_.modelId);
}

void PlayerShip::Release(void)
{
}
