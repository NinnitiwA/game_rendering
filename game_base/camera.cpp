#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "input.h"
#include "player.h"
#include "animationObject.h"


D3DXMATRIX Camera::m_ViewMatrix = {};
D3DXMATRIX Camera::m_ProjectionMatrix = {};
D3DXMATRIX Camera::m_ReflectionMatrix = {};

void Camera::Init()
{
	m_Position = D3DXVECTOR3( 0.0f, 8.0f, -12.0f );
	m_Target = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_OldTarget = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_FixedDistance = D3DXVECTOR3(0.0f, 16.0f, -14.0f);
	m_EffectOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_EventOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_FovY = 60.0f * D3DX_PI / 180.0f;
	m_Time = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}


void Camera::Uninit()
{

}


void Camera::Update()
{
	Scene* scene = Manager::GetScene();
	D3DXVECTOR3 playerPos = scene->GetGameObject<Player>()->GetPosition();
	GUI::target = m_Target;
	m_EffectOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);


	// �J�����R���g���[�����̏���
	if (scene->GetSceneName() == TITLE_SCENE)
	{
		ActTitleSceneCamera();
		return;
	}


	// �J�����R���g���[�����̏���
	if (GUI::cameraControllFlag)
	{
		m_Target = D3DXVECTOR3(playerPos.x, 0.0f, playerPos.z) + GUI::targetDebugOffset;
		m_Position = m_Target + m_FixedDistance + GUI::cameraDebugOffset + D3DXVECTOR3(sinf(GUI::cameraRotation) * 30.0f, -playerPos.y, 15.0f -cosf(GUI::cameraRotation) * 30.0f);
		return;
	}


	// �J�����̐U��
	if (m_CameraOscillationFrame < 10)
	{
		switch (m_CameraOscillationMode)
		{
		case CO_LARGE_MODE:
			m_EffectOffset = D3DXVECTOR3((5.0f - (float)(rand() % 10)) / 10.0f, (5.0f - (float)(rand() % 10)) / 10.0f, (5.0f - (float)(rand() % 10)) / 10.0f);
			break;
		case CO_MEDIUM_MODE:
			m_EffectOffset = D3DXVECTOR3((3.0f - (float)(rand() % 6)) / 6.0f, (3.0f - (float)(rand() % 6)) / 6.0f, (3.0f - (float)(rand() % 6)) / 6.0f);
			break;
		case CO_SMALL_MODE:
			m_EffectOffset = D3DXVECTOR3((1.5f - (float)(rand() % 3)) / 3.0f, (1.5f - (float)(rand() % 3)) / 3.0f, (1.5f - (float)(rand() % 3)) / 3.0f) * 0.5f;
			break;
		default:
			break;
		}
		// �t���[���X�V
		m_CameraOscillationFrame++;
	}



	// �J�����̃^�[�Q�b�g���W�ݒ�
	m_Target.x = playerPos.x;
	m_Target.z = playerPos.z;

	// Y���W�͗h�ꂪ�������Ȃ邽�߈��l�ȉ��ł͍l�����Ȃ�
	if (abs(playerPos.y - m_Target.y) >= 0.03f)
	{
		// �����Ă���Ƃ��̏����ȗh��͍l�����Ȃ�
		if (playerPos.y < 1.5f && abs(playerPos.y - m_Target.y) <= 0.15f)
		{
			m_Target.y = m_OldTarget.y;
		}
		// �������v���C���[��Y���W�֋߂Â�
		else if (abs(playerPos.y - m_Target.y) >= 0.15f)
		{
			m_Target.y += (playerPos.y - m_Target.y) / 60.0f;
		}
	}
	else
	{
		m_Target.y = playerPos.y;
	}
	m_Target += m_EffectOffset;

	// �^�[�Q�b�g���W���L�^
	m_OldTarget = m_Target;
	// �J�����̍��W�𔽉f
	m_Position = m_Target + m_EffectOffset + m_FixedDistance + m_EventOffset;
}


