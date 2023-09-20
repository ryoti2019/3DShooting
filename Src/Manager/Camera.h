#pragma once
#include <DxLib.h>
#include "../Common/Quaternion.h"

class Transform;

class Camera
{

public:

	// �J�����X�s�[�h�FNEAR
	static constexpr float SPEED = 20.0f;

	// �J�����N���b�v�FNEAR
	static constexpr float CAMERA_NEAR = 40.0f;

	// �J�����N���b�v�FNEAR
	static constexpr float CAMERA_FAR = 15000.0f;

	// �J�����̏������W
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 100.0f, -500.0f };

	// �J�����ʒu���璍���_�܂ł̑��΍��W
	static constexpr VECTOR RELATIVE_C2T_POS = { 0.0f, -100.0f, 500.0f };

	// �Ǐ]�Ώۂ���J�����ʒu�܂ł̑��΍��W(���S�Ǐ])
	static constexpr VECTOR RELATIVE_F2C_POS_FOLLOW = { 0.0f, 25.0f, -80.0f };

	// �Ǐ]�Ώۂ���J�����ʒu�܂ł̑��΍��W(�΂˕t��)
	static constexpr VECTOR RELATIVE_F2C_POS_SPRING = { 0.0f, 30.0f, -10.0f };

	// �J�������[�h
	enum class MODE
	{
		NONE,
		FIXED_POINT,	// ��_�J����
		FREE,			// �t���[���[�h
		FOLLOW,			// �Ǐ]���[�h
		FOLLOW_SPRING,	// �΂�
	};

	Camera(void);
	~Camera(void);

	void Init(void);
	void Update(void);
	void SetBeforeDraw(void);
	void SetBeforeDrawFixedPoint(void);
	void SetBeforeDrawFree(void);
	void SetBeforeDrawFollow(void);
	void SetBeforeDrawFollowSprnig(void);
	void Draw(void);
	void Release(void);

	VECTOR GetPos(void) const;

	// �Ǐ]�Ώۂ̐ݒ�
	void SetFollow(const Transform* follow);

	// �J�������[�h�̕ύX
	void ChangeMode(MODE mode);

private:

	// �J�������[�h
	MODE mode_;

	// �J�����̈ʒu
	VECTOR pos_;

	// �J�����̒����_
	VECTOR targetPos_;

	// �J�����̏����
	VECTOR cameraUp_;

	// �J�����̑��x
	VECTOR velocity_;

	// �J�����̉�]
	Quaternion rot_;

	// �J�����������ʒu�ɖ߂�
	void SetDefault(void);

	// �Ǐ]�Ώ�
	const Transform* followTransform_;

};

