// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "ShooterEnemy.generated.h"

UCLASS()
class TOPDOWNSHOOTER_API AShooterEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShooterEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Class Settings")
		UStaticMeshComponent* EnemyMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Class Settings")
		USphereComponent* SphereComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Class Settings")
		float EnemyInterpSpeed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Class Settings")
		int EnemyHitPoints;
	// Refrence to the shooter enemy blueprint
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Class Settings")
		TSubclassOf<class AShooterEnemy> EnemyRef;

	// A Refrence to the Player in Tick
	class AShooterPlayer* PlayerRef;
	// List of Vector Points
	TArray <FVector> TargetPoints;

	static int EnemyDestroyedCount;

	// Enemy Rotation rate
	float EnemyRotationRate;

	// Which point are we goint to 
	int PointIndex;

	// Have we reached a point
	bool bPointReached;

	// Do we have a point to go to ?
	bool bPointSet;

	bool bEnemiesCreated;

	UFUNCTION(BlueprintPure)
		FORCEINLINE int GetStaticEnemyDestroyedCount() { return EnemyDestroyedCount; }

	UFUNCTION(BlueprintCallable)
		void ReduceEnemyHitPoints();

	static void EnemyDestroyed();

	void CreateEnemies();
	// Function to handle the movement to any given point
	void MoveToPoint(float deltaTime);

	// Overlap Events
	//UFUNCTION()
	//void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);


	
};
