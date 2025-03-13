#pragma once

#ifndef UI_H
#define UI_H

/*
* UI
* ユーザーインターフェースクラス
*/

class UI
{
public:
    // コンストラクタ
    UI();

    //デストラクタ
    ~UI();

    // シーン(追加)
    // スタート画面
    void Start(const char* taitleTexture, int gWidth, int gHeight);

    // ゲームクリア画面
    void Clear(const char* taitleTexture, int gWidth, int gHeight);

    // ゲームオーバー
    void Over(const char* taitleTexture, int gWidth, int gHeight);
    //

    // HPアイコンの設定
    void InitHp(const char* hpTexture, int maxHP, int iconWidth, int iconHeight);

    // マナアイコンの設定
    void InitMana(const char* manaTexture, int iconWidth, int iconHeight);

    // 経過時間を設定する関数を追加
    void SetElapsedTime(float time);

    // UIの表示
    void Render();

    // HPを設定
    void SetHP(int hp); 

    // マナを設定
    void SetMana(int mana); 

private:
    // シーン(追加)
    std::vector<CImage> m_titleG; // タイトル画面用

    float m_elapsedTime; // 経過時間を保存

    std::vector<CImage> m_hpIcons; // HPアイコン用
    int m_maxHP; // 最大HP
    int m_currentHP; // 現在のHP

    std::vector<CImage> m_manaIcons; // マナアイコン用
    int m_maxMana; // 最大マナ
    int m_currentMana; // 現在のマナ
};

#endif
