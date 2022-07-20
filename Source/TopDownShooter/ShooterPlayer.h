// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"
#include "ShooterPlayer.generated.h"

UCLASS()
class TOPDOWNSHOOTER_API AShooterPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AShooterPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnParticle(UWorld* World);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/**/
	// Refrence to Our GameStartUI
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UserInterface")
		class UShooterGameStartUI* GameStartMenu;

	// Refrence to Our GameOvertUI
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UserInterface")
		class UShooterGameOverUI* GameOverMenu;

	// Refrence to our W_GameHUD
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UserInterface")
		class UMyUserWidget* InGameHUD;

	// The Display Mesh Component
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Class Setup")
		UStaticMeshComponent* PlayerMesh;

	// Create the Spring Arm Component
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Class Setup")
		class USpringArmComponent* SpringArm;
	
	// Create the Camera Component
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Class Setup")
		class UCameraComponent* Camera;

	// Sphere Collision Component
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Class Setup")
		USphereComponent* SphereComponent;

	// A temporary Arrow Component to help set up correct trajectory
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Class Setup")
		UArrowComponent* HelperArrow;
		
	// The Max Speed
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Class Settings")
		float MaxSpeed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Class Settings")
		float InterpSpeed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Class Settings")
		float FireDelay;

		// A Refrence to the Projectile
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Class Settings")
		TSubclassOf <class AShooterPlayerProjectile> ProjectileRef;    

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Class Settings")
		float PlayerHealth;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Class Settings")
		int ShotsFired;

		// Refrence to the PlayerController
		APlayerController* PlayerController;

		// Handle for Efficient Management of FireTimerExpired Fucntion
		FTimerHandle TimerHandle_FireTimer;

		// Position where the Raycast Hit when traced from the cursor 
		FVector MouseHitPos;

		float CurrentSpeedX;
		float CurrentSpeedY;

		// Player Ready Variables
		bool bPlayerReady;
		bool bPCRefrenced;

		// Fire Variables
		bool bFirePressed;
		bool bFire;

		int EnemyDestroyedCount;
		
	UFUNCTION(BlueprintImplementableEvent)
		void CE_CreateUI();

	UFUNCTION(BlueprintImplementableEvent)
		void CE_Death();

	UFUNCTION(BlueprintImplementableEvent)
		void CE_SetHUDValues();

	UFUNCTION(BlueprintCallable)
	void ReducePlayerHealth();

		// Remove GameStartMenu widget from the screen 
		void ClearStartMenu();

		// Handle World Offset for the PlayerMesh
		void HandleWorldOffset(FVector velocity);

		// Fire Input Events
		void FireEventPressed();
		void FireEventReleased();
		void FireTimerExpiresThen();

		void SetGameOverUIValues();
		void SetInGameHUDValues();


};
