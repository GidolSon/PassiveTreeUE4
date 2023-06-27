// Fill out your copyright notice in the Description page of Project Settings.


#include "PassiveTreeNodeButton.h"
#include "Components/CanvasPanelSlot.h"

const FLinearColor UPassiveTreeNodeButton::Color_Root = FLinearColor::Black;
const FLinearColor UPassiveTreeNodeButton::Color_Deselect = FLinearColor::Gray;
const FLinearColor UPassiveTreeNodeButton::Color_Select = FLinearColor::Green;


void UPassiveTreeNodeButton::InitSetting(uint32 _ID, FVector2D Pos, UTextBlock* NewTextBlock, FString _text, IPassiveTreeNodePressedInterface* _Owner)
{
	ForceVolatile(true);

	ID = _ID;
	OwnerPtr = _Owner;

	DescTextBlockPtr = NewTextBlock;

	// 노드 위치에 따라 위치 설정
	UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(Slot);
	if (PanelSlot)
	{
		PanelSlot->SetAlignment(FVector2D(0.5f, 0.5f));
		PanelSlot->SetSize(FVector2D(50, 50));
		PanelSlot->SetPosition(Pos);
	}

	AddChild(NewTextBlock);

	// 텍스트 설정
	FText text = FText::FromString(_text);
	NewTextBlock->SetText(text);

	// 버튼 클릭 이벤트 설정
	OnClicked.AddUniqueDynamic(this, &UPassiveTreeNodeButton::OnNodePressed);
}

void UPassiveTreeNodeButton::OnNodePressed()
{
	// 클릭 이벤트 전달
	if (OwnerPtr)
	{
		OwnerPtr->OnNodePressed(ID);
	}
}

void UPassiveTreeNodeButton::SetNodeColor(const FLinearColor& color)
{
	SetBackgroundColor(color);
}
