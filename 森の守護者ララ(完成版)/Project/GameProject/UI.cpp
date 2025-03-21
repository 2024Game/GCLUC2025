#include "UI.h"
#include "DebugPrint.h"
#include <cstdio>

// �R���X�g���N�^
UI::UI() : m_elapsedTime(0), m_maxHP(3), m_currentHP(3), m_currentMana(0), m_maxBossHP(20), m_currentBossHP(20) {}

// �f�X�g���N�^
UI::~UI() {}

// �V�[��(�ǉ�)
// �X�^�[�g��ʂ̐ݒ�
void UI::Start(const char* titleTexture, int gWidth, int gHeight)
{
    CImage StartG;
    StartG.Load(titleTexture); // �f�[�^�̓ǂݍ���
    StartG.SetSize(gWidth, gHeight); // �T�C�Y���w��
    StartG.SetPos(0, 0); // �\������ꏊ(����)

    m_titleG.push_back(StartG);

    m_titleG[0].Draw(); // �`��
}

// �N���A��ʂ̐ݒ�
void UI::Clear(const char* clearTexture, int gWidth, int gHeight)
{
    CImage ClearG;
    ClearG.Load(clearTexture); // �f�[�^�̓ǂݍ���
    ClearG.SetSize(gWidth, gHeight); // �T�C�Y���w��
    ClearG.SetPos(0, 0); // �\������ꏊ(����)

    m_clearG.push_back(ClearG);

    m_clearG[0].Draw(); // �`��
}

// �I�[�o�[���
void UI::Over(const char* overTexture, int gWidth, int gHeight)
{
    CImage OverG;
    OverG.Load(overTexture); // �f�[�^�̓ǂݍ���
    OverG.SetSize(gWidth, gHeight); // �T�C�Y���w��
    OverG.SetPos(0, 0); // �\������ꏊ(����)

    m_overG.push_back(OverG);

    m_overG[0].Draw(); // �`��
}

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
    m_currentMana = 0;  // �����}�i�� 0
    m_manaIcons.clear();

    CImage manaIcon;
    manaIcon.Load(manaTexture);
    manaIcon.SetSize(iconWidth, iconHeight);
    manaIcon.SetPos(20, 130); // �}�i�A�C�R���̈ʒu�iHP�̉��j

    m_manaIcons.push_back(manaIcon);
}

// �^�C���̌��̔̐ݒ�
void UI::InitTimeBack(const char* timebackTexture, int iconWidth, int iconHeight)
{
    CImage timebackIcon;
    timebackIcon.Load(timebackTexture);
    timebackIcon.SetSize(iconWidth, iconHeight);
    timebackIcon.SetPos(1280, 20);

    m_timebackIcon.push_back(timebackIcon);
}

// �o�ߎ��Ԃ�ݒ肷��֐�
void UI::SetElapsedTime(float time)
{
    m_elapsedTime = time;
}

// �N���A���Ԃ�ݒ�
void UI::SetClearTime(float time)
{
    m_clearTime = time;
}

// UI�̕\��
void UI::Render()
{
    // �^�C���̕\�����O�Ɍ��̉摜��`��
    m_timebackIcon[0].Draw();

    // ������̍쐬
    char timeText[32];
    sprintf_s(timeText, "Time:%.2f sec", m_elapsedTime);
    // ���������ʂɕ\��
    // �t�H���g�C���X�^���X���쐬(PC�ɓ����Ă���t�H���g)
    CREATE_FONT_F("customFont", "C:\\Windows\\Fonts\\msgothic.ttc", 80); // �T�C�Y80��MS�S�V�b�N���g�p
    // �t�H���g���g���ĕ������`��(���ʒu(���)�A�c�ʒu(��)�AR�AG�AB�A�\������e�L�X�g)
    FONT("customFont")->Draw(1300, 100, 0.8f, 0.3f, 0.5f, timeText, "Time: %.2f sec", m_elapsedTime);
    //FONT("customFont")->Draw(1350, 100, 1.0f, 1.0f, 1.0f, timeText, "Time: %.2f sec", m_elapsedTime);

    // HP�̕ύX
    for (int i = 0; i < m_maxHP; ++i) {
        if (i < m_currentHP)
        {
            // HP���c���Ă���A�C�R���͒ʏ�̐F�ŕ`��
            m_hpIcons[i].SetColor(1.0f, 1.0f, 1.0f, 1.0f); // ���F
        }
        else
        {
            // HP���������A�C�R���͍����`��
            m_hpIcons[i].SetColor(0.1f, 0.1f, 0.1f, 0.8f); // ���F
        }
        // �A�C�R����`��
        m_hpIcons[i].Draw();
    }

    /*// �}�i�A�C�R���`��i1�����\���j
    if (m_currentMana > 0 && !m_manaIcons.empty())
    {
        // �}�i���c���Ă���ꍇ�͒ʏ�̐F�ŕ`��
        m_manaIcons[0].SetColor(1.0f, 1.0f, 1.0f, 1.0f); // ���F
    }
    else
    {
        // �}�i�����������ꍇ�͍����`��
        if (!m_manaIcons.empty())
        {
            m_manaIcons[0].SetColor(0.1f, 0.1f, 0.1f, 0.8f); // ���F
        }
    }
    // �}�i�A�C�R����`��
    if (!m_manaIcons.empty())
    {
        m_manaIcons[0].Draw();
    }*/
}

