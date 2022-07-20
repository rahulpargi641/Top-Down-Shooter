// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayer.h"
#include "ShooterGameStartUI.h"
#include "ShooterGameOverUI.h"
#include "MyUserWidget.h"
#include "Engine.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "ShooterPlayerProjectile.h"
#include "Components/SceneComponent.h"


// Sets default values
AShooterPlayer::AShooterPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PLayerMesh"));

	// Setup SpringArm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPringArm"));
	SpringArm->SetupAttachment(PlayerMesh);
	SpringArm->TargetArmLength = 1200.f;
	SpringArm->SetRelativeRotation(FRotator(-80.f, 0.f, 0.f));
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 30.f;

	// Setup Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAmera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	// Setup the HelperArrow
	HelperArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("HElperArrow"));
	HelperArrow->bHiddenInGame = false;  // At run time it will not show
	HelperArrow->SetupAttachment(PlayerMesh);

	// Setup Collision
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SPhereComponent"));
	SphereComponent->AttachTo(PlayerMesh);
	SphereComponent->SetHiddenInGame(false);  // will not be hidden
	SphereComponent->ShapeColor = FColor(0.f, 255.f, 0.f);
	SphereComponent->SetSphereRadius(158.f);


	// Player Stats
	bPlayerReady = false;
	MaxSpeed = 50;
	InterpSpeed = 5.f;

	PlayerHealth = 5;
	
	FireDelay = 0.5f;

	EnemyDestroyedCount = 0;


}

// Called when the game starts or when spawned
void AShooterPlayer::BeginPlay()
{
	Super::BeginPlay();

	CE_CreateUI();

	if (GameStartMenu != nullptr) 
	{
		GameStartMenu->AddToViewport(9999);
	}

}

// Called every frame
void AShooterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bPlayerReady)
	{
		if (!bPCRefrenced) 
		{
			PlayerController = GetWorld()->GetFirstPlayerController();
			bPCRefrenced = true;

			PlayerController->bShowMouseCursor = true;

		}
		//Get the Game World
		UWorld* World = GetWorld();

		if (World)
		{
			//SetScaleColor(DeltaTime);
			SetInGameHUDValues();
			SetGameOverUIValues();

			if (PlayerHealth <= 0)
			{
				CE_Death();
			}
		}


		if (bPCRefrenced)
		{
			HandleWorldOffset(FVector(CurrentSpeedX, CurrentSpeedY, 0.f));

			// Moving Left and Right
			if (InputComponent->GetAxisValue("MoveRight") != 0)
			{
				if (InputComponent->GetAxisValue("MoveRight") > 0)
				{
					CurrentSpeedY = FMath::FInterpTo(CurrentSpeedY, MaxSpeed, DeltaTime, InterpSpeed);

				}
				else
				{
					CurrentSpeedY = FMath::FInterpTo(CurrentSpeedY, -MaxSpeed, DeltaTime, InterpSpeed);
				}
			}
			else
			{
				CurrentSpeedY = FMath::FInterpTo(CurrentSpeedY, 0.f, DeltaTime, InterpSpeed);

			}

			// Moving Up and Down
			if (InputComponent->GetAxisValue("MoveForward") != 0)
			{
				if (InputComponent->GetAxisValue("MoveForward") > 0)
				{
					CurrentSpeedX = FMath::FInterpTo(CurrentSpeedX, MaxSpeed, DeltaTime, InterpSpeed);

				}
				else
				{
					CurrentSpeedX = FMath::FInterpTo(CurrentSpeedX, -MaxSpeed, DeltaTime, InterpSpeed);
				}
			}
			else
			{
				CurrentSpeedX = FMath::FInterpTo(CurrentSpeedX, 0.f, DeltaTime, InterpSpeed);

			}



			if (World)
			{
				ECollisionChannel Trace = ECC_Visibility;
				bool Complex = false;
				FHitResult Hit;
				PlayerController->GetHitResultUnderCursor(Trace, Complex, Hit);

				MouseHitPos = Hit.ImpactPoint;

				HelperArrow->SetWorldRotation(FRotator(0, (UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), MouseHitPos).Yaw), 0));


				// We're gonna trace a line from the mouse cursor into the game world and try to read our hit Position Vector 
				// Check for Fire Input
				if (bFirePressed)
				{
					if (bFire)
					{
						//World->SpawnActor<AShooterPlayerProjectile>(HelperArrow->GetComponentLocation() + HelperArrow->GetForwardVector() * 100.f, HelperArrow->GetComponentRotation())->SetParams(ProjectileSpeed, GetActorScale3D().X/3 , PlayerMesh->GetStaticMesh(), CurrentPlayerColor);

						AShooterPlayerProjectile* ParticleSpawned = World->SpawnActor<AShooterPlayerProjectile>(ProjectileRef, HelperArrow->GetComponentLocation() + HelperArrow->GetForwardVector() * 100.f, HelperArrow->GetComponentRotation());
						if (ParticleSpawned)
						{
							ShotsFired++;
							FVector ParticleSpawnedLocation = ParticleSpawned->SphereComponent->GetComponentLocation();
							ParticleSpawned->CheckIfParticleSpawned(true, ParticleSpawnedLocation);
						}
						
						bFire = false;
						World->GetTimerManager().SetTimer(TimerHandle_FireTimer, this, &AShooterPlayer::FireTimerExpiresThen, FireDelay);
					}
						
				}
			}
			
		}


	}

}



// Called to bind functionality to input
void AShooterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAction("GameEntry", IE_Pressed, this, &AShooterPlayer::ClearStartMenu);
	InputComponent->BindAction("Fire", IE_Pressed, this, &AShooterPlayer::FireEventPressed);
	InputComponent->BindAction("Fire", IE_Released, this, &AShooterPlayer::FireEventReleased);


	InputComponent->BindAxis("MoveForward");
	InputComponent->BindAxis("MoveRight");

}

void AShooterPlayer::ReducePlayerHealth()
{
	PlayerHealth -= 1;
}

void AShooterPlayer::ClearStartMenu()
{
	if (GameStartMenu != nullptr)
	{
		GameStartMenu->RemoveFromParent();
		bPlayerReady = true;
	}

	if (InGameHUD != nullptr)
	{
		InGameHUD->AddToViewport(9999);
	}
}

void AShooterPlayer::HandleWorldOffset(FVector velocity)
{
	PlayerMesh->AddWorldOffset(velocity);
}

void AShooterPlayer::FireEventPressed()
{
	bFirePressed = true;
	bFire = true;
}

void AShooterPlayer::FireEventReleased()
{
	bFirePressed = false;
}

void AShooterPlayer::FireTimerExpiresThen()
{
	bFire = true;
}

void AShooterPlayer::SetGameOverUIValues()
{
	if (GameOverMenu)
	{
		GameOverMenu->EnemyHitCount = EnemyDestroyedCount;
	}
}

void AShooterPlayer::SetInGameHUDValues()
{
	if (InGameHUD)
	{
		InGameHUD->ShotsFired_HUD = ShotsFired;
		InGameHUD->EnemyDestroyedCount_HUD = EnemyDestroyedCount;
		//UE_LOG(LogTemp, Warning, TEXT(" Enemy Destroyed in ShooterPlayer : %d"), EnemyDestroyedCount);

	}
}


