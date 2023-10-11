#pragma once
#include "../Common/Transform.h"
class GameScene;

class PlayerShip
{

public:

	// �ړ��X�s�[�h
	static constexpr float SPEED_MOVE = 1.0f;

	// ���@�̉�]��
	static constexpr float SPEED_ROT_DEG_Y = 1.0f;
	static constexpr float SPEED_ROT_DEG_X = 1.0f;

	// ���@�j�󎞂���̃��X�^�[�g
	static constexpr float TIME_RESTART = 2.0f;

	// �G�t�F�N�g���璍���_�܂ł̑��΍��W
	static constexpr VECTOR RELATIVE_F2T_POS = { 0.0f, 0.0f, 0.0f };

	// �Ǐ]�Ώۂ���G�t�F�N�g�܂ł̑��΍��W(���S�Ǐ])
	static constexpr VECTOR LOCAL_POS_L = { -5.0f, 0.0f, -20.0f };

	// �Ǐ]�Ώۂ���G�t�F�N�g�܂ł̑��΍��W(���S�Ǐ])
	static constexpr VECTOR LOCAL_POS_R = { 5.0f, 0.0f, -20.0f };

	// �Ǐ]�Ώۂ���G�t�F�N�g�܂ł̑��΍��W(���S�Ǐ])
	static constexpr VECTOR LOCAL_BOOST_POS = { 0.0f, 0.0f, 500.0f };

	// �Ǐ]�Ώۂ���G�t�F�N�g�܂ł̑��΍��W(���S�Ǐ])
	static constexpr VECTOR LOCAL_DESTROY_POS = { 0.0f, 0.0f, -50.0f };

	// �Փ˔���F���̔��a
	static constexpr float COLLISION_RADIUS = 20.0f;

	// ���
	enum class STATE
	{
		NONE,
		RUN, // ���s���
		DESTROY// �j��
	};

	// ���@�j���Ԃ֑J��
	void Destroy(void);

	// �j���Ԃ�����
	bool IsDestroy(void) const;

	// �R���X�g���N�^
	PlayerShip(void);

	// �f�X�g���N�^
	~PlayerShip(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	// �v���C���[��Transform�̎擾
	const Transform& GetTransform(void) const;

private:

	// ���f������̊�{���
	Transform transform_;

	// ���
	STATE state_;

	// �Q�[���V�[��
	GameScene* gameScene_;

	// ���˃G�t�F�N�g
	int effectJetResId_;
	int effectJetLPlayId_;
	int effectJetRPlayId_;

	VECTOR effectLPos_;
	VECTOR effectRPos_;

	// �u�[�X�g���̉��Z���x
	float speedBoost_;

	// �u�[�X�g�g�p�Ԋu
	float deleyBoost_;

	// �u�[�X�g�G�t�F�N�g
	int effectBoostResId_;
	int effectBoostPlayId_;

	VECTOR effectBoostPos_;

	// ���@�j��G�t�F�N�g
	int effectDestroyResId_;
	int effectDestroyPlayId_;

	VECTOR effectDestroyPos_;

	// �G�t�F�N�g�̍Đ�����
	float effectTime_;

	// �G�t�F�N�g�̃t���O
	bool isEffect_;

	// ����F�ړ��u�[�X�g
	void ProcessBoost(void);

	// �G�t�F�N�g����
	void SyncBoostEffect(void);

	// �G�t�F�N�g������
	void InitEffect(void);

	// �G�t�F�N�g����
	void SyncJetEffect(void);

	// ���j�G�t�F�N�g����
	void SyncDestroyEffect(void);

	// ����
	void ProcessTurn(void);

	void Turn(float deg, VECTOR axis);

	// ��ԑJ��
	void ChangeState(STATE state);

	// ��ԕʍX�V�X�e�b�v
	void UpdateRun(void);
	void UpdateDestroy(void);

};