void Camera::Draw()
{
	// �r���[�}�g���N�X�ݒ�
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&m_ViewMatrix, &m_Position, &m_Target, &up);

	Renderer::SetViewMatrix(&m_ViewMatrix);

	// �v���W�F�N�V�����}�g���N�X�ݒ�
	D3DXMatrixPerspectiveFovLH(&m_ProjectionMatrix, m_FovY, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 3000.0f);

	Renderer::SetProjectionMatrix(&m_ProjectionMatrix);


	// ���˃}�g���N�X�v�Z
	D3DXVECTOR3 pos = m_Position;
	D3DXVECTOR3 target = m_Target;
	pos.y = fabs(m_Position.y);
	target.y = fabs(m_Target.y);
	float reflectionHeight = 0.0f * 2; // ���ʂ̍���
	D3DXVECTOR3 reflectPos(pos.x, -pos.y + reflectionHeight, pos.z);
	D3DXVECTOR3 reflectTarget(target.x, -target.y, target.z);
	D3DXMatrixLookAtLH(&m_ReflectionMatrix, &reflectPos, &reflectTarget, &up);
	D3DXMatrixTranspose(&m_ReflectionMatrix, &m_ReflectionMatrix);


	// �J�����萔�o�b�t�@�ݒ�--------------------------------------------------------
	CAMERA camera{};

	// ��������W�v�Z
	D3DXMATRIX vp, invvp;

	vp = m_ViewMatrix * m_ProjectionMatrix;
	D3DXMatrixInverse(&invvp, NULL, &vp);

	D3DXVECTOR3 vpos[4]; // �v���W�F�N�V�������W

	vpos[0] = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
	vpos[1] = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	vpos[2] = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
	vpos[3] = D3DXVECTOR3(1.0f, -1.0f, 1.0f);

	// ������S���_
	D3DXVec3TransformCoord(&vpos[0], &vpos[0], &invvp);
	D3DXVec3TransformCoord(&vpos[1], &vpos[1], &invvp);
	D3DXVec3TransformCoord(&vpos[2], &vpos[2], &invvp);
	D3DXVec3TransformCoord(&vpos[3], &vpos[3], &invvp);

	camera.FrustumPosition[0] = D3DXVECTOR4(vpos[0], 1.0f);
	camera.FrustumPosition[1] = D3DXVECTOR4(vpos[1], 1.0f);
	camera.FrustumPosition[2] = D3DXVECTOR4(vpos[2], 1.0f);
	camera.FrustumPosition[3] = D3DXVECTOR4(vpos[3], 1.0f);

	// �J�������W
	camera.Position = D3DXVECTOR4(m_Position, 1.0f);
	camera.Time = m_Time;
	camera.ReflectionMatrix = m_ReflectionMatrix;
	Renderer::SetCamera(camera);
}

void Camera::DrawReflection()
{
	// �r���[�}�g���N�X�ݒ�
	D3DXVECTOR3 pos = m_Position;
	D3DXVECTOR3 target = m_Target;
	pos.y = -fabs(m_Position.y) - 1.0f;
	target.y = -fabs(m_Target.y);

	D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&m_ViewMatrix, &pos, &target, &up);
	Renderer::SetViewMatrix(&m_ViewMatrix);

	//�v���W�F�N�V�����}�g���N�X�ݒ�
	D3DXMatrixPerspectiveFovLH(&m_ProjectionMatrix, 1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 3000.0f);
	Renderer::SetProjectionMatrix(&m_ProjectionMatrix);


	// �J�����p�����[�^�ݒ�
	CAMERA camera;
	camera.Position = D3DXVECTOR4(pos, 1.0f);
	m_Time.r += 0.001f;
	camera.Time = m_Time;
	Renderer::SetCamera(camera);
}

