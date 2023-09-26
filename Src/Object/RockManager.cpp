#include "../Manager/ResourceManager.h"
#include "../Common/IntVector3.h"
#include "../Utility/AsoUtility.h"
#include "Common/Transform.h"
#include "Rock.h"
#include "RockManager.h"

RockManager::RockManager(const Transform& target) : target_(target)
{
	// �Ď��Ώۂ�ݒ�
}

RockManager::~RockManager(void)
{
}

void RockManager::Init(void)
{
}

void RockManager::Update(void)
{
}

void RockManager::Draw(void)
{

	// �}�b�v�A�z�z��̃��[�v
	// �A�z�z����̗v�f��͈�for���Ŏ��o��
	// ���o�����v�f�́A�L�[�ƃo�����[�ƂłQ��ނ���(pair)
	for (const auto& p : mapRocks_)
	{
		// map�A�z�z�񂩂���o�����v�f(pair)�́A
		// first�ŃL�[�ɃA�N�Z�X
		// second�Œl(�o�����[)�ɃA�N�Z�X
		// second�ɂ́ARock�N���X�̐��|�C���^�z�񂪓����Ă���̂ŁA
		// �Q�d���[�v
		for (const auto rock : p.second)
		{
			rock->Draw();
		}
	}
	
	// �f�o�b�O�p
	DrawDebugMap();

}

void RockManager::Release(void)
{

	for (auto& p : mapRocks_)
	{
		for (auto rock : p.second)
		{
			rock->Release();
			delete rock;
		}
		p.second.clear();
	}
	mapRocks_.clear();

}

Rock* RockManager::CreateRandom(IntVector3 mapPos)
{

	// ��̃��f���������_���Ɍ��߂�
	int modelId;

	int rand = GetRand(2);
	if (rand == 0)
	{
		modelId = static_cast<int>(ResourceManager::SRC::ROCK01);
	}
	if (rand == 1)
	{
		modelId = static_cast<int>(ResourceManager::SRC::ROCK02);
	}

	// �ʒu�������_����
	// ���[���h���W�ɕϊ�
	VECTOR pos;



	// �p�x�������_����
	VECTOR angles;

	// �傫���������_����
	VECTOR scale;

	// ��������Rock�N���X�̃C���X�^���X&���������s���A�Ԃ�l�ɕԂ�




	return nullptr;
}

void RockManager::DrawDebugMap(void)
{
	// �f�u���}�b�v
	int num = 4;
	float inumLen;
	float jnumLen;
	float lineS;
	float lineE;
	for (int i = 0; i < num; i++)
	{
		for (int j = 0; j < num; j++)
		{
			lineS = 0.0f;
			lineE = static_cast<float>(MAP_SIZE * (num - 1));
			inumLen = static_cast<float>(i * MAP_SIZE);
			jnumLen = static_cast<float>(j * MAP_SIZE);
			DrawLine3D({ lineS, inumLen, jnumLen },
				{ lineE, inumLen, jnumLen }, 0xffffff);
			DrawLine3D({ inumLen, lineS, jnumLen },
			{ inumLen, lineE, jnumLen }, 0xffffff);
			DrawLine3D({ inumLen, jnumLen, lineS },
				{ inumLen, jnumLen, lineE }, 0xffffff);
		}
	}
	float p = static_cast<float>((MAP_SIZE * (num - 1)) / 2);
	DrawSphere3D({ p, p, p }, 100, 10, 0xff0000, 0xff0000, true);

}
