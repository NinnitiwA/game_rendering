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
	// �C�x���g�ҋ@���̏ꍇ���s���Ȃ�
	if (m_EventState == EVENT_STATE_OFF) return;

	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();

	D3DXVECTOR3 eventOffset = camera->GetEventOffset(); // ���݂̃C�x���g�I�t�Z�b�g���擾


	// �C�x���g������
	if (m_EventState == EVENT_STATE_READY)
	{
		// ���̃J�����C�x���g���I������
		std::vector<CameraEvent*> cameraEvents = scene->GetGameObjects<CameraEvent>();
		for (CameraEvent* cameraEvent : cameraEvents)
		{
			if (cameraEvent->GetEventState() == EVENT_STATE_ON) cameraEvent->SetEventStateOff();
		}
		// �C�x���g�J�n
		m_EventState = EVENT_STATE_ON;
	}


	// �C�x���g���s��
	if (m_EventState == EVENT_STATE_ON)
	{
		// �I�t�Z�b�g��ʏ펞����C�x���g���֕ύX
		if (m_Contact)
		{
			// X���W�̏���
			if (abs(eventOffset.x - m_IdealEventOffset.x) > 0.02f)
			{
				eventOffset.x += m_IdealEventOffset.x / 120.0f;
			}
			if (abs(eventOffset.x - m_IdealEventOffset.x) <= 0.02f)eventOffset.x = m_IdealEventOffset.x;

			// Y���W�̏���
			if (abs(eventOffset.y - m_IdealEventOffset.y) > 0.02f)
			{
				eventOffset.y += m_IdealEventOffset.y / 120.0f;
			}
			if (abs(eventOffset.y - m_IdealEventOffset.y) <= 0.02f)eventOffset.y = m_IdealEventOffset.y;

			// Z���W�̏���
			if (abs(eventOffset.z - m_IdealEventOffset.z) > 0.02f)
			{
				eventOffset.z += m_IdealEventOffset.z / 120.0f;
			}
			if (abs(eventOffset.z - m_IdealEventOffset.z) <= 0.02f)eventOffset.z = m_IdealEventOffset.z;
		}


		// �I�t�Z�b�g���C�x���g������ʏ펞�֕ύX
		else
		{
			// X���W�̏���
			if (abs(eventOffset.x - 0.0f) > 0.02f)
			{
				eventOffset.x -= eventOffset.x / 120.0f;
				if (abs(eventOffset.x - 0.0f) <= 0.02f)eventOffset.x = 0.0f;
			}

			// Y���W�̏���
			if (abs(eventOffset.y - 0.0f) > 0.02f)
			{
				eventOffset.y -= eventOffset.y / 120.0f;
				if (abs(eventOffset.y - 0.0f) <= 0.02f)eventOffset.y = 0.0f;
			}

			// Z���W�̏���
			if (abs(eventOffset.z - 0.0f) > 0.02f)
			{
				eventOffset.z -= eventOffset.z / 120.0f;
				if (abs(eventOffset.z - 0.0f) <= 0.02f)eventOffset.z = 0.0f;
			}
		}
	}
	// �X�V�����C�x���g�I�t�Z�b�g���i�[
	camera->SetEventOffset(eventOffset);

	// �Փ˒���Ɏ��s���Ȃ������ߍŊ��ɏՓ˃t���O��false�ɖ߂�
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
