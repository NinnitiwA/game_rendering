#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "eventManager.h"
#include "cameraEvent.h"
#include "textEvent.h"
#include "sceneEvent.h"
#include "invisibleBox.h"

void EventManager::InitTutorialStage()
{
	Scene* scene = Manager::GetScene();

	// �e�L�X�g�C�x���g
	{// �C�x���gNO.1 �W�����v
		InvisibleBox* invisibleBox = scene->AddGameObject<InvisibleBox>(1);
		invisibleBox->SetPosition(D3DXVECTOR3(40.0f, 3.0f, 10.0f));
		invisibleBox->SetScale(D3DXVECTOR3(25.0f, 2.0f, 10.0f));
		TextEvent* textEvent = scene->AddGameObject<TextEvent>(3);
		textEvent->SetTextEvent(TEXTEVENT_TUTORIAL_JUMP);
		invisibleBox->SetEvent(textEvent);
	}
	{// �C�x���gNO.2 �U��1
		InvisibleBox* invisibleBox = scene->AddGameObject<InvisibleBox>(1);
		invisibleBox->SetPosition(D3DXVECTOR3(100.0f, 6.0f, 80.0f));
		invisibleBox->SetScale(D3DXVECTOR3(10.0f, 2.0f, 20.0f));
		TextEvent* textEvent = scene->AddGameObject<TextEvent>(3);
		textEvent->SetTextEvent(TEXTEVENT_TUTORIAL_PRESSATTACK);
		invisibleBox->SetEvent(textEvent);
	}
	{// �C�x���gNO.3 �U��2
		InvisibleBox* invisibleBox = scene->AddGameObject<InvisibleBox>(1);
		invisibleBox->SetPosition(D3DXVECTOR3(155.0f, 6.0f, 80.0f));
		invisibleBox->SetScale(D3DXVECTOR3(20.0f, 2.0f, 20.0f));
		TextEvent* textEvent = scene->AddGameObject<TextEvent>(3);
		textEvent->SetTextEvent(TEXTEVENT_TUTORIAL_CHARGEATTACK);
		invisibleBox->SetEvent(textEvent);
	}
	{// �C�x���gNO.4 �_�b�V��
		InvisibleBox* invisibleBox = scene->AddGameObject<InvisibleBox>(1);
		invisibleBox->SetPosition(D3DXVECTOR3(160.0f, 6.0f, 130.0f));
		invisibleBox->SetScale(D3DXVECTOR3(20.0f, 2.0f, 10.0f));
		TextEvent* textEvent = scene->AddGameObject<TextEvent>(3);
		textEvent->SetTextEvent(TEXTEVENT_TUTORIAL_DODGE);
		invisibleBox->SetEvent(textEvent);
	}


	// �J�����C�x���g
	{// �C�x���gNO.1
		InvisibleBox* invisibleBox = scene->AddGameObject<InvisibleBox>(1);
		invisibleBox->SetPosition(D3DXVECTOR3(160.0f, 7.5f, 260.0f));
		invisibleBox->SetScale(D3DXVECTOR3(20.0f, 6.0f, 60.0f));
		CameraEvent* cameraEvent = scene->AddGameObject<CameraEvent>(3);
		cameraEvent->SetCameraEvent(CAMERAEVENT_TUTORIAL_01);
		invisibleBox->SetEvent(cameraEvent);
	}

	// �V�[���C�x���g
	{// �Q�[���V�[���ɐ���
		InvisibleBox* invisibleBox = scene->AddGameObject<InvisibleBox>(1);
		invisibleBox->SetPosition(D3DXVECTOR3(160.0f, 7.5f, 275.0f));
		invisibleBox->SetScale(D3DXVECTOR3(20.0f, 6.0f, 10.0f));
		SceneEvent* sceneEvent = scene->AddGameObject<SceneEvent>(3);
		sceneEvent->SetSceneEvent(SCENEEVENT_TUTORIAL_GAME);
		invisibleBox->SetEvent(sceneEvent);
	}
}

