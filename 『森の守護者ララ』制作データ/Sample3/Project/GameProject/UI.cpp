#include "UI.h"
#include "DebugPrint.h"
#include <cstdio>

// コンストラクタ
UI::UI() : m_elapsedTime(0), m_maxHP(3), m_currentHP(3), m_currentMana(0) {}

// デストラクタ
UI::~UI() {}

// HPアイコンの設定
void UI::InitHp(const char* hpTexture, int maxHP, int iconWidth, int iconHeight) {
    m_maxHP = maxHP;
    m_currentHP = maxHP;
    m_hpIcons.clear();

    // HPアイコンをロード
    for (int i = 0; i < maxHP; ++i) {
        CImage hpIcon;
        hpIcon.Load(hpTexture);
        hpIcon.SetSize(iconWidth, iconHeight);  // アイコンのサイズを設定
        hpIcon.SetPos(20 + i * 100, 30); // 左上に3つ並べる
        m_hpIcons.push_back(hpIcon);
    }
}

// マナアイコンの設定
void UI::InitMana(const char* manaTexture, int iconWidth, int iconHeight) {
    m_currentMana = 1;  // 初期マナは 0
    m_manaIcons.clear();

    CImage manaIcon;
    manaIcon.Load(manaTexture);
    manaIcon.SetSize(iconWidth, iconHeight);
    manaIcon.SetPos(20, 130); // マナアイコンの位置（HPの下）

    m_manaIcons.push_back(manaIcon);
}

// 経過時間を設定する関数
void UI::SetElapsedTime(float time) {
    m_elapsedTime = time;
}

// UIの表示
void UI::Render()
{
    // 文字列の作成
    char timeText[32];
    sprintf_s(timeText, "Time: %.2f sec", m_elapsedTime);
    // 文字列を画面に表示
    // フォントインスタンスを作成(PCに入っているフォント)
    CREATE_FONT_F("customFont", "C:\\Windows\\Fonts\\msgothic.ttc", 80); // サイズ80のMSゴシックを使用
    // フォントを使って文字列を描画(横位置(左基準)、縦位置(上基準)、R、G、B、表示するテキスト)
    FONT("customFont")->Draw(620, 100, 0.8f, 0.3f, 0.5f, timeText, "Time: %.2f sec", m_elapsedTime);

    // HPアイコン描画
    for (int i = 0; i < m_currentHP; ++i) {
        m_hpIcons[i].Draw();
    }

    // マナアイコン描画（1つだけ表示）
    if (m_currentMana > 0 && !m_manaIcons.empty()) {
        m_manaIcons[0].Draw();
    }
}

// HPを設定
void UI::SetHP(int hp) {
    m_currentHP = hp;
}

// マナを設定（1つまで）
void UI::SetMana(int mana) {
    m_currentMana = (mana > 0) ? 1 : 0; // 1以上なら1に制限
}
