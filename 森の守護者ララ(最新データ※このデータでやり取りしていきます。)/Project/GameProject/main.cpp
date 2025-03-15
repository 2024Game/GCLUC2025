#include "TaskManager.h"
#include "Player.h"
#include "Slime.h"
#include "Mosubi.h"
#include "Rock.h"

//UI関係
#include "UI.h"
#include <chrono>
//

//--------------------------------------------
//グローバル変数領域
//--------------------------------------------
Player* g_player = nullptr;	// プレイヤーのポインタ
Slime* g_slime = nullptr;	// キューゴンのポインタ
Mosubi* g_mosubi = nullptr; // モスビのポインタ
Rock* g_rock = nullptr;     // 岩のポインタ

CImage* g_field = nullptr;	// フィールド画像のポインタ

// UI関係
UI* g_ui = nullptr;      // UI クラスのポインタ
std::chrono::steady_clock::time_point g_startTime; // ゲーム開始時間
//

// シーン関係
enum class EState
{
	ESTART,  //ゲーム開始
	EPLAY,   //ゲーム中
	ECLEAR,  //ゲームクリア
	EOVER,   //ゲームオーバー
};

EState mState;

bool isGame; // ゲームの判定
bool isStart; // 経過時間の判定
bool isDeath = false; // 死亡の判定(仮)
float deathCount = 0.0f; // 死亡カウント(仮)

/*
// 障害物のリスト
std::vector<Rock*> g_rocks;
std::vector<Slime*> g_slimes;
std::vector<Mosubi*> g_mosubis;
*/

