#include "main.h"
#include "manager.h"
#include "scene.h"
#include "renderer.h"
#include "camera.h"
#include "event.h"
#include "cameraEvent.h"

D3DXVECTOR3 CameraEvent::m_Diff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

void CameraEvent::Init()
{
}

void CameraEvent::Uninit()
{
}

void CameraEvent::Update()
{
	// イベント待機中の場合実行しない
	if (m_EventState == EVENT_STATE_OFF) return;

	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();

	D3DXVECTOR3 eventOffset = camera->GetEventOffset(); // 現在のイベントオフセットを取得


	// イベント準備中
	if (m_EventState == EVENT_STATE_READY)
	{
		// 他のカメライベントを終了する
		std::vector<CameraEvent*> cameraEvents = scene->GetGameObjects<CameraEvent>();
		for (CameraEvent* cameraEvent : cameraEvents)
		{
			if (cameraEvent->GetEventState() == EVENT_STATE_ON) cameraEvent->SetEventStateOff();
		}
		// イベント開始
		m_EventState = EVENT_STATE_ON;
	}


	// イベント実行中
	if (m_EventState == EVENT_STATE_ON)
	{
		// オフセットを通常時からイベント時へ変更
		if (m_Contact)
		{
			// X座標の処理
			if (abs(eventOffset.x - m_IdealEventOffset.x) > 0.02f)
			{
				eventOffset.x += m_IdealEventOffset.x / 120.0f;
			}
			if (abs(eventOffset.x - m_IdealEventOffset.x) <= 0.02f)eventOffset.x = m_IdealEventOffset.x;

			// Y座標の処理
			if (abs(eventOffset.y - m_IdealEventOffset.y) > 0.02f)
			{
				eventOffset.y += m_IdealEventOffset.y / 120.0f;
			}
			if (abs(eventOffset.y - m_IdealEventOffset.y) <= 0.02f)eventOffset.y = m_IdealEventOffset.y;

			// Z座標の処理
			if (abs(eventOffset.z - m_IdealEventOffset.z) > 0.02f)
			{
				eventOffset.z += m_IdealEventOffset.z / 120.0f;
			}
			if (abs(eventOffset.z - m_IdealEventOffset.z) <= 0.02f)eventOffset.z = m_IdealEventOffset.z;
		}


		// オフセットをイベント時から通常時へ変更
		else
		{
			// X座標の処理
			if (abs(eventOffset.x - 0.0f) > 0.02f)
			{
				eventOffset.x -= eventOffset.x / 120.0f;
				if (abs(eventOffset.x - 0.0f) <= 0.02f)eventOffset.x = 0.0f;
			}

			// Y座標の処理
			if (abs(eventOffset.y - 0.0f) > 0.02f)
			{
				eventOffset.y -= eventOffset.y / 120.0f;
				if (abs(eventOffset.y - 0.0f) <= 0.02f)eventOffset.y = 0.0f;
			}

			// Z座標の処理
			if (abs(eventOffset.z - 0.0f) > 0.02f)
			{
				eventOffset.z -= eventOffset.z / 120.0f;
				if (abs(eventOffset.z - 0.0f) <= 0.02f)eventOffset.z = 0.0f;
			}
		}
	}
	// 更新したイベントオフセットを格納
	camera->SetEventOffset(eventOffset);

	// 衝突中常に実行しなおすため最期に衝突フラグをfalseに戻す
	m_Contact = false;
}

void CameraEvent::SetCameraEvent(CAMERAEVENT eventNumber)
{
	switch (eventNumber)
	{
	case CAMERAEVENT_TUTORIAL_01:
		m_IdealEventOffset = D3DXVECTOR3(0.0f, -8.0f, 2.0f);
		break;
	case CAMERAEVENT_TUTORIAL_02:
		m_IdealEventOffset = D3DXVECTOR3(0.0f, -2.0f, -7.0f);
		break;
	case CAMERAEVENT_TLOB_01:
		m_IdealEventOffset = D3DXVECTOR3(0.0f, 5.0f, -6.0f);
		break;
	case CAMERAEVENT_TLOB_02:
		m_IdealEventOffset = D3DXVECTOR3(-6.0f, -4.0f, -7.0f);
		break;
	case CAMERAEVENT_RESULT:
		m_IdealEventOffset = D3DXVECTOR3(120.0f, -10.0f, -420.0f);
		break;
	}
}
