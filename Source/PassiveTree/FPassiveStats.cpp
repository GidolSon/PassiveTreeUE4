// Fill out your copyright notice in the Description page of Project Settings.


#include "FPassiveStats.h"


#define MACRO_MAP_ENUM(name) ePassiveStatType::##name
#define MACRO_MAP_STR_ENUM(name) {#name, ePassiveStatType::##name}
#define MACRO_MAP_ENUM_STR(name) {ePassiveStatType::##name, #name}

const TArray<ePassiveStatType> FPassiveStatTypeHelper::s_mapPassiveStatTypeArray =
{
	MACRO_MAP_ENUM(STR),
	MACRO_MAP_ENUM(DEX),
	MACRO_MAP_ENUM(INT),
	MACRO_MAP_ENUM(CON),
	MACRO_MAP_ENUM(SKILL_DMGUP_FIRE),
};

const TMap<FString, ePassiveStatType> FPassiveStatTypeHelper::s_mapPassiveStatStringToType =
{
	MACRO_MAP_STR_ENUM(STR),
	MACRO_MAP_STR_ENUM(DEX),
	MACRO_MAP_STR_ENUM(INT),
	MACRO_MAP_STR_ENUM(CON),
	MACRO_MAP_STR_ENUM(SKILL_DMGUP_FIRE),
};
const TMap<ePassiveStatType, FString> FPassiveStatTypeHelper::s_mapPassiveStatTypeToString =
{
	MACRO_MAP_ENUM_STR(STR),
	MACRO_MAP_ENUM_STR(DEX),
	MACRO_MAP_ENUM_STR(INT),
	MACRO_MAP_ENUM_STR(CON),
	MACRO_MAP_ENUM_STR(SKILL_DMGUP_FIRE),
};

const FString FPassiveStatTypeHelper::s_PassiveStatTypeNullString = "";


ePassiveStatType FPassiveStatTypeHelper::getRandomStat()
{
	int32 randIndex = FGenericPlatformMath::Rand() % s_mapPassiveStatTypeArray.Num();
	return s_mapPassiveStatTypeArray[randIndex];
}

ePassiveStatType FPassiveStatTypeHelper::convertFromString(const FString& str)
{
	const ePassiveStatType* pStatType = FPassiveStatTypeHelper::s_mapPassiveStatStringToType.Find(str);
	if (pStatType == nullptr)
	{
		return ePassiveStatType::UNDEFINED;
	}
	return *pStatType;
}

const FString& FPassiveStatTypeHelper::convertToString(ePassiveStatType type)
{
	const FString* pStr = FPassiveStatTypeHelper::s_mapPassiveStatTypeToString.Find(type);
	if (pStr == nullptr)
	{
		return FPassiveStatTypeHelper::s_PassiveStatTypeNullString;
	}
	return *pStr;
}
