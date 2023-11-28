#pragma once
#include <vector>
#include "SceneBase.h"
#include "../Object/Common/Transform.h"
class SpaceDome;
class BossShip;
class ShotEvent;

class EventScene : public SceneBase
{

public:

	// �|���S�����΂߂ɓ������Ƃ���Y���W�̋���
	static constexpr int POLYGON_X_POW = 3;

	// �|���S�����΂߂ɓ������Ƃ���Y���W�̋���
	static constexpr int POLYGON_Y_POW = 10;

	// �R���X�g���N�^
	EventScene(void);

	// �f�X�g���N�^
	~EventScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

	// ���ꂽ���ǂ����̃t���O�̐ݒ�
	void SetIsBreak(bool isBreak);

private:

	// �X�J�C�h�[���p
	Transform spaceDomeTran_;

	// �X�J�C�h�[��
	SpaceDome* spaceDome_;

	// �{�X
	BossShip* bossShip_;

	// �C�x���g�p
	ShotEvent* shotEvent_;

	// ���_���̍\����
	std::vector<VERTEX2D> vertexs_;

	std::vector<VECTOR> polygonCenterPos_;

	// �|���S������������
	VECTOR polygonDir_;

	// �|���S������������
	float polygonSpeed_;

	// �d�͉����x
	float gravityPow_;

	// �X�N���[���ɕ`�悵�Ă���e�N�X�`��
	int breakScreen_;

	// ��ꂽ���̃t���O
	bool isBreak_;

	// �X�N���[��������
	void MakebreakScreen(void);

};

