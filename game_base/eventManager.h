#pragma once
#include "gameObject.h"

class EventManager
{
public:
	static void InitTutorialStage();
	static void InitTheLandOfBeginnings(D3DXVECTOR3 areaPosition);
	static void InitTheCrystalFountainhead(D3DXVECTOR3 areaPosition);
	static void InitTheSkyIsland(D3DXVECTOR3 areaPosition);
	static void InitResult();

	// �V�K��TextEvent���n�܂鎞�Ɍ��ݎ��s����TextEvent���I��������
	static void SwitchTextEvent();
};