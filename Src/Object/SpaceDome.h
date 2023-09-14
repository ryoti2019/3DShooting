#pragma once
#include "../Object/Common/Transform.h"

class SpaceDome

{

public:

	// �R���X�g���N�^
	SpaceDome(const Transform& follow);

	// �f�X�g���N�^
	~SpaceDome(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

private:

	// �Ǐ]�Ώ�
	const Transform& follow_;

	// ���f������̊�{���
	Transform transform_;

};

