#pragma once
#include "gameObject.h"
#include "event.h"

// 各透明な壁の情報
struct InvisibleBoxParameter
{
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Scale;
	float       Length;
	bool        Collision;
};

// 透明オブジェクトクラス
class InvisibleBox : public GameObject
{
private:
	InvisibleBoxParameter m_Param{};

	// イベントオブジェクト
	Event* m_Event;

public:
	// オブジェクトパラメータ取得
	InvisibleBoxParameter GetInvisibleBoxParameter(void) { return m_Param; }
	// イベント設定
	void SetEvent(Event* event) { m_Event = event; }
	// コリジョン設定
	void SetCollision(bool flag) { m_Param.Collision = flag; }
	// イベント実行中
	void RunningEvent(void) { if (!m_Event->GetDestroy()) m_Event->SetEventStateReady(); }

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void DrawZPrePass() override;
};