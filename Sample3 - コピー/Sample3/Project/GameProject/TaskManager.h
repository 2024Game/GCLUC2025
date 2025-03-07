#pragma once

class Task;
class ObjectBase;

// タスクを管理するクラス
class TaskManager
{
public:
	// TaskManagerのインスタンスを取得
	static TaskManager* Instance();
	// TaskManagerのインスタンスを破棄
	static void ClearInstance();

	// タスクリストにタスクを追加
	void Add(Task* task, bool sort = false);
	// タスクリストからタスクを取り除く
	void Remove(Task* task, bool sort = false);

	// 全てのタスクを削除
	void DeleteAll();
	// 削除フラグが立っているタスクを削除
	void DeleteKilledTasks();

	// リストに登録されているタスクを更新
	void Update();
	// リストに登録されているタスクを描画
	void Render();

private:
	// コンストラクタ
	TaskManager();
	// デストラクタ
	~TaskManager();

	// TaskManagerのインスタンス
	static TaskManager* ms_instance;
	// 生成済みのタスクのリスト
	std::list<Task*> m_taskList;

	// 生成済みのオブジェクトのリスト
	std::list<Task*> m_objectList;
};