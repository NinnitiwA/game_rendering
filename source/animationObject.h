#pragma once
#include "gameObject.h"

#define ANIM_END_FRAME 20

// アニメーションデバッグウィンドウ用オブジェクト
class AnimationObject : public GameObject
{
private:
	D3DXVECTOR3 m_Velocity{};       // 移動ベクトル
	D3DXVECTOR3 m_AttackedVector{}; // 攻撃を受けた方向ベクトル
	D3DXVECTOR3 m_DodgeVector{};    // 回避方向のベクトル;

	class EnemyField*     m_Target{}; // ロックオン中のターゲットエネミー
	D3DXVECTOR3     m_TargetVector{}; // ターゲット方向のベクトル
	float           m_TargetRot{};    // プレイヤーとターゲットの角度

	bool            m_isAnimation{};  // アニメーション可能識別子
	bool            m_EndAnimation{}; // アニメーション終了識別子
	int             m_EndFrame{};     // アニメーション終了フレーム
	PlayerAnim      m_PlayerAnim{};   // プレイヤーのアタック種類

public:

	void Init() override;
	void Uninit() override;
	void Update() override;
	void DrawDebug() override;

	void ResetAnimation();
};



