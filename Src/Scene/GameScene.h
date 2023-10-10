#pragma once
#include "SceneBase.h"

class Grid;
class SpaceDome;
class PlayerShip;
class RockManager;
class Stage;

class GameScene : public SceneBase
{

public:

	// ���@�j�󎞂���̃��X�^�[�g
	static constexpr float TIME_RESTART = 2.0f;

	// ��͂𒆐S�Ƃ����C�x���g�V�[���˓��G���A�̋���
	static constexpr float DIS_EVENT_AREA = 8000.0f;

	// �R���X�g���N�^
	GameScene(void);

	// �f�X�g���N�^
	~GameScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

	// ���@�j�󎞊ԉ��o���Ԃ̎擾
	float GetStepShipDestroy(void);

private:

	// �X�J�C�h�[��
	SpaceDome* spaceDome_;

	// �O���b�h��
	Grid* grid_;

	// �v���C���[
	PlayerShip* playerShip_;

	// �w�i�̃X�y�[�X�f�u��
	RockManager* rockManager_;

	// �X�e�[�W
	Stage* stage_;

	// ���@�j�󉉏o����
	float stepShipDestroy_;

	// �Փ˔���
	void Collision(void);

};
