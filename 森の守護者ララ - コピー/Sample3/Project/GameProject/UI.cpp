#include "UI.h"
#include "DebugPrint.h"
#include <cstdio>

// �R���X�g���N�^
UI::UI() : m_elapsedTime(0), m_maxHP(3), m_currentHP(3), m_currentMana(0) {}

// �f�X�g���N�^
UI::~UI() {}

// HP�A�C�R���̐ݒ�
void UI::InitHp(const char* hpTexture, int maxHP, int iconWidth, int iconHeight) {
    m_maxHP = maxHP;
    m_currentHP = maxHP;
    m_hpIcons.clear();

    // HP�A�C�R�������[�h
    for (int i = 0; i < maxHP; ++i) {
        CImage hpIcon;
        hpIcon.Load(hpTexture);
        hpIcon.SetSize(iconWidth, iconHeight);  // �A�C�R���̃T�C�Y��ݒ�
        hpIcon.SetPos(20 + i * 100, 30); // �����3���ׂ�
        m_hpIcons.push_back(hpIcon);
    }
}

// �}�i�A�C�R���̐ݒ�
void UI::InitMana(const char* manaTexture, int iconWidth, int iconHeight) {
    m_currentMana = 1;  // �����}�i�� 0
    m_manaIcons.clear();

    CImage manaIcon;
    manaIcon.Load(manaTexture);
    manaIcon.SetSize(iconWidth, iconHeight);
    manaIcon.SetPos(20, 130); // �}�i�A�C�R���̈ʒu�iHP�̉��j

    m_manaIcons.push_back(manaIcon);
}

// �o�ߎ��Ԃ�ݒ肷��֐�
void UI::SetElapsedTime(float time) {
    m_elapsedTime = time;
}

// UI�̕\��
void UI::Render()
{
    // ������̍쐬
    char timeText[32];
    sprintf_s(timeText, "Time: %.2f sec", m_elapsedTime);
    // ���������ʂɕ\��
    // �t�H���g�C���X�^���X���쐬(PC�ɓ����Ă���t�H���g)
    CREATE_FONT_F("customFont", "C:\\Windows\\Fonts\\msgothic.ttc", 80); // �T�C�Y80��MS�S�V�b�N���g�p
    // �t�H���g���g���ĕ������`��(���ʒu(���)�A�c�ʒu(��)�AR�AG�AB�A�\������e�L�X�g)
    FONT("customFont")->Draw(620, 100, 0.8f, 0.3f, 0.5f, timeText, "Time: %.2f sec", m_elapsedTime);

    // HP�A�C�R���`��
    for (int i = 0; i < m_currentHP; ++i) {
        m_hpIcons[i].Draw();
    }

    // �}�i�A�C�R���`��i1�����\���j
    if (m_currentMana > 0 && !m_manaIcons.empty()) {
        m_manaIcons[0].Draw();
    }
}

// HP��ݒ�
void UI::SetHP(int hp) {
    m_currentHP = hp;
}

// �}�i��ݒ�i1�܂Łj
void UI::SetMana(int mana) {
    m_currentMana = (mana > 0) ? 1 : 0; // 1�ȏ�Ȃ�1�ɐ���
}
