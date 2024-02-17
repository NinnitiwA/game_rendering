#pragma once
#include "gameObject.h"


enum CameraOscillationMode // �U���̃��x��
{
	CO_LARGE_MODE,  // ��
	CO_MEDIUM_MODE, // ��
	CO_SMALL_MODE,  // ��

	CO_MODE_NUM,
};


class Camera : public GameObject
{
private:
	D3DXVECTOR3	m_Target{};         // �^�[�Q�b�g�i�v���C���[�j���W
	D3DXVECTOR3 m_OldTarget{};      // �O�t���[���̃J�����^�[�Q�b�g
	D3DXVECTOR3 m_FixedDistance{};  // �J�����ʒu�̌Œ苗��
	D3DXVECTOR3 m_EffectOffset{};   // �J�����G�t�F�N�g�̃I�t�Z�b�g�i�U���j
	D3DXVECTOR3 m_EventOffset{};    // �J�����C�x���g�̃I�t�Z�b�g
	float       m_FovY;             // ����p
	D3DXCOLOR   m_Time;

	// �J�����U���p
	CameraOscillationMode m_CameraOscillationMode{}; // �J�����̐U�����x��
	int m_CameraOscillationFrame = 10;               // �J�����̐U���t���[��

	// �^�C�g���V�[���p
	int m_Phase;      // �J�����̃t�F�[�Y
	int m_PhaseFrame; // �J�����t���[��

	// �V���h�E�}�b�v�p
	float m_CascadeArea[SHADOWMAP_NUM] = { 30.0f, 60.0f, 100.0f };
	LIGHT* m_Light;
	D3DXMATRIX  m_LVP[SHADOWMAP_NUM];
	static D3DXMATRIX m_ViewMatrix;
	static D3DXMATRIX m_ProjectionMatrix;
	static D3DXMATRIX m_ReflectionMatrix;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void DrawReflection();
	void DrawCascadeShadowMap(int num);
	void DrawDebugCascadeShadow();
	void DrawDebugAnimation();

	void SetLight(LIGHT* Light) { m_Light = Light; }

	// �J�����̐U�����x���ݒ�
	void SetCameraOscillationMode(CameraOscillationMode cameraOscillationMode)
	{
		m_CameraOscillationMode = cameraOscillationMode;
		m_CameraOscillationFrame = 0;
	}
	// �C�x���g�I�t�Z�b�g�̎擾
	D3DXVECTOR3 GetEventOffset() { return m_EventOffset; }
	// �C�x���g�I�t�Z�b�g�̐ݒ�
	void SetEventOffset(D3DXVECTOR3 eventOffset) { m_EventOffset = eventOffset; }
	// �Œ苗���̐ݒ�
	void SetFixedDistance(D3DXVECTOR3 distance) { m_FixedDistance = distance; }
	// �^�C�g���V�[���ł̃J��������
	void ActTitleSceneCamera();
};