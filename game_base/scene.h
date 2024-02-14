#pragma once
#include "gameObject.h"
#include <list>
#include <vector>
#include <typeinfo>
#define SCENE_LAYER 6

// 各シーン名
enum SCENE_NAME
{
	TITLE_SCENE,
	TUTORIAL_SCENE,
	GAME_SCENE,
	WORFSPACE_SCENE,

	NON_SCENE
};

// 各シーンの基本となるクラス
class Scene
{
protected:
	/*
	レイヤー
	0 : システム
	1 : オブジェクト
	2 : エフェクト
	3 : UI
	4 : デバッグ：カスケードシャドウ
	5 : デバッグ：アニメーション
	*/
	std::list<GameObject*>  m_GameObject[SCENE_LAYER];

	class PostProcess* m_PostProcess;
	class Polygon2D*   m_Polygon2D;
	SCENE_NAME         m_SceneName;

public:
	virtual void Init() {}

	virtual void Uninit()
	{
		for (int i = 0; i < SCENE_LAYER; i++)
		{
			for (GameObject* gameObject : m_GameObject[i])
			{
				gameObject->Uninit();
				delete gameObject;
			}
		}
	}

	virtual void Update()
	{
		// ゲームオブジェクト
		for (int i = 0; i < SCENE_LAYER; i++)
		{
			for (GameObject* gameObject : m_GameObject[i])
			{
				gameObject->Update();
			}

			// オブジェクト削除
			m_GameObject[i].remove_if([](GameObject* object) { return object->Destroy(); }); // ラムダ式
		}
	}

	virtual void Draw()
	{
		for (int i = 0; i < SCENE_LAYER; i++)
		{
			for (GameObject* gameObject : m_GameObject[i])
			{
				gameObject->Draw();
			}
		}
	}

	//ゲームオブジェクト追加
	template <typename T>
	T* AddGameObject(int layer)
	{
		T* gameObject = new T();
		m_GameObject[layer].push_back(gameObject);

		gameObject->Init();

		return gameObject;
	}

	// ゲームオブジェクト取得（クラス検索）
	template <typename T>
	T* GetGameObject()
	{
		for (int i = 0; i < 4; i++)
		{
			for (GameObject* object : m_GameObject[i])
			{
				if (typeid(*object) == typeid(T))
				{
					return (T*)object;
				}
			}
		}
		return nullptr;
	}

	// デバッグゲームオブジェクト取得（クラス検索）
	template <typename T>
	T* GetDebugGameObject()
	{
		for (int i = 5; i < SCENE_LAYER; i++)
		{
			for (GameObject* object : m_GameObject[i])
			{
				if (typeid(*object) == typeid(T))
				{
					return (T*)object;
				}
			}
		}
		return nullptr;
	}

	// ゲームオブジェクトの配列を取得（クラス検索）
	template <typename T>
	std::vector<T*> GetGameObjects()
	{
		std::vector<T*> objects;
		for (int i = 0; i < 4; i++)
		{
			for (GameObject* object : m_GameObject[i])
			{
				if (typeid(*object) == typeid(T))
				{
					objects.push_back((T*)object);
				}
			}
		}
		return objects;
	}

	// ゲームオブジェクトの配列を取得（タグ検索）
	std::vector<GameObject*> GetGameObjectWithTag(Tag tag)
	{
		std::vector<GameObject*> objects;
		for (int i = 0; i < 4; i++)
		{
			for (GameObject* object : m_GameObject[i])
			{
				if (object->GetTag() == tag)
				{
					objects.push_back(object);
				}
			}
		}
		return objects;
	}

	// シーン名取得
	SCENE_NAME GetSceneName() { return m_SceneName; }
};