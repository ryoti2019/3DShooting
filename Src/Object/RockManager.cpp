#include <DxLib.h>
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

	// �Ď��Ώۂ̍��W���}�b�v���W�ɕϊ�
	VECTOR monitorMapPos = target_.pos;
	monitorMapPos.x /= MAP_SIZE;
	monitorMapPos.y /= MAP_SIZE;
	monitorMapPos.z /= MAP_SIZE;

	// �ϊ����ꂽ�}�b�v���W�𒆐S��27�}�b�v����
	IntVector3 startMapPos = IntVector3(monitorMapPos);

	// �₪��������Ă��邩�m�F����p(�}�b�v���W)
	IntVector3 checkMapPos;
	startMapPos.Sub(1);


	// 27�}�b�v���̃��[�v
	const int loop = 3;
	for (int x = 0; x < loop; x++)
	{
		for (int y = 0; y < loop; y++)
		{
			for (int z = 0; z < loop; z++)
			{

				// �`�F�b�N����}�b�v���W�����
				checkMapPos = { x + startMapPos.x,
								y + startMapPos.y,
								z + startMapPos.z };

				// ���Ɋ�𐶐����Ă���}�b�v���W���`�F�b�N
				if (mapRocks_.count(checkMapPos) == 0)
				{

					std::vector<Rock*>tmpRocks;


					// ����w������A�����_���ɐ�������
					for (int i = 0; i < NUM_CREATE; i++)
					{
						// ���1��������
						tmpRocks.emplace_back(CreateRandom(checkMapPos));
					}

					// �}�b�v�A�z�z��ɃL�[�ƃo�����[���Z�b�g�Ɋi�[
					// �}�b�v�Ǘ��z��ɁA�}�b�v���W�Ɛ������������̊���i�[
					mapRocks_.emplace(checkMapPos, tmpRocks);

				}

			}
		}
	}

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

	// ���1���

	// ��̃��f���������_���Ɍ��߂�
	int modelId = 0;

	int Rand = GetRand(2);
	if (Rand == 0)
	{
		modelId = ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::ROCK01);
	}
	if (Rand == 1)
	{
		modelId = ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::ROCK02);
	}

	// �ʒu�������_����
	// ���[���h���W�ɕϊ�
	int randPosX = rand();
	int randPosY = rand();
	int randPosZ = rand();
	VECTOR pos;
	pos = { mapPos.x,mapPos.y,mapPos.z };
	//pos = { 0.0f,0.0f,0.0f };
	// �p�x�������_����
	int randAngleX = rand();
	int randAngleY = rand();
	int randAngleZ = rand();
	VECTOR angles;
	angles = { 1.0f * randAngleX,1.0f * randAngleY,1.0f * randAngleZ };
	//angles = { 1.0f,1.0f,1.0f };
	// �傫���������_����
	int randScaleX = rand();
	int randScaleY = rand();
	int randScaleZ = rand();
	VECTOR scale;
	scale = { 10.0f ,10.0f ,10.0f  };
	//scale = { 10.0f,10.0f,10.0f };
	// ��������Rock�N���X�̃C���X�^���X&���������s���A�Ԃ�l�ɕԂ�
	Rock* ret = new Rock();
	ret->Init(modelId, pos, angles, scale);

	return ret;

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
