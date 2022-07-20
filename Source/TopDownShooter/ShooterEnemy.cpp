// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterEnemy.h"
#include "ShooterPlayer.h"

// Sets default values
AShooterEnemy::AShooterEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EnemyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemyMesh"));
	EnemyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Setup the Collision Sphere
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(EnemyMesh);
	SphereComponent->SetHiddenInGame(false);
	SphereComponent->ShapeColor = FColor(0, 255, 0);
	SphereComponent->SetSphereRadius(158.f);

	EnemyHitPoints = 3;

	EnemyInterpSpeed = 0.09;

	bEnemiesCreated = false;

	//SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AShooterEnemy::OnBeginOverlap);

}

// Called when the game starts or when spawned
void AShooterEnemy::BeginPlay()
{
	Super::BeginPlay();

	EnemyInterpSpeed = FMath::FRandRange(0.1, 0.25);
	EnemyRotationRate = FMath::FRandRange(-1.f, 1.f);

	// Adding target points for enemy to move
	for (int i = 0; i < 101; i++)
	{
		TargetPoints.Add(FVector(FMath::FRandRange(-5000.f, 5000.f), FMath::FRandRange(-5000.f, 5000.f), 112.f));
	}

	APlayerController* PlayerController;
	PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerRef = Cast<AShooterPlayer>(PlayerController->GetPawn());
	
}

// Called every frame
void AShooterEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerRef)
	{
		if (PlayerRef->bPlayerReady)
		{
				MoveToPoint(DeltaTime);
		}
	}

}

void AShooterEnemy::MoveToPoint(float deltaTime)
{
	// If We reach a point, set the Index for the next one
	if (bPointReached)
	{
		if (!PointIndex)
		{
			PointIndex = FMath::RandRange(0, 100);
			bPointSet = true;
		}
	}

	// Check if we are at a given point
	if (FVector(EnemyMesh->GetComponentLocation() - TargetPoints[PointIndex]).Size() < 50.f)
	{
		bPointReached = true;
	}
	else
	{
		bPointReached = false;
		bPointSet = false;
	}

	// Move and Rotate
	EnemyMesh->SetWorldLocation(FMath::VInterpTo(EnemyMesh->GetComponentLocation(), TargetPoints[PointIndex], deltaTime, EnemyInterpSpeed));
	EnemyMesh->SetWorldRotation(FRotator(0.f, EnemyRotationRate, 0.f));


}

void AShooterEnemy::CreateEnemies()
{
	UWorld* World = GetWorld();
	if (EnemyRef!=nullptr)
	{
		World->SpawnActor<AShooterEnemy>(EnemyRef, PlayerRef->PlayerMesh->GetComponentLocation() + FVector(1000.f, 1000.f, 0.f), FRotator(0.f, 0.f, 0.f));
		UE_LOG(LogTemp, Warning, TEXT("Enemy-I'm Spawnned"));
		bEnemiesCreated = true;
	}
	
}


void AShooterEnemy::ReduceEnemyHitPoints()
{
	EnemyHitPoints -= 1;

	if (EnemyHitPoints <= 0)
	{
		if (PlayerRef)
		{
			if (PlayerRef->bPlayerReady)
			{
				UE_LOG(LogTemp, Warning, TEXT("I Should be Dead"));
				EnemyDestroyed();
				PlayerRef->EnemyDestroyedCount = EnemyDestroyedCount;

				Destroy();

			}
		}
	}
}

int AShooterEnemy::EnemyDestroyedCount;

 void AShooterEnemy::EnemyDestroyed()
{
	EnemyDestroyedCount += 1;
	UE_LOG(LogTemp, Warning, TEXT(" Enemy Destroyed: %d"), EnemyDestroyedCount);

}





/*void AShooterEnemy::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player Collided"));

	if ((OtherActor != NULL) && (OtherActor->GetClass() != this->GetClass()) && (OtherComp != NULL))
	{

		if (Cast<AShooterPlayer>(OtherActor))
		{
			if (GEngine)
			{
				if (Cast<AShooterPlayer>(OtherActor)->bPlayerReady)
				{
					//Cast<AShooterPlayer>(OtherActor)->ReducePlayerSize();
				}
			}

		}
	}
}  */