void Camera::DrawCascadeShadowMap(int i)
{
	// �J�X�P�[�h�V���h�E����-----------------------------------------------------------

	// ������̊e���_�v�Z---------------------------------------------------------------

	// ������x�N�g�����v�Z
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	// �O�����x�N�g�����v�Z
	D3DXVECTOR3 forward = m_Target - m_Position;
	D3DXVec3Normalize(&forward, &forward);
	// �E�����x�N�g�����v�Z
	D3DXVECTOR3 right;
	D3DXVec3Cross(&right, &forward, &up);


	float aspect = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
	float nearDepth = 1.0f;
	if (i != 0)nearDepth = m_CascadeArea[i - 1];
	float farDepth = m_CascadeArea[i];

	// �G���A�̋ߕ��ʂ̒��S����̋������v�Z
	float nearY = tanf(m_FovY * 0.5f) * nearDepth;
	float nearX = nearY * aspect;

	// �G���A�̉����ʂ̒��S����̋������v�Z
	float farY = tanf(m_FovY * 0.5f) * farDepth;
	float farX = farY * aspect;


	// �G���A�̋ߕ��ʂ̒��S���W�����߂�
	D3DXVECTOR3 nearPos(m_Position + forward * nearDepth);
	// �G���A�̉����ʂ̒��S���W�����߂�
	D3DXVECTOR3 farPos(m_Position + forward * farDepth);

	// 8���_
	D3DXVECTOR3 vertex[8];

	// �ߕ���
	vertex[0] = nearPos + up * nearY + right * nearX;  	// �E��
	vertex[1] = nearPos + up * nearY + right * -nearX;  // ����
	vertex[2] = nearPos + up * -nearY + right * nearX;  // �E��
	vertex[3] = nearPos + up * -nearY + right * -nearX; // ����

	// ������
	vertex[4] = farPos + up * farY + right * farX;      // �E��
	vertex[5] = farPos + up * farY + right * -farX;     // ����
	vertex[6] = farPos + up * -farY + right * farX;     // �E��
	vertex[7] = farPos + up * -farY + right * -farX;    // ����


	// �J�X�P�[�h�̒��S���W���v�Z
	D3DXVECTOR3 vec(0.0f, 0.0f, 0.0f);
	for (D3DXVECTOR3& v : vertex)
	{
		vec += v;
	}
	D3DXVECTOR3 cascadeCenter(vec / 8.0f);
	GUI::pos[i] = cascadeCenter;

	// ������̎l���_�擾
	if (i == 2)
	{
		GUI::frustumPos[0] = vertex[4];
		GUI::frustumPos[1] = vertex[5];
		GUI::frustumPos[2] = vertex[6];
		GUI::frustumPos[3] = vertex[7];
	}


	// ���C�g�쐬---------------------------------------------------------------------

    // ���C�g�J�����\���̂̏�����
	LIGHT light;
	light.LightType = DIRECTIONAL_LIGHT;
	light.Direction = D3DXVECTOR4(1.0f, -3.0f, 2.0f, 0.0f); // �i���W - ���_�j�̃x�N�g��
	D3DXVec4Normalize(&light.Direction, &light.Direction);
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ���C�g�J�����̃r���[�s����쐬
	D3DXVECTOR3 lightDir(light.Direction.x, light.Direction.y, light.Direction.z);
	D3DXVECTOR3 mainPos(cascadeCenter.x, m_Position.y, cascadeCenter.z);
	float length = D3DXVec3Length(&cascadeCenter);
	D3DXVECTOR3 lightPos = mainPos - lightDir * length;
	D3DXVECTOR3 lightTarget = mainPos;
	D3DXVECTOR3 lightUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&light.ViewMatrix, &lightPos, &lightTarget, &lightUp);

	// ���C�g�J�����̃v���W�F�N�V�����s����쐬
	D3DXMatrixPerspectiveFovLH(&light.ProjectionMatrix, m_FovY, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 3000.0f);
	Renderer::SetViewMatrix(&light.ViewMatrix);
	Renderer::SetProjectionMatrix(&light.ProjectionMatrix);
	Renderer::SetLight(light);


	// �J�X�P�[�h���`��t���O�i�f�o�b�O�p�j
	float cascade = 0.0f;
	if (GUI::cascadeFlag) cascade = 1.0f;
	light.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, cascade);



	// �ő�l�ŏ��l���v�Z
	D3DXVECTOR3 vMax(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	D3DXVECTOR3 vMin(FLT_MAX, FLT_MAX, FLT_MAX);
	for (D3DXVECTOR3& v : vertex)
	{
		D3DXVECTOR3 vec;
		// ���W�ϊ�
		D3DXMATRIX lightViewProjection = light.ViewMatrix * light.ProjectionMatrix;
		D3DXVec3TransformCoord(&vec, &v, &light.ViewMatrix);
		// �ő�l�X�V
		D3DXVec3Maximize(&vMax, &vMax, &vec);
		// �ŏ��l�X�V
		D3DXVec3Minimize(&vMin, &vMin, &vec);
	}

	// �͈͂��g��
	float margin = 5.0f;
	vMin -= D3DXVECTOR3(margin, margin, margin);
	vMax += D3DXVECTOR3(margin, margin, margin);

	// ���s�ˉe�s��ɕϊ�
	D3DXMatrixOrthoOffCenterLH(&light.ProjectionMatrix, vMin.x, vMax.x, vMin.y, vMax.y, vMin.z, vMax.z);

	// �v���W�F�N�V�����s��𒼍s�ˉe�s��ɕϊ��������̂��Z�b�g
	Renderer::SetProjectionMatrix(&light.ProjectionMatrix);
	Renderer::SetLight(light);


	// �萔�o�b�t�@�Ɋi�[
	m_LVPC[i] = light.ViewMatrix * light.ProjectionMatrix;
	if (i == 2) Renderer::SetLightViewProjectionCropMatrix(m_LVPC);
}


