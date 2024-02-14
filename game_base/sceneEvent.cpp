#include "main.h"
#include "manager.h"
#include "scene.h"
#include "game.h"
//#include "result.h"

#include "renderer.h"
#include "camera.h"
#include "event.h"
#include "sceneEvent.h"
#include "fade.h"


void SceneEvent::Init()
{
}

void SceneEvent::Uninit()
{
}

void SceneEvent::Update()
{
	// イベント待機中の場合実行しない
	if (m_EventState == EVENT_STATE_OFF) return;

	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();


	// イベント準備中
	if (m_EventState == EVENT_STATE_READY)
	{
		// イベント開始
		m_EventState = EVENT_STATE_ON;
		scene->GetGameObject<Fade>()->SetFadeIn();
	}

	// イベント実行中
	if (m_EventState == EVENT_STATE_ON)
	{
		Scene* scene = Manager::GetScene();
		Fade* fade = scene->GetGameObject<Fade>();
		if (fade->GetFadeFinished()) SetDestroy();
	}
}

void SceneEvent::SetSceneEvent(SCENEEVENT eventNumber)
{
	m_SceneEvent = eventNumber;
}
