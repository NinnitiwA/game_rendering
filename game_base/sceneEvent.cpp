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
	// �C�x���g�ҋ@���̏ꍇ���s���Ȃ�
	if (m_EventState == EVENT_STATE_OFF) return;

	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();


	// �C�x���g������
	if (m_EventState == EVENT_STATE_READY)
	{
		// �C�x���g�J�n
		m_EventState = EVENT_STATE_ON;
		scene->GetGameObject<Fade>()->SetFadeIn();
	}

	// �C�x���g���s��
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
