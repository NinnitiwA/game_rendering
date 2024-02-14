#pragma once
#include "gameObject.h"


enum CameraOscillationMode // 振動のレベル
{
	CO_LARGE_MODE,  // 強
	CO_MEDIUM_MODE, // 並
	CO_SMALL_MODE,  // 弱

	CO_MODE_NUM,
};


class Camera : public GameObject
{
private:
	D3DXVECTOR3	m_Target{};         // ターゲット（プレイヤー）座標
	D3DXVECTOR3 m_OldTarget{};      // 前フレームのカメラターゲット
	D3DXVECTOR3 m_FixedDistance{};  // カメラ位置の固定距離
	D3DXVECTOR3 m_EffectOffset{};   // カメラエフェクトのオフセット（振動）
	float       m_FovY;             // 視野角
	D3DXCOLOR   m_Time;

	// カメラ振動用
	CameraOscillationMode m_CameraOscillationMode{}; // カメラの振動レベル
	int m_CameraOscillationFrame = 10;               // カメラの振動フレーム

	// シャドウマップ用
	float m_CascadeArea[SHADOWMAP_NUM] = { 30.0f, 60.0f, 100.0f };
	LIGHT* m_Light;
	D3DXMATRIX  m_LVPC[SHADOWMAP_NUM];
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

	// カメラの振動レベル設定
	void SetCameraOscillationMode(CameraOscillationMode cameraOscillationMode)
	{
		m_CameraOscillationMode = cameraOscillationMode;
		m_CameraOscillationFrame = 0;
	}
};