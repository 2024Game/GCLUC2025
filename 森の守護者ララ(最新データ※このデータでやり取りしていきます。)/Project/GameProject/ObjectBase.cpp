#include "ObjectBase.h"
#define FIELD_CENTER_Z (SCREEN_HEIGHT * 0.75f)

#include "Player.h"

// �R���X�g���N�^
ObjectBase::ObjectBase()
	: ObjectBase(CVector3D::zero)
{
	
}

ObjectBase::ObjectBase(const CVector3D& pos)
	: Task((int)ETaskPrio::Object, 0)
	, m_pos(pos)
	, m_isGrounded(true)
	, mp_shadowImg(nullptr)
{
	// �e�̉摜��ǂݍ���
	mp_shadowImg = CImage::CreateImage("shadow.png");
	mp_shadowImg->SetCenter(mp_shadowImg->GetSize() * 0.5f);
}

// �f�X�g���N�^
ObjectBase::~ObjectBase()
{
	// �e�̉摜�f�[�^���폜
	if (mp_shadowImg != nullptr)
	{
		delete mp_shadowImg;
		mp_shadowImg = nullptr;
	}
}

// ���W���擾
const CVector3D& ObjectBase::GetPos() const
{
	return m_pos;
}

// ���W��ݒ�
void ObjectBase::SetPos(const CVector3D& pos)
{
	m_pos = pos;
}

float testScrollX = 0.0f;

// 3�������W����2�������W���v�Z
CVector2D ObjectBase::CalcScreenPos(bool grounded) const
{
	CVector2D ret;

	if (Player::IsMove())
	{
		//testScrollX += 1.0;
	}
	else
	{
		// �ړ����Ă��Ȃ��ꍇ�͕ύX�Ȃ�
	}

	ret.x = m_pos.x - testScrollX;

	// �ʏ���W�����߂�ꍇ
	if (!grounded)
	{
		// Y���W�́A3�������W��Y�i�����j��Z�i���s�j�����킹��
		float posZ = FIELD_CENTER_Z + m_pos.z;
		ret.y = -m_pos.y + posZ;
	}
	// �������l�����Ȃ��n�ʂ̈ʒu�����߂�ꍇ
	else
	{
		// Y���W�́A3�������W��Z�i���s�j�̂ݔ��f����
		ret.y = FIELD_CENTER_Z + m_pos.z;
	}

	return ret;
}

// �e�`��
void ObjectBase::RenderShadow()
{
	// �e��n�ʂ̈ʒu�ŕ`��
	mp_shadowImg->SetPos(CalcScreenPos(true));

	// �e�̃T�C�Y�ύX(�ǉ�)
	mp_shadowImg->SetSize(150, 150);


	mp_shadowImg->Draw();
}

float ObjectBase::GetChipSizeX()
{
	return 50.0f;
}
float ObjectBase::GetChipSizeY()
{
	return 0.0f;
}
float ObjectBase::GetChipSizeZ()
{
	return 0.0f;
}