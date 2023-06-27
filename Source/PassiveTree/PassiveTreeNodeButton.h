// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "PassiveTreeNodePressedInterface.h"
#include "PassiveTreeNodeButton.generated.h"

/**
 * 패시브트리 노드 나타내는 Button
 */
UCLASS()
class PASSIVETREE_API UPassiveTreeNodeButton : public UButton
{
	GENERATED_BODY()

public:
	/* 초기화 - 노드ID, 위치, TextBlock 객체, 표시할 텍스트, OnNodePressed 이벤트함수 전달받을 Owner */
	void InitSetting(uint32 _ID, FVector2D Pos, UTextBlock* NewTextBlock, FString _text, IPassiveTreeNodePressedInterface* _Owner);

	UFUNCTION()
	void OnNodePressed();

	void SetNodeColor(const FLinearColor& color);

	uint32 ID;

	static const FLinearColor Color_Root;
	static const FLinearColor Color_Deselect;
	static const FLinearColor Color_Select;

protected:
	UTextBlock* DescTextBlockPtr;
	IPassiveTreeNodePressedInterface* OwnerPtr;
};