// �N���A���Ԃ̕\��
void UI::Render1()
{
    // ��������쐬
    char timeText[32];
    sprintf_s(timeText, "Time:%.2f sec", m_clearTime);

    // �t�H���g�C���X�^���X���쐬�i����̂݁j
    CREATE_FONT_F("customFont", "C:\\Windows\\Fonts\\msgothic.ttc", 100);

    // �N���A�^�C���̕\��
    FONT("customFont")->Draw(155, 690, 1.0f, 0.5f, 0.5f, timeText);
}

// HP��ݒ�
void UI::SetHP(int hp)
{
    // HP���ő�l�𒴂��Ȃ��悤�ɐ���
    m_currentHP = (hp > m_maxHP) ? m_maxHP : hp;

    // HP����������A�C�R���������ݒ�
    for (int i = 0; i < m_currentHP; ++i) {
        m_hpIcons[i].SetColor(1.0f, 1.0f, 1.0f, 1.0f);  // ��
    }
    for (int i = m_currentHP; i < m_maxHP; ++i) {
        m_hpIcons[i].SetColor(0.0f, 0.0f, 0.0f, 1.0f);  // ��
    }
}

// �}�i��ݒ�i1�܂Łj
void UI::SetMana(int mana)
{
    m_currentMana = (mana > 0) ? 1 : 0; // 1�ȏ�Ȃ�1�ɐ���
}


// �{�XHP�o�[�̏�����
void UI::InitBossHPBar(const char* hpBarBackTexture, const char* hpBarFrontTexture, int barWidth, int barHeight)
{
    m_bossHPBarWidth = barWidth; // �ő啝��ۑ�
    m_bossHPBarHeight = barHeight; // ������ۑ�

    // �w�i�o�[
    CImage bossHPBarBack;
    bossHPBarBack.Load(hpBarBackTexture);
    bossHPBarBack.SetSize(barWidth, barHeight);
    bossHPBarBack.SetPos(480, 80);  // ��ʏ㕔�ɔz�u
    m_bossHPBarBack.push_back(bossHPBarBack);

    // �O�ʃo�[�iHP�̌����𔽉f�j
    CImage bossHPBarFront;
    bossHPBarFront.Load(hpBarFrontTexture);
    bossHPBarFront.SetSize(barWidth, barHeight);
    bossHPBarFront.SetPos(480, 80);
    m_bossHPBarFront.push_back(bossHPBarFront);
}

// �{�X�� HP ��ݒ�
void UI::SetBossHP(int hp)
{
    m_currentBossHP = (hp > m_maxBossHP) ? m_maxBossHP : hp;

    // HP�̊�������o�[�̕����v�Z
    float hpRatio = static_cast<float>(m_currentBossHP) / m_maxBossHP;
    int newWidth = static_cast<int>(hpRatio * m_bossHPBarWidth);

    if (!m_bossHPBarFront.empty()) {
        m_bossHPBarFront[0].SetSize(newWidth, m_bossHPBarHeight); // �����͌Œ�
    }
}

// �{�XHP�o�[�̕`��
void UI::RenderBossHP()
{
    // ��������쐬
    char timeText[32];
    sprintf_s(timeText, "DragonHP");

    // �t�H���g�C���X�^���X���쐬�i����̂݁j
    CREATE_FONT_F("customFont", "C:\\Windows\\Fonts\\msgothic.ttc", 50);

    // �����̕\��
    FONT("customFont")->Draw(500, 75, 1.0f, 0.0f, 0.0f, timeText);

    if (!m_bossHPBarBack.empty()) {
        m_bossHPBarBack[0].Draw();  // �w�i��`��
    }
    if (!m_bossHPBarFront.empty()) {
        m_bossHPBarFront[0].Draw(); // HP�o�[��`��
    }
}