#pragma once
#include "../Object/Common/Transform.h"
class SpaceDome
{

public:

	// �R���X�g���N�^
	SpaceDome(void);

	// �f�X�g���N�^
	~SpaceDome(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

private:
	
	// ���f������̊�{���
	Transform transform_;

};

