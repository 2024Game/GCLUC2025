#include "Boss.h"

#define CHIP_SIZE_W 1750	// 1�R�}�̃T�C�Y(��)
#define CHIP_SIZE_H 950     // 1�R�}�̃T�C�Y(����)

#define CENTER_POS CVector2D(0.0f, 300.0f)	// ���S���W
#define MOVE_SPEED_X 5.0f	// �������̈ړ����x
#define GRAVITY -0.23f		// �d��

// �v���C���[�̃A�j���[�V�����f�[�^�̑O�錾
TexAnimData Boss::ANIM_DATA[(int)EAnimType::Num] =
{
	// �_�b�V���A�j���[�V����
	{
		new TexAnim[5]
		{
			{0, 6}, {1, 6}, {2, 6},
			{3, 6}, {4, 6}
		},
		5
	},
	// �W�����v�A�j���[�V����
	{
		new TexAnim[4]
		{
			{27, 2}, {28, 2}, {29, 2},
			{30, 2}
		},
		4
	},
	// ���S�A�j���[�V����
	{
		new TexAnim[2]
		{
			{45, 30},
			{46, 30},
		},
		2
	},
	// �U���A�j���[�V����
	{
		new TexAnim[2]
		{
			{18, 20},
			{19, 20},
		},
		2
	},
	// ��_���A�j���[�V����
	{
		new TexAnim[3]
		{
			{5, 20},
			{6, 20},
			{7, 20}
		},
		3
	},
};


// �R���X�g���N�^
Boss::Boss(const CVector3D& pos)
	: CharaBase(pos)
	, m_state(EState::Idle)
	, m_stateStep(0)
	, mp_image(nullptr)
	, m_isInvincible(false)
	, m_invincibleTime(0.0f)
{
	m_hp = 20; // UI �ɍ��킹�ď���HP��3��
	s_isMove = false;
	// HP�����L
	sHp = m_hp;
	//

	// �v���C���[�̉摜��ǂݍ���
	mp_image = CImage::CreateImage
	(
		"dragon.png",	// �摜�t�@�C���̃p�X
		ANIM_DATA,		// �A�j���[�V�����̃f�[�^
		CHIP_SIZE_W, CHIP_SIZE_H	// 1�R�}�̕��ƍ���
	);
	mp_image->ChangeAnimation((int)EAnimType::Dash);
	mp_image->SetCenter(CENTER_POS);
}

// �f�X�g���N�^
Boss::~Boss()
{
	// �摜�f�[�^���폜
	delete mp_image;
}

// ���݂̏�Ԃ�؂�ւ�
void Boss::ChangeState(EState state)
{
	if (m_state == state) return;

	m_state = state;
	m_stateStep = 0;
}

// �ړ������̍X�V
bool Boss::UpdateMove()
{
	// �E�����ֈړ�
	m_pos.x += MOVE_SPEED_X;
	mp_image->SetFlipH(false);
	return true;
}

// �_�b�V�����̍X�V����
void Boss::StateIdle()
{
	// �ړ�����
	bool isMove = UpdateMove();

	// �_�b�V���A�j���[�V�����ɕύX
	mp_image->ChangeAnimation((int)EAnimType::Dash);
}

void Boss::StateJump()
{
}

void Boss::StateAttack()
{
}

// ���S���̍X�V����
void Boss::StateDeath()
{
	// �X�e�b�v���Ƃɏ�����؂�ւ�
	switch (m_stateStep)
	{
		// �X�e�b�v0�F��_���A�j���[�V�����ɐ؂�ւ�
	case 0:
		mp_image->ChangeAnimation((int)EAnimType::Damage, false);
		m_stateStep++;
		break;
		// �X�e�b�v1�F�A�j���[�V�����I���҂�
	case 1:
		// ��_���A�j���[�V�������I��������A���S��Ԃֈڍs
		if (mp_image->CheckAnimationEnd())
		{
			m_stateStep++;
		}
		break;
		// �X�e�b�v0�F���S�A�j���[�V�����ɐ؂�ւ�
	case 2:
		printf("Hello, World!\n");
		mp_image->ChangeAnimation((int)EAnimType::Death, false);
		m_stateStep++;
		break;
		// �X�e�b�v1�F�A�j���[�V�����I���҂�
	case 3:
		// ��_���A�j���[�V�������I��������A�Q�[���I�[�o�[��ʂֈڍs
		if (mp_image->CheckAnimationEnd())
		{
			//Death();
		}
		break;
	}
}

// ��_�����̍X�V����
void Boss::StateDamage()
{
	// �X�e�b�v���Ƃɏ�����؂�ւ�
	switch (m_stateStep)
	{
		// �X�e�b�v0�F��_���A�j���[�V�����ɐ؂�ւ�
	case 0:
		mp_image->ChangeAnimation((int)EAnimType::Damage, false);
		m_stateStep++;
		break;
		// �X�e�b�v1�F�A�j���[�V�����I���҂�
	case 1:
		// ��_���A�j���[�V�������I��������A�ҋ@��Ԃֈڍs
		if (mp_image->CheckAnimationEnd())
		{
			ChangeState(EState::Idle);
		}
		break;
	}
}

// �_���[�W���󂯂鏈��
void Boss::TakeDamage(int damage)
{
	if (m_isInvincible) return; // ���G���Ȃ牽�����Ȃ�

	if (m_hp > damage)
	{
		m_hp -= damage;					// HP�����炷

		// HP�����L
		sHp = m_hp;
		m_isInvincible = true;			// ���G��Ԃɐݒ�
		m_invincibleTime = 3.0f;		// ���G���Ԃ�3�b�ɐݒ�
		ChangeState(EState::Damage);	// ��_����Ԃֈڍs
	}
	else
	{
		// HP��0�ɂ��āA��_���A�j���[�V����+���S�A�j���[�V�����ɐ؂�ւ�
		m_hp = 0;
		sHp = m_hp; // HP�����L
		ChangeState(EState::Death); // ���S��Ԃֈڍs
	}
}

// HP���擾
int Boss::sHp = 0; // HP


int Boss::Hp()
{
	return sHp;
}

bool Boss::s_isMove = false;
bool Boss::IsMove()
{
	return s_isMove;
}

extern float testScrollX;

// �X�V����
void Boss::Update()
{
	// ���G��Ԃ̎��Ԃ��J�E���g�_�E��
	if (m_isInvincible)
	{
		m_invincibleTime -= 1.0f / 60.0f; // 1�t���[�����ƂɎ��Ԃ����� (60FPS)
		if (m_invincibleTime <= 0.0f)
		{
			m_isInvincible = false; // ���G����
		}
	}

	// ���݂̏�Ԃɍ��킹�āA������؂�ւ���
	switch (m_state)
	{
	case EState::Idle:		StateIdle();	break;
	case EState::Jump:		StateJump();	break;
	case EState::Attack:	StateAttack();	break;
	case EState::Death:		StateDeath();	break;
	case EState::Damage:    StateDamage();  break;
	}

	m_pos.x = testScrollX;

	// �C���[�W�ɍ��W��ݒ肵�āA�A�j���[�V�������X�V
	mp_image->SetPos(CalcScreenPos());
	mp_image->UpdateAnimation();
}

// �`�揈��
void Boss::Render()
{
	mp_image->Draw();
}