void MainLoop()
{
	//--------------------------------------------------------------
	//ゲーム中の動きはここに書く
	//ゲーム中はこの関数_を1秒間に60回呼び出している
	//--------------------------------------------------------------

	// シーン関係
	switch (mState)
	{
	case EState::ESTART: // ゲームスタート状態だったら、
		isGame = false; // ゲーム(MainLoop())を止める
		g_ui->Start("title.png", 1920, 1080); // タイトル画面の描画

		if (PUSH(CInput::eButton10)) // Enterキーが押されたら、
		{
			isStart = true; // 経過時間(タイム)を動かす
			mState = EState::EPLAY; // ゲームプレイ状態にする
		}
		break;
	case EState::EPLAY: // ゲームプレイ状態だったら、
		isGame = true; // ゲーム(MainLoop())を動かす
		if (PUSH(CInput::eButton10)) // Enterキーが押されたら、
		{
			isStart = false; // 経過時間(タイム)を止める
			mState = EState::ECLEAR; // ゲームスタート状態にする
		}
		break;
	case EState::ECLEAR: // ゲームクリア状態だったら、
		isGame = false; // ゲーム(MainLoop())を止める

		// Enterキーを押すとゲーム終了
		if (PUSH(CInput::eButton10)) // Enterキーが押されたら
		{
			// **クリーンアップ処理**
			TaskManager::ClearInstance(); // ゲームのインスタンスを削除

			// **プログラムを安全に終了**
			PostQuitMessage(0); // Windowsアプリの正しい終了方法
			// exit(0); // 代わりに exit(0) でもOK
		}

		/*if (PUSH(CInput::eButton10)) // Enterキーが押されたら、
		{
			// **ゲームの初期化
			// ゲームのインスタンス削除(追加)
			TaskManager::ClearInstance(); // 既存のインスタンスを削除

			// ゲームのインスタンス作成(追加)
			TaskManager::Instance(); // 新しいインスタンスを作成

			// ここでプレイヤーや敵キャラを再作成(Init()を呼ぶとエラーになる)
			// フィールドの画像を生成
			

			// プレイヤーを生成
			

			// 障害物を生成
			
			// UI関係
			// UI を生成
			

			// 全てのタスクを更新
			TaskManager::Instance()->Update();			

			mState = EState::ESTART; // ゲームスタート状態にする
		}*/
		break;
	case EState::EOVER: // ゲームオーバー状態だったら、
		isGame = false; // ゲーム(MainLoop())を止める

		// Enterキーを押すとゲーム終了
		if (PUSH(CInput::eButton10)) // Enterキーが押されたら
		{
			// **クリーンアップ処理**
			TaskManager::ClearInstance(); // ゲームのインスタンスを削除
			/*if (g_player) {
				delete g_player;
				g_player = nullptr;
			}
			if (g_field) {
				delete g_field;
				g_field = nullptr;
			}
			if (g_ui) {
				delete g_ui;
				g_ui = nullptr;
			}
			if (g_slime) {
				delete g_slime;
				g_slime = nullptr;
			}
			if (g_mosubi) {
				delete g_mosubi;
				g_mosubi = nullptr;
			}
			if (g_rock) {
				delete g_rock;
				g_rock = nullptr;
			}*/

			// **プログラムを安全に終了**
			PostQuitMessage(0); // Windowsアプリの正しい終了方法
			// exit(0); // 代わりに exit(0) でもOK
		}

		break;
	}

	// 死亡判定(仮)
	if (isDeath)
	{
		mState = EState::EOVER;
		isDeath = false;
	}

	// ゲーム中(EState::EPLAY)なら、以下を実行
	if (isGame)
	{
		// ゲーム中なら経過時間を最初に0にする
		if (isStart)
		{
			// ゲーム開始時間を記録
			g_startTime = std::chrono::steady_clock::now();
			//
			isStart = false;
		}

		// UI関係
		// 経過時間を計算
		auto now = std::chrono::steady_clock::now();
		float elapsedTime = std::chrono::duration<float>(now - g_startTime).count();

		// UI に経過時間をセット
		g_ui->SetElapsedTime(elapsedTime);

		// 全てのタスクを更新
		TaskManager::Instance()->Update();

		//　全ての当たり判定のチェック
		TaskManager::Instance()->CheckCollisions();

		// フィールド画像を描画
		g_field->Draw();

		// 影の描画
		g_player->RenderShadow();
		g_slime->RenderShadow();
		g_mosubi->RenderShadow();

		// 全てのタスクを描画
		TaskManager::Instance()->Render();

		// UI関係
		// UI の描画
		g_ui->Render();

		// HPを共有
		g_ui->SetHP(Player::Hp());

		// 死亡判定(仮)
		if (Player::Hp() == 0)
		{
			deathCount++;
			if (deathCount >= 110.0f)
			{
				isDeath = true;
				deathCount = 0.0f;
			}
		}

		// デバッグ文字の描画
		DebugPrint::Render();
	}
}
void Init()
{
	CFPS::SetFPS(60);
	//フレーム制御初期化
	CFPS::Init();
	//ボタンの設定
	CInput::Init();
	CInput::SetButton(0, CInput::eButton1, 'Z');
	CInput::SetButton(0, CInput::eButton2, 'X');
	CInput::SetButton(0, CInput::eButton3, 'C');
	CInput::SetButton(0, CInput::eButton4, 'V');
	CInput::SetButton(0, CInput::eButton5, VK_SPACE);
	CInput::SetButton(0, CInput::eButton10, VK_RETURN);
	CInput::SetButton(0, CInput::eUp, VK_UP);
	CInput::SetButton(0, CInput::eDown, VK_DOWN);
	CInput::SetButton(0, CInput::eLeft, VK_LEFT);
	CInput::SetButton(0, CInput::eRight, VK_RIGHT);
	CInput::SetButton(0, CInput::eMouseL, VK_LBUTTON);
	CInput::SetButton(0, CInput::eMouseR, VK_RBUTTON);
	CInput::SetButton(0, CInput::eMouseC, VK_MBUTTON);
	//	CInput::SetMouseInside(true);
	//	CInput::ShowCursor(false);
	CInput::Update();
	CInput::Update();

	SetCurrentDirectory("data");
	CSound::GetInstance();

	//-----------------------------------------------------
	//初期化の命令を書く
	//ゲーム起動時に一度だけ呼ばれる
	//-----------------------------------------------------

	// シーン関係(追加)
	// 最初はタイトル画面を表示
	mState = EState::ESTART;

	// フィールドの画像を生成
	g_field = CImage::CreateImage("background.png");

	// プレイヤーを生成
	g_player = new Player(
		CVector3D(SCREEN_WIDTH * 0.0f, 0.0f, -80.0f));

	///*
	// 障害物を生成
	// キューゴン
	g_slime = new Slime(0,
		CVector3D(SCREEN_WIDTH * 0.40f, 0.0f, 0.0f));

	// モスビ
	g_mosubi = new Mosubi(0,
		CVector3D(SCREEN_WIDTH * 0.80f, 160.0f, 0.0f));

	// 岩
	g_rock = new Rock(0,
		CVector3D(SCREEN_WIDTH * 0.40f, 100.0f, 150.0f));
	g_rock = new Rock(0,
		CVector3D(SCREEN_WIDTH * 0.40f, 100.0f, -150.0f));
	g_rock = new Rock(0,
		CVector3D(SCREEN_WIDTH * 0.80f, 100.0f, -150.0f));

	// UI関係
	// UI を生成
	g_ui = new UI();
	g_ui->InitHp("HPUI.png", 3, 100, 100); // HPアイコン画像をセット
	g_ui->InitMana("mana.png", 100, 100); // マナアイコン画像をセット
}


