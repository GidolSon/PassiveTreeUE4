// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "PassiveTreeEdgeImage.generated.h"

/**
 * 패시브트리 엣지 나타내는 Image
 */
UCLASS()
class PASSIVETREE_API UPassiveTreeEdgeImage : public UImage
{
	GENERATED_BODY()

public:
	/* 초기화 - 링크ID(연결된 두 노드ID), 두 노드의 위치 */
	void InitSetting(FIntPoint _LinkID, FVector2D Pos1, FVector2D Pos2);
	void SetEdgeColor(const FLinearColor& color);

	static const FLinearColor Color_Deselect;
	static const FLinearColor Color_Select;

protected:
	FIntPoint LinkID;
};
