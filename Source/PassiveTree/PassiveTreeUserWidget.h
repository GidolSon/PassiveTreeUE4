// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "PassiveTreeNodeButton.h"
#include "PassiveTreeEdgeImage.h"
#include "PassiveTreeUserWidget.generated.h"


/**
 * 패시브트리 나타내는 UserWidget
 */
UCLASS()
class PASSIVETREE_API UPassiveTreeUserWidget : public UUserWidget, public IPassiveTreeNodePressedInterface
{
	GENERATED_BODY()

public:
	void InitTreeComponent();

	/* 노드 클릭 이벤트 수신 */
	UFUNCTION()
		virtual void OnNodePressed(uint32 NodeID) override;

	UFUNCTION()
	void OnResetPressed();

protected:
	virtual void NativeConstruct() override;

	/* 총 투자 코스트 텍스트 갱신 */
	void RefreshTotalCostText(uint32 totalCost);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
		UCanvasPanel* RootCanvasPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
		UButton* ButtonReset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
		UTextBlock* TextBlockTotalCost;

	TMap<uint32, UPassiveTreeNodeButton*> NodeUIMap;
	TMap<FIntPoint, UPassiveTreeEdgeImage*> EdgeUIMap;
};
