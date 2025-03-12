#include "Player.h"

#define CHIP_SIZE 430		// 1�R�}�̃T�C�Y
#define CENTER_POS CVector2D(1000.0f, 400.0f)	// ���S���W
#define MOVE_SPEED_X 5.0f	// �������̈ړ����x
#define MOVE_SPEED_Z 150.0f	// �������̈ړ����x
#define JUMP_SPEED 25.0f	// �W�����v���x
#define GRAVITY -1.0f		// �d��

#define SLIDING_MOVE_SPEED_X 5.0f	// �X���C�f�B���O���̈ړ����x
#define SLOW_DOWN_SPEED_X 2.0f		// �X���C�f�B���O��̂������ړ����x

bool m_slowDownAfterSliding = false;		// �X���C�f�B���O��ɂ������ړ�����t���O

// �v���C���[�̃A�j���[�V�����f�[�^�̑O�錾
TexAnimData Player::ANIM_DATA[(int)EAnimType::Num] =
{
	// �_�b�V���A�j���[�V����
	{
		new TexAnim[4]
		{
			{0, 6}, {1, 6}, {2, 6},
			{3, 6}
		},
		4
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
			{45, 12},
			{46, 12},
		},
		2
	},
	// �U���A�j���[�V����
	{
		new TexAnim[2]
		{
			{18, 6},
			{19, 6},
		},
		2
	},
	// �X���C�f�B���O�A�j���[�V����
	{
		new TexAnim[5]
		{
			{9, 6},
			{10, 10},
			{11, 60},
			{12, 10},
			{13, 6},
		},
		5
	},
	// ��_���A�j���[�V����
	{
		new TexAnim[2]
		{
			{36, 20},
			{37, 20},
		},
		2
	},
};


// �R���X�g���N�^
Player::Player(const CVector3D& pos)
	: CharaBase(pos)
	, m_state(EState::Idle)
	, m_stateStep(0)
	, mp_image(nullptr)
{
	m_hp = 3; // UI �ɍ��킹�ď���HP��3��

	// �v���C���[�̉摜��ǂݍ���
	mp_image = CImage::CreateImage
	(
		"elf.png",	// �摜�t�@�C���̃p�X
		ANIM_DATA,		// �A�j���[�V�����̃f�[�^
		CHIP_SIZE, CHIP_SIZE	// 1�R�}�̕��ƍ���
	);
	mp_image->ChangeAnimation((int)EAnimType::Dash);
	mp_image->SetCenter(CENTER_POS);
}

// �f�X�g���N�^
Player::~Player()
{
	// �摜�f�[�^���폜
	delete mp_image;
}

// ���݂̏�Ԃ�؂�ւ�
void Player::ChangeState(EState state)
{
	if (m_state == state) return;

	m_state = state;
	m_stateStep = 0;
}

// �ړ������̍X�V
bool Player::UpdateMove()
{
	bool isMove = false;

	/* ���E�̈ړ��͂���Ȃ��̂ŁA�����͍폜
	// ���L�[�������Ă����
	if (HOLD(CInput::eLeft))
	{
		// �������ֈړ�
		m_pos.x -= MOVE_SPEED_X;
		mp_image->SetFlipH(true);
		isMove = true;
	}
	// �E�L�[�������Ă����
	else if (HOLD(CInput::eRight))
	{
		// �E�����ֈړ�
		m_pos.x += MOVE_SPEED_X;
		mp_image->SetFlipH(false);
		isMove = true;
	}
	*/

	// ���ō쐬
	// ���N���b�N�Ŕ�_��(HP�_�E��)
	if (PUSH(CInput::eMouseL))
	{
		ChangeState(EState::Damage);
	}



	// ��L�[���������u�Ԃ�Z���W�����炷�i���ֈړ��j
	if (PUSH(CInput::eUp) && m_pos.z > -150.0f)
	{
		m_pos.z -= MOVE_SPEED_Z;
		isMove = true;
	}
	// ���L�[���������u�Ԃ�Z���W�𑝂₷�i��O�ֈړ��j
	else if (PUSH(CInput::eDown) && m_pos.z < 150.0f)
	{
		m_pos.z += MOVE_SPEED_Z;
		isMove = true;
	}

	// �X���C�f�B���O���͑����ړ�
	if (m_state == EState::Sliding)
	{
		m_pos.x += SLIDING_MOVE_SPEED_X; // �����ړ�
		isMove = true;
	}
	// �X���C�f�B���O��͈ړ����x���������ɂ���
	else if (m_slowDownAfterSliding)
	{
		m_pos.x += SLOW_DOWN_SPEED_X; // �X���C�f�B���O��̈ړ����x
		isMove = true;
	}
	// �ʏ�̉E�ړ�
	else
	{
		m_pos.x += MOVE_SPEED_X;
		isMove = true;
	}

	// �v���C���[�̈ړ����s��ꂽ��J�������X�N���[��������
	if (isMove) {
		CCamera::ForceCameraScroll(m_pos.x + 40.0f);  // �v���C���[��X�ʒu�Ɋ�Â��ăJ�������ړ�(�J�����͏�ɐ^�񒆂ɂ��Ă���)

		printf("���������_���̒l: %.2f\n", m_pos.x + 40.0f);
	}

	return isMove;
}

