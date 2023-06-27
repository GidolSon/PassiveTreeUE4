// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 패시브 스탯 타입 정의
 */
enum class ePassiveStatType {
	UNDEFINED = 0,
	STR = 1,
	DEX = 2,
	INT = 3,
	CON = 4,
	SKILL_DMGUP_FIRE = 1001,
};

/**
 * 패시브 스탯 타입 관련 도움 함수 모음
 */
class FPassiveStatTypeHelper
{
public:
	static ePassiveStatType getRandomStat();
	static ePassiveStatType convertFromString(const FString& str);
	static const FString& convertToString(ePassiveStatType type);

private:
	static const TArray<ePassiveStatType> s_mapPassiveStatTypeArray;
	static const TMap<FString, ePassiveStatType> s_mapPassiveStatStringToType;
	static const TMap<ePassiveStatType, FString> s_mapPassiveStatTypeToString;
	static const FString s_PassiveStatTypeNullString;
};
