#pragma once
#include "../Object/Ship/ShotBase.h"

class ShotPlayer : public ShotBase
{

public:

	// �R���X�g���N�^
	ShotPlayer(void);

	// �f�X�g���N�^
	~ShotPlayer(void);

	ShotPlayer::STATE GetState(void);

protected:

	// �p�����[�^�ݒ�
	void SetParam(void) override;

};