// �_�b�V�����̍X�V����
void Player::StateIdle()
{
	// �ړ�����
	bool isMove = UpdateMove();

	// �_�b�V���A�j���[�V�����ɕύX
	mp_image->ChangeAnimation((int)EAnimType::Dash);

	// [SPACE]�L�[�ŃW�����v��Ԃֈڍs
	if (PUSH(CInput::eButton5))
	{
		ChangeState(EState::Jump);
	}
	// [X]�L�[�ōU����Ԃֈڍs
	else if (PUSH(CInput::eButton2))
	{
		ChangeState(EState::Attack);
	}
	// [C]�L�[�ŃX���C�f�B���O��Ԃֈڍs
	else if (PUSH(CInput::eButton3))
	{
		ChangeState(EState::Sliding);
	}
}

// �W�����v���̍X�V����
void Player::StateJump()
{
	// �X�e�b�v���Ƃɏ�����؂�ւ�
	switch(m_stateStep)
	{
		// �X�e�b�v0�F�W�����v�J�n
		case 0:
			// Y���i�����j�̈ړ����x�ɃW�����v�𑬓x��ݒ肵�A
			// �ڒn��Ԃ���������
			m_moveSpeedY = JUMP_SPEED;
			m_isGrounded = false;

			// �W�����v�A�j���[�V�����ɕύX
			mp_image->ChangeAnimation((int)EAnimType::Jump, false);

			m_stateStep++;
			break;
		// �X�e�b�v1�F�W�����v�I��
		case 1:
			// �ڒn������A�ҋ@��Ԃֈڍs
			if (m_isGrounded)
			{
				ChangeState(EState::Idle);
			}
			break;
	}

	// �ړ�����
	bool isMove = UpdateMove();

	if (!isMove) {
		mp_image->ChangeAnimation((int)EAnimType::Jump);
	}

	// �ȉ����ƁA�W�����v�̃A�j���[�V�����������Ȃ��ׁA��L�ɕύX
	//mp_image->ChangeAnimation((int)EAnimType::Dash);
}

// �U�����̍X�V����
void Player::StateAttack()
{
	// �X�e�b�v���Ƃɏ�����؂�ւ�
	switch (m_stateStep)
	{
		// �X�e�b�v0�F�U���A�j���[�V�����ɐ؂�ւ�
		case 0:
			mp_image->ChangeAnimation((int)EAnimType::Attack, false);
			m_stateStep++;
			break;
		// �X�e�b�v1�F�A�j���[�V�����I���҂�
		case 1:
			// �U���A�j���[�V�������I��������A�ҋ@��Ԃֈڍs
			if (mp_image->CheckAnimationEnd())
			{
				ChangeState(EState::Idle);
			}
			break;
	}
}

// ���S���̍X�V����
void Player::StateDeath()
{
}

// �X���C�f�B���O���̍X�V����
void Player::StateSliding()
{
	// �X�e�b�v���Ƃɏ�����؂�ւ�
	switch (m_stateStep)
	{
		// �X�e�b�v0�F�X���C�f�B���O�A�j���[�V�����ɐ؂�ւ�
	case 0:
		mp_image->ChangeAnimation((int)EAnimType::Sliding, false);
		m_stateStep++;
		break;

		// �X�e�b�v1�F�A�j���[�V�����I���҂�
	case 1:
		// �X���C�f�B���O���̈ړ��i�����X�s�[�h�ŉE�ֈړ��j
		m_pos.x += SLIDING_MOVE_SPEED_X;

		// �X���C�f�B���O�A�j���[�V�������I��������A�������ړ����n�߂�
		if (mp_image->CheckAnimationEnd())
		{
			// �X���C�f�B���O��͈ړ����x��x������
			m_slowDownAfterSliding = true;
			ChangeState(EState::Idle);
		}
		break;
	}
}

// ��_�����̍X�V����
void Player::StateDamage()
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
void Player::TakeDamage(int damage) {
	if (m_hp == 0) return; // ���ł�HP��0�Ȃ牽�����Ȃ�

	if (m_hp > damage) {
		m_hp -= damage; // HP �����炷
		
		ChangeState(EState::Damage); // ��_����Ԃֈڍs
	}
	else {
		m_hp = 0;
		Death();
	}
}

// �X�V����
void Player::Update()
{
	// ���݂̏�Ԃɍ��킹�āA������؂�ւ���
	switch (m_state)
	{
	case EState::Idle:		StateIdle();	break;
	case EState::Jump:		StateJump();	break;
	case EState::Attack:	StateAttack();	break;
	case EState::Death:		StateDeath();	break;
	case EState::Sliding:	StateSliding();	break;
	case EState::Damage:    StateDamage();  break;
	}

	// Y���i�����j�̈ړ������W�ɔ��f
	m_pos.y += m_moveSpeedY;
	m_moveSpeedY += GRAVITY;	// Y���̈ړ����x�ɏd�͂����Z
	// �n�ʂ�艺�ɂ�����
	if (m_pos.y <= 0.0f)
	{
		// �n�ʂ̍��W�֖߂�
		m_pos.y = 0.0f;
		m_moveSpeedY = 0.0f;
		m_isGrounded = true;
	}

	// �C���[�W�ɍ��W��ݒ肵�āA�A�j���[�V�������X�V
	mp_image->SetPos(CalcScreenPos());
	mp_image->UpdateAnimation();
}

// �`�揈��
void Player::Render()
{
	mp_image->Draw();
}
