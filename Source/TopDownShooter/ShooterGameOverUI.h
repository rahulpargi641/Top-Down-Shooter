// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShooterGameOverUI.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNSHOOTER_API UShooterGameOverUI : public UUserWidget
{
	GENERATED_BODY()

public:
	// C++ declared custom event
	UFUNCTION(BlueprintImplementableEvent)  // creates custom event that we can call from Blueprint
		void FadeOut();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Class Setup")
		int EnemyHitCount;
	
};
