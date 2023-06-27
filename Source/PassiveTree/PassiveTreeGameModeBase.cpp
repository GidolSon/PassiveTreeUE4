// Copyright Epic Games, Inc. All Rights Reserved.


#include "PassiveTreeGameModeBase.h"
#include "FPassiveTreeBuilder.h"
#include "PassiveTreeUserWidget.h"

const FPassiveTree& APassiveTreeGameModeBase::GetPassiveTree() const
{
    return passiveTree;
}

FPassiveTreeState& APassiveTreeGameModeBase::GetPassiveTreeState()
{
    return passiveTreeState;
}

void APassiveTreeGameModeBase::BeginPlay()
{
    Super::BeginPlay();
    // 메인 위젯 갱신 (GameMode 설정에서 PassiveTreeUserWidget 선택)
    ChangeMenuWidget(StartingWidgetClass);

    // 무작위 패시브트리 생성
    FPassiveTreeBuilder::MakeRandomPassiveTree(passiveTree);
    passiveTreeState.SetTree(&passiveTree);

    UPassiveTreeUserWidget* passiveTreeWidget = Cast<UPassiveTreeUserWidget>(CurrentWidget);
    if (passiveTreeWidget)
    {
        // 패시브트리 위젯 초기화
        passiveTreeWidget->InitTreeComponent();
    }

}

void APassiveTreeGameModeBase::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
    if (CurrentWidget != nullptr)
    {
        CurrentWidget->RemoveFromViewport();
        CurrentWidget = nullptr;
    }
    if (NewWidgetClass != nullptr)
    {
        CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
        if (CurrentWidget != nullptr)
        {
            CurrentWidget->AddToViewport();
        }
    }
}