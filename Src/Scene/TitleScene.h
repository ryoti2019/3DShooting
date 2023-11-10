#pragma once
#include "SceneBase.h"
#include "../Object/Common/Transform.h"
#include <vector>
class SpaceDome;

class TitleScene : public SceneBase
{

public:

	// �e�L�X�g�̍\����
	struct TEXT 
	{
		std::vector<int> handle;
		VECTOR pos;
	};

	// �e�L�X�g�̐�
	static constexpr int NUM_TEXT = 60;

	// �R���X�g���N�^
	TitleScene(void);

	// �f�X�g���N�^
	~TitleScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	// �^�C�g�����S
	int imgTitleLogo_;

	// �X�y�[�X�h�[��
	Transform spaceDomeTrans_;

	// �X�J�C�h�[��
	SpaceDome* spaceDome_;

	// �\���^�̕ϐ�
	TEXT text_;

	// ���S�`��
	void DrawLogo(void);

};