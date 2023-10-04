#pragma once
#include "Common/Transform.h"

class PlayerShip
{

public:

	// �ړ��X�s�[�h
	static constexpr float SPEED_MOVE = 20.0f;

	// ���@�̉�]��
	static constexpr float SPEED_ROT_DEG_Y = 1.0f;
	static constexpr float SPEED_ROT_DEG_X = 1.0f;

	// �G�t�F�N�g���璍���_�܂ł̑��΍��W
	static constexpr VECTOR RELATIVE_F2T_POS = { 0.0f, 0.0f, 0.0f };

	// �Ǐ]�Ώۂ���G�t�F�N�g�܂ł̑��΍��W(���S�Ǐ])
	static constexpr VECTOR LOCAL_POS_L = { -5.0f, 0.0f, -20.0f };

	// �Ǐ]�Ώۂ���G�t�F�N�g�܂ł̑��΍��W(���S�Ǐ])
	static constexpr VECTOR LOCAL_POS_R = { 5.0f, 0.0f, -20.0f };

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

	// ���˃G�t�F�N�g
	int effectJetResId_;
	int effectJetLPlayId_;
	int effectJetRPlayId_;

	VECTOR effectLPos_;
	VECTOR effectRPos_;

	// �G�t�F�N�g������
	void InitEffect(void);

	// �G�t�F�N�g����
	void SyncJetEffect(void);

	// ����
	void ProcessTurn(void);

	void Turn(float deg, VECTOR axis);

};