void Release()
{
	DebugPrint::ClearInstance();
	CLoadThread::ClearInstance();
	CSound::ClearInstance();
	CResourceManager::ClearInstance();
	TaskManager::ClearInstance();

	// UI関係(追加)
	// UI を解放
	delete g_ui;
	g_ui = nullptr;
	//
}

static void ResizeCallback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);

	//画面解像度変動
	CCamera::GetCamera()->SetSize((float)w, (float)h);
	//画面解像度固定
	//CCamera::GetCamera()->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	CCamera::GetCamera()->Viewport(0, 0, w, h);
	CCamera::GetCurrent()->Perspective(DtoR(60.0), (float)w / (float)h, 1.0, 1000.0);

	glfwGetWindowPos(window, &GL::window_x, &GL::window_y);
	GL::UpdateWindowRect(GL::window_x, GL::window_y, w, h);
	CInput::UpdateClipCursor(true);

}
static void WheelCallback(GLFWwindow* _window, double _offsetx, double _offsety)
{
	CInput::AddMouseWheel((int)_offsety);
}
static void PosCallback(GLFWwindow* _window, int x, int y)
{
	GL::window_x = x;
	GL::window_y = y;
	GL::UpdateWindosRect(x, y, GL::window_width, GL::window_height);
	CInput::UpdateClipCursor(true);
}
static void FocusCallback(GLFWwindow* _window, int f)
{
	CInput::UpdateClipCursor(f);
	GL::focus = f;
}

static void error_callback(int error, const char* description)
{
	printf("Error: %s\n", description);
}

//フルスクリーン?ウインドウモードの切り替え
//Alt+Enterで切り替える
void CheckFullScreen()
{
	static int key_enter = 0;
	int key_enter_buf = key_enter;
	if (key_enter_buf ^ (key_enter = glfwGetKey(GL::window, GLFW_KEY_ENTER)) && key_enter && (glfwGetKey(GL::window, GLFW_KEY_LEFT_ALT) || glfwGetKey(GL::window, GLFW_KEY_RIGHT_ALT)))
	{
		GL::ChangeFullScreen(!GL::full_screen);
	}
}

int __main(int* argcp, char** argv)
{
	// メモリリーク検出
	//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);


	//OpenGL4.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	glfwSetErrorCallback(error_callback);
	if (!glfwInit()) return -1;
	//	glutInit(argcp, argv);

	GL::window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Simple", nullptr, nullptr);
	glfwGetWindowSize(GL::window, &GL::window_width, &GL::window_height);
	glfwGetWindowPos(GL::window, &GL::window_x, &GL::window_y);

	glfwSetFramebufferSizeCallback(GL::window, ResizeCallback);
	glfwSetScrollCallback(GL::window, WheelCallback);
	glfwSetWindowFocusCallback(GL::window, FocusCallback);
	glfwSetWindowPosCallback(GL::window, PosCallback);
	if (!GL::window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(GL::window);
	glfwSwapInterval(1);
	ResizeCallback(GL::window, SCREEN_WIDTH, SCREEN_HEIGHT);

	GLenum err = glewInit();
	if (err == GLEW_OK)
	{
		printf("%s\n", glewGetString(GLEW_VERSION));
	}
	else
	{
		printf("%s\n", glewGetErrorString(err));
		getchar();
		return -1;
	}
	HDC glDc = wglGetCurrentDC();
	GL::hWnd = WindowFromDC(glDc);
	glClearColor(0.0, 0.0, 0.0, 1.0);

	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);//ブレンドの有効化
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//固定シェーダー用
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_ALPHA_TEST);

	Init();
	while (!glfwWindowShouldClose(GL::window))
	{
		CheckFullScreen();


		CInput::Update();
		//各バッファーをクリア
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		MainLoop();


		glfwSwapBuffers(GL::window);


		CFPS::Wait();

		char title[32];
		sprintf_s(title, "Lara, Guardian of the Forest");
		glfwSetWindowTitle(GL::window, title);

		glfwPollEvents();
		if (glfwGetKey(GL::window, GLFW_KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(GL::window, GL_TRUE);
		}

	}

	glfwTerminate();

	Release();
	return 0;
}

INT WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR argv, INT argc)
{
	return __main(&__argc, __argv);
}

int main(int argc, char** argv)
{
	return __main(&argc, argv);
}
