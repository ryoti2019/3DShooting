#pragma once
#include "../../Object/Ship/ShotBase.h"

class ShotTurret : public ShotBase
{

public:

	// �R���X�g���N�^
	ShotTurret(void);

	// �f�X�g���N�^
	~ShotTurret(void);

	ShotTurret::STATE GetState(void);

	void SetState(ShotTurret::STATE state);

protected:

	// �p�����[�^�ݒ�
	void SetParam(void) override;

};

