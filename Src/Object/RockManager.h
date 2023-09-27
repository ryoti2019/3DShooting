#pragma once
#include <vector>
#include <map>
#include <DxLib.h>
#include "../Common/IntVector3.h"

class Rock;
class ResourceManager;
class Transform;

class RockManager
{

public:

	// 1�}�b�v������̃T�C�Y
	static constexpr int MAP_SIZE = 2000;

	// 1�}�b�v������ɐ��������̐�
	static constexpr int NUM_CREATE = 15;

	// �R���X�g���N�^
	RockManager(const Transform& target);

	// �f�X�g���N�^
	~RockManager(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

private:

	// �Ď��Ώ�
	const Transform& target_;
	
	// ��̃|�C���^
	Rock* rock_;

	// �}�b�v�Ǘ�
	// �L�[		�F�}�b�v���W
	// �o�����[	�F��̔z��(15��)
	std::map<IntVector3, std::vector<Rock*>> mapRocks_;

	// {0,0,0} =>��A�A��B�A��C
	// {1,0,0} =>��X�A��Y�A��Z

	// �����_������
	Rock* CreateRandom(IntVector3 mapPos);

	// �f�o�b�O�p�}�b�v�\��
	void DrawDebugMap(void);

};

