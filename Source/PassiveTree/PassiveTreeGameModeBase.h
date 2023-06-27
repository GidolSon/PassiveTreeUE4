// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "FPassiveTree.h"
#include "FPassiveTreeState.h"
#include "PassiveTreeGameModeBase.generated.h"

/**
 * 패시브트리 프로젝트 GameModeBase
 */
UCLASS()
class PASSIVETREE_API APassiveTreeGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
    UFUNCTION(BlueprintCallable, Category = "UMG Game")
        void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);

    const FPassiveTree& GetPassiveTree() const;
    FPassiveTreeState& GetPassiveTreeState();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
        TSubclassOf<UUserWidget> StartingWidgetClass;

    UPROPERTY()
        UUserWidget* CurrentWidget;

    FPassiveTree passiveTree;
    FPassiveTreeState passiveTreeState;
};
