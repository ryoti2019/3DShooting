#pragma once
#include <DxLib.h>
#include "Common/Transform.h"

class Rock
{

public:

	// �R���X�g���N�^
	Rock(void);

	// �f�X�g���N�^
	~Rock(void);

	// ��������
	void Init(int modelId, VECTOR pos, VECTOR angle, VECTOR scale);
	void Update(void);
	void Draw(void);
	void Release(void);

private:

	// ���f������̊�{���
	Transform transform_;

};
