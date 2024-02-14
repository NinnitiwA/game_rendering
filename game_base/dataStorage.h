#include <vector>

struct PlayerDataStorage
{
	std::vector<D3DXVECTOR3> WalkAnimPos;
	std::vector<D3DXVECTOR3> WalkAnimScale;

	std::vector<D3DXVECTOR3> JumpAnimPos;
	std::vector<D3DXVECTOR3> JumpAnimScale;

	std::vector<D3DXVECTOR3> PressAttackAnimPos;
	std::vector<D3DXVECTOR3> PressAttackAnimScale;

	std::vector<D3DXVECTOR3> DashAttackAnimPos;
	std::vector<D3DXVECTOR3> DashAttackAnimScale;

	std::vector<D3DXVECTOR3> MagicAnimPos;
	std::vector<D3DXVECTOR3> MagicAnimScale;
};

struct EnemyFieldDataStorage
{
	std::vector<D3DXVECTOR3> WalkAnimPos;
	std::vector<D3DXVECTOR3> WalkAnimScale;

	std::vector<D3DXVECTOR3> AttackAnimPos;
	std::vector<D3DXVECTOR3> AttackAnimScale;

	std::vector<D3DXVECTOR3> MagicAttackAnimPos;
	std::vector<D3DXVECTOR3> MagicAttackAnimScale;
};

struct FieldDataStorage
{
	std::vector<D3DXVECTOR3> RockPos;
	std::vector<D3DXVECTOR3> TreePos;
	std::vector<D3DXVECTOR3> GroundGrassPos;
};

class DataStorage
{
private:
	static PlayerDataStorage m_PlayerDataStorage;
	static EnemyFieldDataStorage m_EnemyFieldDataStorage;
	static FieldDataStorage m_FieldDataStorage;

public:
	static PlayerDataStorage* GetPlayerDataStorage() { return &m_PlayerDataStorage; }
	static EnemyFieldDataStorage* GetEnemyFieldDataStorage() { return &m_EnemyFieldDataStorage; }
	static FieldDataStorage* GetFieldDataStorage() { return &m_FieldDataStorage; }
};