void Camera::DrawDebugCascadeShadow()
{
	//�r���[�}�g���N�X�ݒ�
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 cameraPos(0.0f, 400.0f, -1.0f);
	D3DXMatrixLookAtLH(&m_ViewMatrix, &cameraPos, &up, &up);
	Renderer::SetViewMatrix(&m_ViewMatrix);

	//�v���W�F�N�V�����}�g���N�X�ݒ�
	D3DXMatrixPerspectiveFovLH(&m_ProjectionMatrix, m_FovY, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 3000.0f);
	Renderer::SetProjectionMatrix(&m_ProjectionMatrix);
}


void Camera::DrawDebugAnimation()
{
	Scene* scene = Manager::GetScene();
	D3DXVECTOR3 target = D3DXVECTOR3(0.0f, 2.0f, 0.0f);
	D3DXVECTOR3 pos = target + D3DXVECTOR3(-10.0f, 10.0f, -10.0f);

	//�r���[�}�g���N�X�ݒ�
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&m_ViewMatrix, &pos, &target, &up);
	Renderer::SetViewMatrix(&m_ViewMatrix);

	//�v���W�F�N�V�����}�g���N�X�ݒ�
	D3DXMatrixPerspectiveFovLH(&m_ProjectionMatrix, m_FovY, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 3000.0f);
	Renderer::SetProjectionMatrix(&m_ProjectionMatrix);
}

void Camera::ActTitleSceneCamera()
{
	// �J��������
	switch (m_Phase)
	{
	case 0:
		m_Target = D3DXVECTOR3(0.0f, 3.0f, 0.0f);
		m_Position += D3DXVECTOR3(0.04f, 0.004f, 0.01f);

		break;
	case 1:
		m_Target = D3DXVECTOR3(10.0f, 0.0f, 0.0f);
		m_Position = D3DXVECTOR3(5.0f, 4.0f, -15.0f) + D3DXVECTOR3(15.0f, 10.0f, -15.0f) * (m_PhaseFrame / 210.0f);

		break;
	case 2:
		m_Target = D3DXVECTOR3(26.0f, 6.0f, 25.0f) + D3DXVECTOR3(3.0f, 1.0f, 2.0f) * (m_PhaseFrame / 210.0f);
		m_Position = m_Target + D3DXVECTOR3(2.0f, 1.5f, -5.0f);

		break;
	}


	// �t���[���X�V
	m_PhaseFrame++;

	// �t�F�[�Y�X�V
	if (m_PhaseFrame >= 210)
	{
		m_PhaseFrame = 0;
		m_Position = D3DXVECTOR3(0.0f, 8.0f, -12.0f);
		
		m_Phase++;
		m_Phase %= 3;
	}
}
