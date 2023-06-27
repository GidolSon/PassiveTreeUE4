// Fill out your copyright notice in the Description page of Project Settings.


#include "PassiveTreePlayerController.h"

void APassiveTreePlayerController::BeginPlay()
{
    Super::BeginPlay();
    // UI 입력 처리
    SetInputMode(FInputModeGameAndUI());
}