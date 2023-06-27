// Fill out your copyright notice in the Description page of Project Settings.


#include "PassiveTreeEdgeImage.h"
#include "Components/CanvasPanelSlot.h"

const FLinearColor UPassiveTreeEdgeImage::Color_Deselect = FLinearColor::Gray;
const FLinearColor UPassiveTreeEdgeImage::Color_Select = FLinearColor::Green;


void UPassiveTreeEdgeImage::InitSetting(FIntPoint _LinkID, FVector2D Pos1, FVector2D Pos2)
{
	ForceVolatile(true);

	LinkID = _LinkID;

	FVector2D vecDiff = (Pos2 - Pos1);

	// 두 노드 위치에 따라 Edge의 위치 설정
	UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(Slot);
	if (PanelSlot)
	{
		PanelSlot->SetAlignment(FVector2D(0.5f, 0.5f));
		float pointLength = vecDiff.Size();
		PanelSlot->SetSize(FVector2D(pointLength, 10.f));
		PanelSlot->SetPosition((Pos1 + Pos2) * 0.5f);
	}

	// 두 노드 위치에 따라 각도 설정
	SetRenderTransformAngle(FMath::RadiansToDegrees(FGenericPlatformMath::Atan2(vecDiff.Y, vecDiff.X)));
}

void UPassiveTreeEdgeImage::SetEdgeColor(const FLinearColor& color)
{
	SetColorAndOpacity(color);
}
