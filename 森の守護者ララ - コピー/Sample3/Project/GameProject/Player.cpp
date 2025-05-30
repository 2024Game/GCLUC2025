#include "Player.h"

#define CHIP_SIZE 430		// 1コマのサイズ
#define CENTER_POS CVector2D(1000.0f, 400.0f)	// 中心座標
#define MOVE_SPEED_X 5.0f	// 横方向の移動速度
#define MOVE_SPEED_Z 150.0f	// 奥方向の移動速度
#define JUMP_SPEED 25.0f	// ジャンプ速度
#define GRAVITY -1.0f		// 重力

#define SLIDING_MOVE_SPEED_X 5.0f	// スライディング中の移動速度
#define SLOW_DOWN_SPEED_X 2.0f		// スライディング後のゆっくり移動速度

bool m_slowDownAfterSliding = false;		// スライディング後にゆっくり移動するフラグ

// プレイヤーのアニメーションデータの前宣言
TexAnimData Player::ANIM_DATA[(int)EAnimType::Num] =
{
	// ダッシュアニメーション
	{
		new TexAnim[4]
		{
			{0, 6}, {1, 6}, {2, 6},
			{3, 6}
		},
		4
	},
	// ジャンプアニメーション
	{
		new TexAnim[4]
		{
			{27, 2}, {28, 2}, {29, 2},
			{30, 2}
		},
		4
	},
	// 死亡アニメーション
	{
		new TexAnim[2]
		{
			{45, 12},
			{46, 12},
		},
		2
	},
	// 攻撃アニメーション
	{
		new TexAnim[2]
		{
			{18, 6},
			{19, 6},
		},
		2
	},
	// スライディングアニメーション
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
	// 被ダメアニメーション
	{
		new TexAnim[2]
		{
			{36, 20},
			{37, 20},
		},
		2
	},
};


// コンストラクタ
Player::Player(const CVector3D& pos)
	: CharaBase(pos)
	, m_state(EState::Idle)
	, m_stateStep(0)
	, mp_image(nullptr)
{
	m_hp = 3; // UI に合わせて初期HPを3に

	// プレイヤーの画像を読み込み
	mp_image = CImage::CreateImage
	(
		"elf.png",	// 画像ファイルのパス
		ANIM_DATA,		// アニメーションのデータ
		CHIP_SIZE, CHIP_SIZE	// 1コマの幅と高さ
	);
	mp_image->ChangeAnimation((int)EAnimType::Dash);
	mp_image->SetCenter(CENTER_POS);
}

// デストラクタ
Player::~Player()
{
	// 画像データを削除
	delete mp_image;
}

// 現在の状態を切り替え
void Player::ChangeState(EState state)
{
	if (m_state == state) return;

	m_state = state;
	m_stateStep = 0;
}

// 移動処理の更新
bool Player::UpdateMove()
{
	bool isMove = false;

	/* 左右の移動はいらないので、ここは削除
	// 左キーを押している間
	if (HOLD(CInput::eLeft))
	{
		// 左方向へ移動
		m_pos.x -= MOVE_SPEED_X;
		mp_image->SetFlipH(true);
		isMove = true;
	}
	// 右キーを押している間
	else if (HOLD(CInput::eRight))
	{
		// 右方向へ移動
		m_pos.x += MOVE_SPEED_X;
		mp_image->SetFlipH(false);
		isMove = true;
	}
	*/

	// 仮で作成
	// 左クリックで被ダメ(HPダウン)
	if (PUSH(CInput::eMouseL))
	{
		ChangeState(EState::Damage);
	}



	// 上キーを押した瞬間にZ座標を減らす（奥へ移動）
	if (PUSH(CInput::eUp) && m_pos.z > -150.0f)
	{
		m_pos.z -= MOVE_SPEED_Z;
		isMove = true;
	}
	// 下キーを押した瞬間にZ座標を増やす（手前へ移動）
	else if (PUSH(CInput::eDown) && m_pos.z < 150.0f)
	{
		m_pos.z += MOVE_SPEED_Z;
		isMove = true;
	}

	// スライディング中は速く移動
	if (m_state == EState::Sliding)
	{
		m_pos.x += SLIDING_MOVE_SPEED_X; // 速く移動
		isMove = true;
	}
	// スライディング後は移動速度をゆっくりにする
	else if (m_slowDownAfterSliding)
	{
		m_pos.x += SLOW_DOWN_SPEED_X; // スライディング後の移動速度
		isMove = true;
	}
	// 通常の右移動
	else
	{
		m_pos.x += MOVE_SPEED_X;
		isMove = true;
	}

	// プレイヤーの移動が行われたらカメラをスクロールさせる
	if (isMove) {
		CCamera::ForceCameraScroll(m_pos.x + 40.0f);  // プレイヤーのX位置に基づいてカメラを移動(カメラは常に真ん中にしておく)

		printf("浮動小数点数の値: %.2f\n", m_pos.x + 40.0f);
	}

	return isMove;
}

