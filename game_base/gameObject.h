#pragma once
#include "main.h"
#include <list>

// ゲームオブジェクト識別用タグ
enum Tag
{
	ENEMY,
	ENVIRONMENT_OBJECT,

	NON,
};


// すべてのゲームオブジェクトの基本となるクラス
class GameObject
{
protected:
	bool        m_Destroy = false;
	Tag         m_Tag = NON;

	D3DXVECTOR3 m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

public:
	void SetDestroy() { m_Destroy = true; }
	bool GetDestroy() { return m_Destroy; }
	bool Destroy()
	{
		if (m_Destroy)
		{
			Uninit();
			delete this;
			return true;
		}
		else
		{
			return false;
		}
	}
	void SetPosition(D3DXVECTOR3 pos) { m_Position = pos; }
	D3DXVECTOR3 GetPosition() { return m_Position; }
	void SetRotation(D3DXVECTOR3 rot) { m_Rotation = rot; }
	D3DXVECTOR3 GetRotation() { return m_Rotation; }
	void SetScale(D3DXVECTOR3 scl) { m_Scale = scl; }
	D3DXVECTOR3 GetScale() { return m_Scale; }
	D3DXVECTOR3 GetForward()
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 forward;
		forward.x = rot._31;
		forward.y = rot._32;
		forward.z = rot._33;

		return forward;
	}
	D3DXVECTOR3 GetRight()
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 forward;
		forward.x = rot._11;
		forward.y = rot._12;
		forward.z = rot._13;

		return forward;
	}
	D3DXVECTOR3 GetTop()
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 forward;
		forward.x = rot._21;
		forward.y = rot._22;
		forward.z = rot._23;

		return forward;
	}
	void SetTag(Tag tag) { m_Tag = tag; }
	Tag GetTag() { return m_Tag; }

	virtual void Init() {}

	virtual void Uninit(){}

	virtual void Update(){}

	virtual void Draw(){}
	virtual void DrawDebug(){}
	virtual void DrawShadowMapping(){}
	virtual void DrawZPrePass(){}
	virtual void DrawReflection() {}
};