// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PassiveTreePlayerController.generated.h"

/**
* 패시브트리 프로젝트 PlayerController
 */
UCLASS()
class PASSIVETREE_API APassiveTreePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
};
