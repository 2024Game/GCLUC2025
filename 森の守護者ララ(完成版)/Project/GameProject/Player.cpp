#include "Player.h"

#define CHIP_SIZE 430		// 1�R�}�̃T�C�Y
//#define CENTER_POS CVector2D(1000.0f, 400.0f)	// ���S���W
#define CENTER_POS CVector2D(0.0f, 300.0f)	// ���S���W
#define MOVE_SPEED_X 5.0f	// �������̈ړ����x
#define MOVE_SPEED_Z 150.0f	// �������̈ړ����x
#define JUMP_SPEED 15.0f	// �W�����v���x
//#define GRAVITY -0.175f		// �d��
#define GRAVITY -0.45f		// �d��


bool m_slowDownAfterSliding = false;		// �X���C�f�B���O��ɂ������ړ�����t���O
float moveSpeed;
// ���t���[���ł̃X�s�[�h�A�b�v��
float speedUpPerFrame = 10.0f / (300.0f * 60.0f); // 1�t���[�����Ƃ̑�����

// �v���C���[�̃A�j���[�V�����f�[�^�̑O�錾
TexAnimData Player::ANIM_DATA[(int)EAnimType::Num] =
{
	// �_�b�V���A�j���[�V����
	{
		new TexAnim[6]
		{
			{0, 6}, {1, 6}, {2, 6},
			{3, 6}, {4, 6}, {5, 6}
		},
		6
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
			{18, 10},
			{19, 10},
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
			{12, 20},
			{13, 6},
		},
		5
	},
	// ��_���A�j���[�V����
	{
		new TexAnim[2]
		{
			{36, 10},
			{37, 10},
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
	, m_isInvincible(false)
	, m_invincibleTime(0.0f)
	, m_isFloat(false)
	, m_floatTime(0.0f)
{
	m_hp = 3; // UI �ɍ��킹�ď���HP��3��
	moveSpeed = MOVE_SPEED_X;
	s_isMove = false;
	// HP�����L
	sHp = m_hp;
	//

	// �v���C���[�̉摜��ǂݍ���
	mp_image = CImage::CreateImage
	(
		"�G���t�A�j���[�V����.png",	// �摜�t�@�C���̃p�X
		ANIM_DATA,		// �A�j���[�V�����̃f�[�^
		CHIP_SIZE, CHIP_SIZE	// 1�R�}�̕��ƍ���
	);
	mp_image->ChangeAnimation((int)EAnimType::Dash);
	mp_image->SetCenter(CENTER_POS);

	// �ǉ�
	// �W�����vSE
	SOUND("jump_se")->Load("jump.wav", 1, false);
	// ��SE
	SOUND("kaifuku_se")->Load("kaifuku.wav", 1, false);
	// ��_��SE
	SOUND("damage_se")->Load("damage.wav", 1, false);
	// �U��SE
	SOUND("attack_se")->Load("attack.wav", 1, false);
	// �X���C�f�B���OSE
	SOUND("sliding_se")->Load("sliding.wav", 1, false);
	// �W�����vSE
	SOUND("jump_se")->Volume(0.1f);  // 01%�̉���
	// ��SE
	SOUND("kaifuku_se")->Volume(0.1f);  // 01%�̉���
	// ��_��SE
	SOUND("damage_se")->Volume(0.1f);  // 01%�̉���
	// �U��SE
	SOUND("attack_se")->Volume(0.1f);  // 01%�̉���
	// �X���C�f�B���OSE
	SOUND("sliding_se")->Volume(0.1f);  // 01%�̉���
}

// �f�X�g���N�^
Player::~Player()
{
	// �摜�f�[�^���폜
	delete mp_image;
}

void Player::Attack()
{
	ChangeState(EState::Attack);
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

	// �W�����v���̓��[���ړ��𖳌���
	if (m_state == EState::Jump)
	{
		// ��L�[���������u�Ԃ�Z���W�����炷�i���ֈړ��j
		if (PUSH(CInput::eUp) && m_pos.z > -230.0f)
		{
			m_pos.z -= MOVE_SPEED_Z;
			isMove = true;
		}
		// ���L�[���������u�Ԃ�Z���W�𑝂₷�i��O�ֈړ��j
		else if (PUSH(CInput::eDown) && m_pos.z < 70.0f)
		{
			m_pos.z += MOVE_SPEED_Z;
			isMove = true;
		}
	}
	else
	{
		// ��L�[���������u�Ԃ�Z���W�����炷�i���ֈړ��j
		if (PUSH(CInput::eUp) && m_pos.z > -230.0f)
		{
			m_pos.z -= MOVE_SPEED_Z;
			isMove = true;
		}
		// ���L�[���������u�Ԃ�Z���W�𑝂₷�i��O�ֈړ��j
		else if (PUSH(CInput::eDown) && m_pos.z < 70.0f)
		{
			m_pos.z += MOVE_SPEED_Z;
			isMove = true;
		}
	}

	// �X���C�f�B���O���͑����ړ�
	if (m_state == EState::Sliding)
	{
		//m_pos.x += moveSpeed; // �����ړ�
		isMove = true;
	}
	// �X���C�f�B���O��͈ړ����x���������ɂ���
	else if (m_slowDownAfterSliding)
	{
		m_pos.x += moveSpeed; // �X���C�f�B���O��̈ړ����x
		isMove = true;
	}

	// �ʏ�̉E�ړ�
	else
	{
		//m_pos.x += MOVE_SPEED_X;
		//isMove = true;

		// �E�����ֈړ�
		m_pos.x += moveSpeed;
		mp_image->SetFlipH(false);
		isMove = true;
	}

	//s_isMove = isMove;

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
	//else if (PUSH(CInput::eButton2))
	//{
		//ChangeState(EState::Attack);
	//}
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
	switch (m_stateStep)
	{
		// �X�e�b�v0�F�W�����v�J�n
	case 0:
		// Y���i�����j�̈ړ����x�ɃW�����v�𑬓x��ݒ肵�A
		// �ڒn��Ԃ���������
		m_moveSpeedY = JUMP_SPEED;
		m_isGrounded = false;

		// �W�����v�A�j���[�V�����ɕύX
		mp_image->ChangeAnimation((int)EAnimType::Jump, false);

		// �ǉ�
		// �W�����vSE�Đ�
		SOUND("jump_se")->Play(false);
		//

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

	if (!isMove)
	{
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

		// �ǉ�
		// �U��SE�Đ�
		SOUND("attack_se")->Play(false);
		//

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
	// �ړ�����
	bool isMove = UpdateMove();
}

// ���S���̍X�V����
void Player::StateDeath()
{
	// �X�e�b�v���Ƃɏ�����؂�ւ�
	switch (m_stateStep)
	{
		// �X�e�b�v0�F��_���A�j���[�V�����ɐ؂�ւ�
	case 0:
		mp_image->ChangeAnimation((int)EAnimType::Damage, false);

		// �ǉ�
		// ��_��SE�Đ�
		SOUND("damage_se")->Play(false);
		//

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
		//printf("Hello, World!\n");
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

// �X���C�f�B���O���̍X�V����
void Player::StateSliding()
{
	// �X�e�b�v���Ƃɏ�����؂�ւ�
	switch (m_stateStep)
	{
		// �X�e�b�v0�F�X���C�f�B���O�A�j���[�V�����ɐ؂�ւ�
	case 0:
		mp_image->ChangeAnimation((int)EAnimType::Sliding, false);

		// �ǉ�
		// �X���C�f�B���OSE�Đ�
		SOUND("sliding_se")->Play(false);
		//

		m_stateStep++;
		break;

		// �X�e�b�v1�F�A�j���[�V�����I���҂�
	case 1:
		// [SPACE]�L�[�ŃW�����v��Ԃֈڍs
		if (PUSH(CInput::eButton5))
		{
			ChangeState(EState::Jump);
		}

		// �X���C�f�B���O���̈ړ��i�����X�s�[�h�ŉE�ֈړ��j
		m_pos.x += moveSpeed;

		// �X���C�f�B���O�A�j���[�V�������I��������A�������ړ����n�߂�
		if (mp_image->CheckAnimationEnd())
		{
			// �X���C�f�B���O��͈ړ����x��x������
			//m_slowDownAfterSliding = true;
			ChangeState(EState::Idle);
		}
		break;
	}
	bool isMove = UpdateMove();
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

		// �ǉ�
		// ��_��SE�Đ�
		SOUND("damage_se")->Play(false);
		//

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
void Player::TakeDamage(int damage)
{
	if (m_isInvincible) return; // ���G���Ȃ牽�����Ȃ�

	if (m_hp > damage)
	{
		m_hp -= damage;					// HP�����炷

		// HP�����L
		sHp = m_hp;
		//

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

void Player::TakeHeal(int amount)
{
	if (m_isHeal) return; // ���G���Ȃ牽�����Ȃ�
	if (0 < m_hp && m_hp < 3)
	{
		m_hp += amount;					// HP�𑝂₷

		// �ǉ�
		// ��SE�Đ�
		SOUND("kaifuku_se")->Play(false);
		//

		// HP�����L
		sHp = m_hp;

		m_isHeal = true;			// ���G��Ԃɐݒ�
		m_notHealTime = 1.0f;		// ���G���Ԃ�3�b�ɐݒ�
	}
	else if (m_hp <= 0)
	{

	}
	else
	{

	}
}


float Player::GetChipSizeX()
{
	float aX = (CHIP_SIZE * 0.4f);
	// CHIP_SIZE = 430
	return aX;
}

float Player::GetChipSizeY()
{
	float aY = 0.0f;

	if (m_state == EState::Sliding)
	{
		aY = CHIP_SIZE * 0.2f;
	}
	else
	{
		aY = CHIP_SIZE * 0.4f;
	}

	// CHIP_SIZE = 430
	return aY;
}

float Player::GetChipSizeZ()
{
	float aZ = (CHIP_SIZE * 0.2f);
	return aZ;
}

// HP���擾
int Player::sHp = 0; // HP


int Player::Hp()
{
	return sHp;
}

bool Player::s_isMove = false;
bool Player::IsMove()
{
	return s_isMove;
}

extern float testScrollX;

// �X�V����
void Player::Update()
{
	moveSpeed += speedUpPerFrame;
	s_isMove = !m_isInvincible;
	// ���G��Ԃ̎��Ԃ��J�E���g�_�E��
	if (m_isInvincible)
	{
		m_invincibleTime -= 1.0f / 60.0f; // 1�t���[�����ƂɎ��Ԃ����� (60FPS)
		if (m_invincibleTime <= 0.0f)
		{
			m_isInvincible = false; // ���G����
		}
	}

	// ���G��Ԃ̎��Ԃ��J�E���g�_�E��                // 3/17�ύX�i����j
	if (m_isHeal)
	{
		m_notHealTime -= 1.0f / 60.0f; // 1�t���[�����ƂɎ��Ԃ����� (60FPS)
		if (m_notHealTime <= 0.0f)
		{
			m_isHeal = false; // ���G����
		}
	}

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
	if (m_moveSpeedY > 0)
	{
		m_moveSpeedY += GRAVITY;	// Y���̈ړ����x�ɏd�͂����Z
	}

	else if (m_moveSpeedY <= 0 && !m_isGrounded && !m_isFloat)
	{
		m_floatTime = 0.8f; //�@�؋󎞊Ԃ̐ݒ�
		m_isFloat = true;
	}
	else if(m_floatTime <= 0 && !m_isGrounded)
	{
		m_moveSpeedY += GRAVITY;	// Y���̈ړ����x�ɏd�͂����Z
	}
	// �n�ʂ�艺�ɂ�����
	if (m_pos.y <= 0.0f)
	{
		// �n�ʂ̍��W�֖߂�
		m_pos.y = 0.0f;
		m_moveSpeedY = 0.0f;
		m_isGrounded = true;
		m_isFloat = false;
	}

	if (m_floatTime > 0)
	{
		m_floatTime -= 1.0f / 60.0f;
	}

	testScrollX = m_pos.x;

	// �C���[�W�ɍ��W��ݒ肵�āA�A�j���[�V�������X�V
	mp_image->SetPos(CalcScreenPos());
	mp_image->UpdateAnimation();
}

// �`�揈��
void Player::Render()
{
	mp_image->Draw();
}