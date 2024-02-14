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

	// 新規にTextEventが始まる時に現在実行中のTextEventを終了させる
	static void SwitchTextEvent();
};