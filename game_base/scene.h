#pragma once
#include "gameObject.h"
#include <list>
#include <vector>
#include <typeinfo>
#define SCENE_LAYER 6

// �e�V�[����
enum SCENE_NAME
{
	TITLE_SCENE,
	TUTORIAL_SCENE,
	GAME_SCENE,
	WORFSPACE_SCENE,

	NON_SCENE
};

// �e�V�[���̊�{�ƂȂ�N���X
class Scene
{
protected:
	/*
	���C���[
	0 : �V�X�e��
	1 : �I�u�W�F�N�g
	2 : �G�t�F�N�g
	3 : UI
	4 : �f�o�b�O�F�J�X�P�[�h�V���h�E
	5 : �f�o�b�O�F�A�j���[�V����
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
		// �Q�[���I�u�W�F�N�g
		for (int i = 0; i < SCENE_LAYER; i++)
		{
			for (GameObject* gameObject : m_GameObject[i])
			{
				gameObject->Update();
			}

			// �I�u�W�F�N�g�폜
			m_GameObject[i].remove_if([](GameObject* object) { return object->Destroy(); }); // �����_��
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

	//�Q�[���I�u�W�F�N�g�ǉ�
	template <typename T>
	T* AddGameObject(int layer)
	{
		T* gameObject = new T();
		m_GameObject[layer].push_back(gameObject);

		gameObject->Init();

		return gameObject;
	}

	// �Q�[���I�u�W�F�N�g�擾�i�N���X�����j
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

	// �f�o�b�O�Q�[���I�u�W�F�N�g�擾�i�N���X�����j
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

	// �Q�[���I�u�W�F�N�g�̔z����擾�i�N���X�����j
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

	// �Q�[���I�u�W�F�N�g�̔z����擾�i�^�O�����j
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

	// �V�[�����擾
	SCENE_NAME GetSceneName() { return m_SceneName; }
};