// ダッシュ時の更新処理
void Player::StateIdle()
{
	// 移動処理
	bool isMove = UpdateMove();

	// ダッシュアニメーションに変更
	mp_image->ChangeAnimation((int)EAnimType::Dash);

	// [SPACE]キーでジャンプ状態へ移行
	if (PUSH(CInput::eButton5))
	{
		ChangeState(EState::Jump);
	}
	// [X]キーで攻撃状態へ移行
	else if (PUSH(CInput::eButton2))
	{
		ChangeState(EState::Attack);
	}
	// [C]キーでスライディング状態へ移行
	else if (PUSH(CInput::eButton3))
	{
		ChangeState(EState::Sliding);
	}
}

// ジャンプ中の更新処理
void Player::StateJump()
{
	// ステップごとに処理を切り替え
	switch(m_stateStep)
	{
		// ステップ0：ジャンプ開始
		case 0:
			// Y軸（高さ）の移動速度にジャンプを速度を設定し、
			// 接地状態を解除する
			m_moveSpeedY = JUMP_SPEED;
			m_isGrounded = false;

			// ジャンプアニメーションに変更
			mp_image->ChangeAnimation((int)EAnimType::Jump, false);

			m_stateStep++;
			break;
		// ステップ1：ジャンプ終了
		case 1:
			// 接地したら、待機状態へ移行
			if (m_isGrounded)
			{
				ChangeState(EState::Idle);
			}
			break;
	}

	// 移動処理
	bool isMove = UpdateMove();

	if (!isMove) {
		mp_image->ChangeAnimation((int)EAnimType::Jump);
	}

	// 以下だと、ジャンプのアニメーションが動かない為、上記に変更
	//mp_image->ChangeAnimation((int)EAnimType::Dash);
}

// 攻撃中の更新処理
void Player::StateAttack()
{
	// ステップごとに処理を切り替え
	switch (m_stateStep)
	{
		// ステップ0：攻撃アニメーションに切り替え
		case 0:
			mp_image->ChangeAnimation((int)EAnimType::Attack, false);
			m_stateStep++;
			break;
		// ステップ1：アニメーション終了待ち
		case 1:
			// 攻撃アニメーションが終了したら、待機状態へ移行
			if (mp_image->CheckAnimationEnd())
			{
				ChangeState(EState::Idle);
			}
			break;
	}
}

// 死亡時の更新処理
void Player::StateDeath()
{
}

// スライディング時の更新処理
void Player::StateSliding()
{
	// ステップごとに処理を切り替え
	switch (m_stateStep)
	{
		// ステップ0：スライディングアニメーションに切り替え
	case 0:
		mp_image->ChangeAnimation((int)EAnimType::Sliding, false);
		m_stateStep++;
		break;

		// ステップ1：アニメーション終了待ち
	case 1:
		// スライディング中の移動（速いスピードで右へ移動）
		m_pos.x += SLIDING_MOVE_SPEED_X;

		// スライディングアニメーションが終了したら、ゆっくり移動を始める
		if (mp_image->CheckAnimationEnd())
		{
			// スライディング後は移動速度を遅くする
			m_slowDownAfterSliding = true;
			ChangeState(EState::Idle);
		}
		break;
	}
}

// 被ダメ中の更新処理
void Player::StateDamage()
{
	// ステップごとに処理を切り替え
	switch (m_stateStep)
	{
		// ステップ0：被ダメアニメーションに切り替え
	case 0:
		mp_image->ChangeAnimation((int)EAnimType::Damage, false);
		m_stateStep++;
		break;
		// ステップ1：アニメーション終了待ち
	case 1:
		// 被ダメアニメーションが終了したら、待機状態へ移行
		if (mp_image->CheckAnimationEnd())
		{
			ChangeState(EState::Idle);
		}
		break;
	}
}

// ダメージを受ける処理
void Player::TakeDamage(int damage) {
	if (m_hp == 0) return; // すでにHPが0なら何もしない

	if (m_hp > damage) {
		m_hp -= damage; // HP を減らす
		
		ChangeState(EState::Damage); // 被ダメ状態へ移行
	}
	else {
		m_hp = 0;
		Death();
	}
}

// 更新処理
void Player::Update()
{
	// 現在の状態に合わせて、処理を切り替える
	switch (m_state)
	{
	case EState::Idle:		StateIdle();	break;
	case EState::Jump:		StateJump();	break;
	case EState::Attack:	StateAttack();	break;
	case EState::Death:		StateDeath();	break;
	case EState::Sliding:	StateSliding();	break;
	case EState::Damage:    StateDamage();  break;
	}

	// Y軸（高さ）の移動を座標に反映
	m_pos.y += m_moveSpeedY;
	m_moveSpeedY += GRAVITY;	// Y軸の移動速度に重力を加算
	// 地面より下にいくと
	if (m_pos.y <= 0.0f)
	{
		// 地面の座標へ戻す
		m_pos.y = 0.0f;
		m_moveSpeedY = 0.0f;
		m_isGrounded = true;
	}

	// イメージに座標を設定して、アニメーションを更新
	mp_image->SetPos(CalcScreenPos());
	mp_image->UpdateAnimation();
}

// 描画処理
void Player::Render()
{
	mp_image->Draw();
}