void EventManager::InitTheLandOfBeginnings(D3DXVECTOR3 areaPosition)
{
	Scene* scene = Manager::GetScene();

	// �e�L�X�g�C�x���g
	{// �C�x���gNO.1
		InvisibleBox* invisibleBox = scene->AddGameObject<InvisibleBox>(1);
		invisibleBox->SetPosition(D3DXVECTOR3(0.0f, 3.0f, 15.0f) + areaPosition);
		invisibleBox->SetScale(D3DXVECTOR3(20.0f, 6.0f, 4.0f));
		TextEvent* textEvent = scene->AddGameObject<TextEvent>(3);
		textEvent->SetTextEvent(TEXTEVENT_TLOB_TITLE);
		invisibleBox->SetEvent(textEvent);
	}

	// �J�����C�x���g
	{// �C�x���gNO.1
		InvisibleBox* invisibleBox = scene->AddGameObject<InvisibleBox>(1);
		invisibleBox->SetPosition(D3DXVECTOR3(0.0f, 9.0f, 0.0f) + areaPosition);
		invisibleBox->SetScale(D3DXVECTOR3(20.0f, 8.0f, 60.0f));
		CameraEvent* cameraEvent = scene->AddGameObject<CameraEvent>(3);
		cameraEvent->SetCameraEvent(CAMERAEVENT_TLOB_01);
		invisibleBox->SetEvent(cameraEvent);
	}
	{// �C�x���gNO.2
		InvisibleBox* invisibleBox = scene->AddGameObject<InvisibleBox>(1);
		invisibleBox->SetPosition(D3DXVECTOR3(70.0f, 10.0f, 240.0f) + areaPosition);
		invisibleBox->SetScale(D3DXVECTOR3(20.0f, 6.0f, 20.0f));
		CameraEvent* cameraEvent = scene->AddGameObject<CameraEvent>(3);
		cameraEvent->SetCameraEvent(CAMERAEVENT_TLOB_02);
		invisibleBox->SetEvent(cameraEvent);
	}
}

void EventManager::InitTheCrystalFountainhead(D3DXVECTOR3 areaPosition)
{
	Scene* scene = Manager::GetScene();

	// �e�L�X�g�C�x���g
	{
		InvisibleBox* invisibleBox = scene->AddGameObject<InvisibleBox>(1);
		invisibleBox->SetPosition(D3DXVECTOR3(40.0f, 3.0f, 15.0f) + areaPosition);
		invisibleBox->SetScale(D3DXVECTOR3(20.0f, 6.0f, 4.0f));
		TextEvent* textEvent = scene->AddGameObject<TextEvent>(3);
		textEvent->SetTextEvent(TEXTEVENT_TCF_TITLE);
		invisibleBox->SetEvent(textEvent);
	}
}

void EventManager::InitTheSkyIsland(D3DXVECTOR3 areaPosition)
{
	Scene* scene = Manager::GetScene();

	// �e�L�X�g�C�x���g
	{
		InvisibleBox* invisibleBox = scene->AddGameObject<InvisibleBox>(1);
		invisibleBox->SetPosition(D3DXVECTOR3(-20.0f, 3.0f, 25.0f) + areaPosition);
		invisibleBox->SetScale(D3DXVECTOR3(20.0f, 6.0f, 4.0f));
		TextEvent* textEvent = scene->AddGameObject<TextEvent>(3);
		textEvent->SetTextEvent(TEXTEVENT_TSI_TITLE);
		invisibleBox->SetEvent(textEvent);
	}

}

void EventManager::InitResult()
{
	Scene* scene = Manager::GetScene();

	// �e�L�X�g�C�x���g
	{
		Scene* scene = Manager::GetScene();
		TextEvent* textEvent = scene->AddGameObject<TextEvent>(3);
		textEvent->SetTextEvent(TEXTEVENT_END);
		textEvent->SetEventStateReady();
	}

	// �J�����C�x���g
	{
		CameraEvent* cameraEvent = scene->AddGameObject<CameraEvent>(3);
		cameraEvent->SetPosition(D3DXVECTOR3(-40.0f, 30.0f, 520.0f));
		cameraEvent->SetCameraEvent(CAMERAEVENT_RESULT);
		cameraEvent->SetEventStateReady();
	}
}

void EventManager::SwitchTextEvent()
{
	Scene* scene = Manager::GetScene();
	std::vector<TextEvent*> textEvents = scene->GetGameObjects<TextEvent>();
	for (TextEvent* textEvent : textEvents)
	{
		if (textEvent->GetEventState() == EVENT_STATE_ON)textEvent->SetDestroy();
	}
}
