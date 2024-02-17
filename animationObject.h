#pragma once
#include "gameObject.h"

#define ANIM_END_FRAME 20

// �A�j���[�V�����f�o�b�O�E�B���h�E�p�I�u�W�F�N�g
class AnimationObject : public GameObject
{
private:
	D3DXVECTOR3 m_Velocity{};       // �ړ��x�N�g��
	D3DXVECTOR3 m_AttackedVector{}; // �U�����󂯂������x�N�g��
	D3DXVECTOR3 m_DodgeVector{};    // �������̃x�N�g��;

	class EnemyField*     m_Target{}; // ���b�N�I�����̃^�[�Q�b�g�G�l�~�[
	D3DXVECTOR3     m_TargetVector{}; // �^�[�Q�b�g�����̃x�N�g��
	float           m_TargetRot{};    // �v���C���[�ƃ^�[�Q�b�g�̊p�x

	bool            m_isAnimation{};  // �A�j���[�V�����\���ʎq
	bool            m_EndAnimation{}; // �A�j���[�V�����I�����ʎq
	int             m_EndFrame{};     // �A�j���[�V�����I���t���[��
	PlayerAnim      m_PlayerAnim{};   // �v���C���[�̃A�^�b�N���

public:

	void Init() override;
	void Uninit() override;
	void Update() override;
	void DrawDebug() override;

	void ResetAnimation();
};



