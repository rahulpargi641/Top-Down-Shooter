// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNSHOOTER_API UMyUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "In Game HUD")
	int EnemyDestroyedCount_HUD;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "In Game HUD")
	int ShotsFired_HUD;

	
};
