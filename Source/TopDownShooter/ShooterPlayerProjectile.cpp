// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerProjectile.h"
#include "Engine.h"  // Lets us prints messages to screen 
#include "TimerManager.h"
#include "ShooterPlayer.h"
#include "ShooterEnemy.h"


// Sets default values
AShooterPlayerProjectile::AShooterPlayerProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PRojectileMesh"));
	RootComponent = ProjectileMesh;
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//ProjectileMesh->CastShadow = false;

	// Set up Sphere Collision Component
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("PRojectileComponent"));
	SphereComponent->SetupAttachment(GetRootComponent());
	SphereComponent->SetHiddenInGame(false);
	SphereComponent->ShapeColor = FColor(0, 255, 0);
	SphereComponent->SetSphereRadius(558.f);

	bSpawnedFromPlayer = false;
	ProjectileSpeed = 55.f;
	DestroyParticlesAfter = 5.f;
	ParticleReachDistance = 1000.f;

}

// Called when the game starts or when spawned
void AShooterPlayerProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShooterPlayerProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bSpawnedFromPlayer)
	{
		 AddActorLocalOffset(FVector(ProjectileSpeed, 0, 0));	
	}
}

void AShooterPlayerProjectile::CheckIfParticleSpawned(bool spawned, FVector spawnedParticleLocation)
{
	bSpawnedFromPlayer = spawned;
	SpawnedParticleLocation = spawnedParticleLocation;


	UWorld* World = GetWorld();
	if (World)
	{
		World->GetTimerManager().SetTimer(TimerHandle_Destroy, this, &AShooterPlayerProjectile::TimerDestroyEvent, DestroyParticlesAfter);
	}
}

void AShooterPlayerProjectile::TimerDestroyEvent()
{
	Destroy();
}







