// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "ShooterPlayerProjectile.generated.h"

UCLASS()
class TOPDOWNSHOOTER_API AShooterPlayerProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShooterPlayerProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// The Mesh for Projectile
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Class Setup")
		UStaticMeshComponent* ProjectileMesh;
	
	// Sphere Collision Component
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Class Settings")
		USphereComponent* SphereComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Class Settings")
		float DestroyParticlesAfter;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Class Settings")
		float ProjectileSpeed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Class Settings")
		float ParticleReachDistance;

	// Handle for timer
	FTimerHandle TimerHandle_Destroy;

	FVector SpawnedParticleLocation;

	bool bSpawnedFromPlayer;

	//void SetParams(float projectileSpeed, float projectileSize, UStaticMesh* meshRef, FVector projectileColor);

	void CheckIfParticleSpawned(bool Spawned, FVector spawnedParticleLocation);

	void LimitProjectileReachDistance(FVector NewParticlePos);

	void TimerDestroyEvent();
	
};
