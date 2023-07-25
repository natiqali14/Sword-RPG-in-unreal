#pragma once

UENUM(BlueprintType)
enum class ETurnInPlace: uint8
{
	ETIP_NotTurning,
	ETIP_TurnInLeft,
	ETIP_TurnInRight,
};
UENUM(BlueprintType)
enum class EPlayerCombatState : uint8
{
	EPCS_NonCombat,
	EPCS_SingleSword,
	EPCS_DualSword,
	EPCS_HandCombat
};
UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	EPS_UnOccupy,
	EPS_Attacking,
	EPS_Equipping,
};
UENUM(BlueprintType)
enum class EDeathPose : uint8
{
	EDP_Alive,
	EDP_DeathA,
	EDP_DeathB,
};
UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	EES_Patrolling,
	EES_Chasing,
	EES_Looking,
	EES_Attacking,
	
};
