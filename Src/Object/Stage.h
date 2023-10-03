#pragma once
#include <DxLib.h>
#include "Common/Transform.h"

class Stage
{

public:

	// �R���X�g���N�^
	Stage(void);

	// �f�X�g���N�^
	~Stage(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

private:

	// �X�e�[�W
	Transform stage_;

	// �{�X���
	Transform bossShip_;

};

