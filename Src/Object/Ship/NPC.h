#pragma once
#include <vector>
#include <DxLib.h>
#include "../Common/Transform.h"

class PlayerShip;

class NPC
{
public:

	// �ړ��X�s�[�h
	static constexpr float SPEED_MOVE = 10.0f;

	// �������̃X�s�[�h
	static constexpr float BOOST_SPEED = 15.0f;

	// �Ǐ]�Ώۂ���G�t�F�N�g�܂ł̑��΍��W(���S�Ǐ])
	static constexpr VECTOR LOCAL_POS = { 0.0f, 0.0f, -20.0f };

	// �Ǐ]�Ώۂ���G�t�F�N�g�܂ł̑��΍��W(���S�Ǐ])
	static constexpr VECTOR LOCAL_DESTROY_POS = { 0.0f, 0.0f, -50.0f };

	// ���
	enum class STATE
	{
		NONE,
		RUN, // ���s���
		DESTROY// �j��
	};

	// �O�Ճf�[�^
	struct OrbitData
	{

		// �ǂ̕�����
		VECTOR dir;

		// ���񂷂鎞��
		float turnTime;

		// �O�i���鎞��
		float advanceTime;

		// ���񂷂鎞�Ԃ̔䗦
		float stepTime;

	};

	// �R���X�g���N�^
	NPC(void);

	// �f�X�g���N�^
	~NPC(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	// �v���C���[��Transform�̎擾
	const Transform& GetTransform(void) const;

private:

	// ���f������̊�{���
	Transform transform_;

	// �v���C���[
	PlayerShip* playerShip_;

	// �O�̕���
	VECTOR preOrbitDir_;

	// ���
	STATE state_;

	// �O�Ճf�[�^
	std::vector<OrbitData> orbitDatas_;

	// �f�[�^�̓Y����
	int data_;

	// ���˃G�t�F�N�g
	int effectJetResId_;
	int effectJetPlayId_;

	VECTOR effectPos_;

	// ���@�j��G�t�F�N�g
	int effectDestroyResId_;
	int effectDestroyPlayId_;

	VECTOR effectDestroyPos_;

	// �G�t�F�N�g�̍Đ�����
	float effectTime_;

	// �G�t�F�N�g�̃t���O
	bool isEffect_;

	// �G�t�F�N�g������
	void InitEffect(void);

	// �G�t�F�N�g����
	void SyncJetEffect(void);

	// ���j�G�t�F�N�g����
	void SyncDestroyEffect(void);

	// ��ԑJ��
	void ChangeState(STATE state);

	// ��ԕʍX�V�X�e�b�v
	void UpdateRun(void);
	void UpdateDestroy(void);

};

