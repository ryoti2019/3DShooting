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

	// �Q�[���J�n���̃��O���Ȃ������߁A
	// �Ó]����27�}�b�v���쐬���Ă���
	Update();

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

	// ������x���@���₩�痣�ꂽ�烁�����ߖ�̂��ߍ폜
	IntVector3 intV;
	int len;

	// �폜���ׂ��L�[���ϒ��z��ɕϊ�����
	std::vector<IntVector3>deleteMapKeys;

	// ���݁A�������Ă���f�u���}�b�v�𑍃`�F�b�N
	for (const auto& p : mapRocks_)
	{

		// �Ď��Ώۂ���ǂꂭ�炢����Ă��邩�}�b�v�������o��
		intV = p.first;
		len = static_cast<int>(abs(static_cast<float>(intV.x) - monitorMapPos.x))
			+ static_cast<int>(abs(static_cast<float>(intV.y) - monitorMapPos.y))
			+ static_cast<int>(abs(static_cast<float>(intV.z) - monitorMapPos.z));

		// �T�ȏ�}�b�v���痣��Ă����烁����������
		if (len > 4)
		{
			// std::map�Ɋi�[����Ă���Rock���������J��
			for (auto rock : p.second)
			{
				rock->Release();
				delete rock;
			}
			// ��U�A�폜�ΏۂƂȂ��Ă���L�[��z��Ɋm��
			deleteMapKeys.emplace_back(p.first);
		}
	}

	for(const auto& key : deleteMapKeys)
	{
		// map(�A�z�z��)����Ώۂ̃L�[���폜����
		mapRocks_.erase(key);
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
	//DrawDebugMap();

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
	auto& ins = ResourceManager::GetInstance();

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
	// �O���[�o�����W�ɕϊ�
	VECTOR pos;
	pos.x = mapPos.x * MAP_SIZE;
	pos.y = mapPos.y * MAP_SIZE;
	pos.z = mapPos.z * MAP_SIZE;

	// -1000�`1000
	// (0�`2000) - 1000
	int hMapSize = MAP_SIZE / 2;
	int rX = GetRand(MAP_SIZE) - hMapSize;
	int rY = GetRand(MAP_SIZE) - hMapSize;
	int rZ = GetRand(MAP_SIZE) - hMapSize;
	pos.x += rX;
	pos.y += rY;
	pos.z += rZ;

	// �p�x�������_����
	VECTOR angles;
	angles.x = AsoUtility::Deg2RadF(GetRand(360));
	angles.y = AsoUtility::Deg2RadF(GetRand(360));
	angles.z = AsoUtility::Deg2RadF(GetRand(360));

	// �傫���������_����
	VECTOR scale;
	scale.x = 10.0f + AsoUtility::Deg2RadF(GetRand(100));
	scale.y = 10.0f + AsoUtility::Deg2RadF(GetRand(100));
	scale.z = 10.0f + AsoUtility::Deg2RadF(GetRand(100));
	
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
