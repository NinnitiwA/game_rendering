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
	bool DrawDebugWindow; // ウィンドウ描画識別子

	bool Walk;
	bool Jump;
	bool PressAttack;
	bool DashAttack;
	bool ZigzagAttack;
	bool MagicChant;
	bool MagicAttack;
};


// ImGuiマネージャークラス
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
	static bool drawGuiFlag;  // GUI描画制御フラグ
	static bool sceneEndFlag; // シ―ン終了フラグ

	// プレイヤー・カメラ・エネミー
	static bool playerControllFlag;    // プレイヤー操作フラグ
	static D3DXVECTOR4 playerMaterial; // プレイヤーマテリアル
	static bool cameraControllFlag;    // カメラ操作
	static D3DXVECTOR3 cameraDebugOffset; // デバッグ用カメラオフセット
	static D3DXVECTOR3 targetDebugOffset; // デバッグ用ターゲットオフセット
	static float cameraRotation;          // デバッグ用カメラ回転量
	static bool drawInvisibleBox;      // 透明オブジェクト描画フラグ
	static bool addEnemyObjectFlag;    // エネミー生成フラグ

	static PlayerAnimation playerAnimation; // プレイヤー行動アニメーションフラグ
	
	// ライト・texture
	static bool drawLocalLightFlag; // 光源オブジェクト描画フラグ
	static int lightType;           // 光源タイプ
	static int pointLightNum;       // ポイントライトの数
	static int textureType;         // 描画テクスチャタイプ

	// shadow
	static bool cascadeFlag;
	static D3DXVECTOR3 pos[3]; // カスケード中心座標
	static D3DXVECTOR3 target;
	static D3DXVECTOR3 frustumPos[4];
	static D3DXVECTOR3 test;

	// ポストプロセス
	static D3DXCOLOR PBRObjectColor; // オブジェクトカラー
	static bool pbrObjectDrawFlag;// PBRオブジェクト描画フラグ
	static bool postProcessFlag;  // ポストプロセス識別子
	static bool depthOfField;     // 被写界深度識別子
	static bool depthOfFieldDebugFlag; // 被写界深度デバッグ識別子
	static D3DXVECTOR4 depthParameter;
	static bool fog;
	static FOG fogParameter;
	static bool bloom;           // ブルーム識別子
	static float luminanceValue; // 輝度抽出基準値
	static bool antiAliasing;    // アンチエイリアシング識別子
	static float antiAliasingBias; // アンチエイリアシング用基準値
	static float UVOffset;        // ブラー時のUVオフセット
	static bool drawEdgeFlag;     // アンチエイリアシングのエッジ描画識別子

	// プロファイリング
	static bool profilingFlag;
	static int profileNum;
	static std::vector<PROFILE*> profile;
	static PROFILE* allTimeProfile;

	// デバッグ用
	static D3DXVECTOR4 debugPosition;
};