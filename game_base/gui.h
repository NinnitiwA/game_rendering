#pragma once
#include "stl.h"

struct FOG;

enum PROFILING_DIVISION
{
	UPDATE_DIVISION, 
	SHADOWMAP_DIVISION,
	REFLECTION_DIVISION,
	ZPREPASS_DIVISION,
	GBUFFER_DIVISION,
	LIGHTING_DIVISION,
	POSTPROCESS_DIVISION,
};

struct PROFILE
{
	LARGE_INTEGER Freq;
	LARGE_INTEGER Start;
	LARGE_INTEGER End;
	double Time;

	const char* ProcessName;
	int Num;
};

struct PlayerAnimation
{
	bool DrawDebugWindow; // �E�B���h�E�`�掯�ʎq

	bool Walk;
	bool Jump;
	bool PressAttack;
	bool DashAttack;
	bool ZigzagAttack;
	bool MagicChant;
	bool MagicAttack;
};


// ImGui�}�l�[�W���[�N���X
class GUI
{
private:

public:
	static void ImGuiInit(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	static void ImGuiRender();

	static void ImGuiRenderRenderingMenu();
	static void ImGuiRenderProfiler();
	static void ImGuiRenderSRV();
	static void ImGuiRenderCascadeShadow();
	static void ImGuiRenderPlayerAnimation();
	static void ImGuiRenderCameraControll();

	static void BeginProfiling();
	static void EndProfiling(int num);
	static void BeginAllTimeProfiling();
	static void EndAllTimeProfiling();
	const static char* SetProcessName(PROFILING_DIVISION pd);


	// [ImGui]
	static bool drawGuiFlag;  // GUI�`�搧��t���O
	static bool sceneEndFlag; // �V�\���I���t���O

	// �v���C���[�E�J�����E�G�l�~�[
	static bool playerControllFlag;    // �v���C���[����t���O
	static D3DXVECTOR4 playerMaterial; // �v���C���[�}�e���A��
	static bool cameraControllFlag;    // �J��������
	static D3DXVECTOR3 cameraDebugOffset; // �f�o�b�O�p�J�����I�t�Z�b�g
	static D3DXVECTOR3 targetDebugOffset; // �f�o�b�O�p�^�[�Q�b�g�I�t�Z�b�g
	static float cameraRotation;          // �f�o�b�O�p�J������]��
	static bool drawInvisibleBox;      // �����I�u�W�F�N�g�`��t���O
	static bool addEnemyObjectFlag;    // �G�l�~�[�����t���O

	static PlayerAnimation playerAnimation; // �v���C���[�s���A�j���[�V�����t���O
	
	// ���C�g�Etexture
	static bool drawLocalLightFlag; // �����I�u�W�F�N�g�`��t���O
	static int lightType;           // �����^�C�v
	static int pointLightNum;       // �|�C���g���C�g�̐�
	static int textureType;         // �`��e�N�X�`���^�C�v

	// shadow
	static bool cascadeFlag;
	static D3DXVECTOR3 pos[3]; // �J�X�P�[�h���S���W
	static D3DXVECTOR3 target;
	static D3DXVECTOR3 frustumPos[4];
	static D3DXVECTOR3 test;

	// �|�X�g�v���Z�X
	static D3DXCOLOR PBRObjectColor; // �I�u�W�F�N�g�J���[
	static bool pbrObjectDrawFlag;// PBR�I�u�W�F�N�g�`��t���O
	static bool postProcessFlag;  // �|�X�g�v���Z�X���ʎq
	static bool depthOfField;     // ��ʊE�[�x���ʎq
	static bool depthOfFieldDebugFlag; // ��ʊE�[�x�f�o�b�O���ʎq
	static D3DXVECTOR4 depthParameter;
	static bool fog;
	static FOG fogParameter;
	static bool bloom;           // �u���[�����ʎq
	static float luminanceValue; // �P�x���o��l
	static bool antiAliasing;    // �A���`�G�C���A�V���O���ʎq
	static float antiAliasingBias; // �A���`�G�C���A�V���O�p��l
	static float UVOffset;        // �u���[����UV�I�t�Z�b�g
	static bool drawEdgeFlag;     // �A���`�G�C���A�V���O�̃G�b�W�`�掯�ʎq

	// �v���t�@�C�����O
	static bool profilingFlag;
	static int profileNum;
	static std::vector<PROFILE*> profile;
	static PROFILE* allTimeProfile;

	// �f�o�b�O�p
	static D3DXVECTOR4 debugPosition;
};