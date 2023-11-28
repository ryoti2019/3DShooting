#pragma once
#include "ShotBase.h"
class Transform;
class EventScene;

class ShotEvent : public ShotBase
{

public:

	// �R���X�g���N�^
	ShotEvent(void);
	// �f�X�g���N�^
	~ShotEvent(void);

	ShotBase::STATE GetState(void);

protected:

	// �p�����[�^�ݒ�
	void SetParam(void) override;

	// �����`�F�b�N
	void CheckAlive(void) override;

};
