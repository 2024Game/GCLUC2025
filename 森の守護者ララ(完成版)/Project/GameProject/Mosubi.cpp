#include "Mosubi.h"

#define CHIP_SIZE_W 400		// 1�R�}�̃T�C�Y(��)
#define CHIP_SIZE_H 400     // 1�R�}�̃T�C�Y(����)
//#define CENTER_POS CVector2D(128.0f, 340.0f)	// ���S���W
#define CENTER_POS CVector2D(0.0f, 300.0f)	// ���S���W

// ���X�r�̃A�j���[�V�����f�[�^
TexAnimData Mosubi::ANIM_DATA[(int)EAnimType::Num] =
{
	{
		new TexAnim[4]
		{
			{ 0, 10}, { 1, 10},
			{ 2, 10}, { 3, 10},
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
Mosubi::Mosubi(int type, const CVector3D& pos)
	: EnemyBase(pos)
	, mp_image(nullptr)
	, m_type(type)
{
	m_hp = 200;

	// ���X�r�̉摜��ǂݍ���
	std::string imagePath;
	if (m_type == 0) imagePath = "mosubi.png";
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

// �f�X�g���N�^
Mosubi::~Mosubi()
{
	// �摜�f�[�^���폜
	delete mp_image;
}

// ������������
void Mosubi::Death()
{
	// ����������Ԃֈڍs
	ChangeState(EState::Action);
}

// ���݂̏�Ԃ�؂�ւ�
void Mosubi::ChangeState(EState state)
{
	if (m_state == state) return;

	m_state = state;
	m_stateStep = 0;
}

// �ҋ@���̍X�V����
void Mosubi::StateIdle()
{
	// �ҋ@�A�j���[�V�������Đ�
	mp_image->ChangeAnimation((int)EAnimType::Idle);
}

// �����������̍X�V����
void Mosubi::StateAction()
{
	// �X�e�b�v���Ƃɏ�����؂�ւ�
	switch (m_stateStep)
	{
		// �X�e�b�v0�F���������A�j���[�V�������Đ�
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

// �X�V����
void Mosubi::Update()
{
	if (PUSH(CInput::eMouseR))
	{
		ChangeState(EState::Action);
	}

	// ��Ԃɍ��킹�āA�X�V������؂�ւ���
	switch (m_state)
	{
	case EState::Idle:	StateIdle();	break;
	case EState::Action:	StateAction();	break;
	}

	// �C���[�W�ɍ��W��ݒ肵�āA�A�j���[�V�������X�V
	mp_image->SetPos(CalcScreenPos());
	mp_image->UpdateAnimation();
}

// �`�揈��
void Mosubi::Render()
{
	mp_image->Draw();
}

float Mosubi::GetChipSizeX()
{
	float aX = (CHIP_SIZE_W * 0.5);
	return aX;
}

float Mosubi::GetChipSizeY()
{
	float aY = (CHIP_SIZE_H * 0.4);
	return aY;
}

float Mosubi::GetChipSizeZ()
{
	float aZ = (CHIP_SIZE_H * 0.3);
	return aZ;
}