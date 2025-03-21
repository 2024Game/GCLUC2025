#include "BGTree2.h"

#define CHIP_SIZE_W 1920		// ���̃T�C�Y
#define CHIP_SIZE_H 1080		// �c�̃T�C�Y
#define CENTER_POS CVector2D(0.0f, 0.0f)	// ���S���W
#define FIELD_CENTER_Z (SCREEN_HEIGHT * 0.75f)

// �X���C���̃A�j���[�V�����f�[�^
TexAnimData BGTree2::ANIM_DATA[(int)EAnimType::Num] =
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

BGTree2::BGTree2(int type, const CVector3D& pos)
	: ObjectBase(pos)
	, mp_image(nullptr)
	, m_type(type)
{
	std::string imagePath;
	if (m_type == 0) imagePath = "tree.png",	// �摜�t�@�C���̃p�X
		// �v���C���[�̉摜��ǂݍ���
		mp_image = CImage::CreateImage
		(
			imagePath.c_str(),	// �摜�t�@�C���̃p�X
			ANIM_DATA,		// �A�j���[�V�����̃f�[�^
			CHIP_SIZE_W, CHIP_SIZE_H	// 1�R�}�̕��ƍ���
		);
	//mp_image->ChangeAnimation((int)EAnimType::Idle);
	mp_image->ChangeAnimation(0);
	mp_image->SetCenter(CENTER_POS);
	mp_image->SetFlipH(true);
}

BGTree2::~BGTree2()
{
	// �摜�f�[�^���폜
	delete mp_image;
}

void BGTree2::Update()
{
	// ��Ԃɍ��킹�āA�X�V������؂�ւ���
	switch (m_state)
	{
	case EState::Idle:	StateIdle();	break;
	}

	// �C���[�W�ɍ��W��ݒ肵�āA�A�j���[�V�������X�V
	mp_image->SetPos(CalcScreenPos());
	mp_image->UpdateAnimation();
}

void BGTree2::Render()
{
	mp_image->Draw();
}

extern float testScrollX;
//int loopN2 = 0;
CVector2D BGTree2::CalcScreenPos(bool grounded) const
{
	CVector2D ret;

	ret.x = ((m_pos.x + (3840 * loopN2)) - (testScrollX * 0.6f));

	// �ʏ���W�����߂�ꍇ
	if (!grounded)
	{
		// Y���W�́A3�������W��Y�i�����j��Z�i���s�j�����킹��
		float posZ = FIELD_CENTER_Z + m_pos.z;
		ret.y = -m_pos.y + posZ;
		ret.y = -m_pos.y;
	}
	// �������l�����Ȃ��n�ʂ̈ʒu�����߂�ꍇ
	else
	{
		// Y���W�́A3�������W��Z�i���s�j�̂ݔ��f����
		ret.y = FIELD_CENTER_Z + m_pos.z;
	}

	//if (ret.x < -3840.0f)
	if (ret.x < -1920.0f)
	{
		//loopN2++;
		// `loopN` �������o�ϐ��Ƃ��čX�V
		const_cast<BGTree2*>(this)->loopN2++;
	}

	return ret;
}


void BGTree2::ChangeState(EState state)
{
	if (m_state == state) return;

	m_state = state;
	m_stateStep = 0;
}

void BGTree2::StateIdle()
{
	// �ҋ@�A�j���[�V�������Đ�
	mp_image->ChangeAnimation((int)EAnimType::Idle);
}