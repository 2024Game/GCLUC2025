#include "Shadow1.h"

#define CHIP_SIZE_W 259		// 1�R�}�̃T�C�Y(��)
#define CHIP_SIZE_H 259     // 1�R�}�̃T�C�Y(����)
#define CENTER_POS CVector2D(-100.0f, 300.0f)	// ���S���W

// �X���C���̃A�j���[�V�����f�[�^
TexAnimData Shadow1::ANIM_DATA[(int)EAnimType::Num] =
{
	{
		new TexAnim[4]
		{
			{ 0, 10}, { 0, 10},
			{ 0, 10}, { 0, 10},
		},
		4
	},
	{
		new TexAnim[2]
		{
			{7, 10}, {8, 10},
		},
		2
	},
};

// �R���X�g���N�^
Shadow1::Shadow1(int type, const CVector3D& pos)
	: EnemyBase(pos)
	, mp_image(nullptr)
	, m_type(type)
{
	m_hp = 200;

	// �X���C���̉摜��ǂݍ���
	std::string imagePath;
	if (m_type == 0) imagePath = "shadow.png";
	else if (m_type == 1) imagePath = "slime_b.png";
	else if (m_type == 2) imagePath = "slime_c.png";
	mp_image = CImage::CreateImage
	(
		imagePath.c_str(),	// �摜�t�@�C���̃p�X
		ANIM_DATA,			// �A�j���[�V�����̃f�[�^
		CHIP_SIZE_W, CHIP_SIZE_H		// 1�R�}�̕��ƍ���
	);
	mp_image->ChangeAnimation(0);
	mp_image->SetCenter(CENTER_POS);
	mp_image->SetFlipH(true);
}

Shadow1::~Shadow1()
{
	// �摜�f�[�^���폜
	delete mp_image;
}

void Shadow1::Death()
{
}

extern float testScrollX; // �v���C���[�Ǐ]

void Shadow1::Update()
{
	// ��L�[���������u�Ԃ�Z���W�����炷�i���ֈړ��j
	if (PUSH(CInput::eUp) && m_pos.z > -230.0f)
	{
		m_pos.z -= 150.0f;
	}
	// ���L�[���������u�Ԃ�Z���W�𑝂₷�i��O�ֈړ��j
	else if (PUSH(CInput::eDown) && m_pos.z < 0.0f)
	{
		m_pos.z += 150.0f;
	}

	// ��Ԃɍ��킹�āA�X�V������؂�ւ���
	switch (m_state)
	{
	case EState::Idle:	StateIdle();	break;
	case EState::Action:	StateAction();	break;
	}

	m_pos.x = testScrollX; // �v���C���[�̈ʒu�ɂ���ĉe�ړ�

	// �C���[�W�ɍ��W��ݒ肵�āA�A�j���[�V�������X�V
	mp_image->SetPos(CalcScreenPos());
	mp_image->UpdateAnimation();
}

void Shadow1::Render()
{
	mp_image->Draw();
}

void Shadow1::ChangeState(EState state)
{
	if (m_state == state) return;

	m_state = state;
	m_stateStep = 0;
}

void Shadow1::StateIdle()
{
	// �ҋ@�A�j���[�V�������Đ�
	mp_image->ChangeAnimation((int)EAnimType::Idle);
}

void Shadow1::StateAction()
{
	// �X�e�b�v���Ƃɏ�����؂�ւ�
	switch (m_stateStep)
	{
		// �X�e�b�v0�F���܂ꂽ�A�j���[�V�������Đ�
	case 0:
		mp_image->ChangeAnimation((int)EAnimType::Action, false);
		m_stateStep++;
		break;
		// �X�e�b�v1�F�A�j���[�V�����I���҂�
	case 1:
		/* �ꎞ�I�ɍ폜
		// �A�j���[�V�������I��������A�폜
		if (mp_image->CheckAnimationEnd())
		{
			// �폜�t���O�𗧂Ă�
			Kill();
		}*/
		break;
	}
}


