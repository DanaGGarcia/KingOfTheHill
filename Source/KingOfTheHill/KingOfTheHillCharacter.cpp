// Copyright Epic Games, Inc. All Rights Reserved.

#include "KingOfTheHillCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KingOfTheHill.h"
#include "Components/BoxComponent.h"
#include "GameState/KOTHGameState.h"
#include "PlayerState/KOTHPlayerState.h"


AKingOfTheHillCharacter::AKingOfTheHillCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
	// Create the first person mesh that will be viewed only by this character's owner
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Mesh"));

	FirstPersonMesh->SetupAttachment(GetMesh());
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));

	// Create the Camera Component	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCameraComponent->SetupAttachment(FirstPersonMesh, FName("head"));
	FirstPersonCameraComponent->SetRelativeLocationAndRotation(FVector(-2.8f, 5.89f, 0.0f), FRotator(0.0f, 90.0f, -90.0f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
	FirstPersonCameraComponent->bEnableFirstPersonScale = true;
	FirstPersonCameraComponent->FirstPersonFieldOfView = 70.0f;
	FirstPersonCameraComponent->FirstPersonScale = 0.6f;

	// configure the character comps
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	GetCapsuleComponent()->SetCapsuleSize(34.0f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.5f;

	//=============Game============
	SwordMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwordMesh"));
	SwordMesh->SetupAttachment(GetMesh(), FName("SwordSocket"));

	MeleeDetector = CreateDefaultSubobject<UBoxComponent>(TEXT("MeleeDetector"));
	MeleeDetector->SetupAttachment(SwordMesh);
}

void AKingOfTheHillCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AKingOfTheHillCharacter::DoJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AKingOfTheHillCharacter::DoJumpEnd);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AKingOfTheHillCharacter::MoveInput);

		// Looking/Aiming
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AKingOfTheHillCharacter::LookInput);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AKingOfTheHillCharacter::LookInput);
	}
	else
	{
		UE_LOG(LogKingOfTheHill, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void AKingOfTheHillCharacter::MoveInput(const FInputActionValue& Value)
{
	// get the Vector2D move axis
	FVector2D MovementVector = Value.Get<FVector2D>();

	// pass the axis values to the move input
	DoMove(MovementVector.X, MovementVector.Y);

}

void AKingOfTheHillCharacter::LookInput(const FInputActionValue& Value)
{
	// get the Vector2D look axis
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// pass the axis values to the aim input
	DoAim(LookAxisVector.X, LookAxisVector.Y);

}

void AKingOfTheHillCharacter::DoAim(float Yaw, float Pitch)
{
	if (GetController())
	{
		// pass the rotation inputs
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AKingOfTheHillCharacter::DoMove(float Right, float Forward)
{
	if (GetController())
	{
		// pass the move inputs
		AddMovementInput(GetActorRightVector(), Right);
		AddMovementInput(GetActorForwardVector(), Forward);
	}
}

void AKingOfTheHillCharacter::DoJumpStart()
{
	// pass Jump to the character
	Jump();
}

void AKingOfTheHillCharacter::DoJumpEnd()
{
	// pass StopJumping to the character
	StopJumping();
}

//=============== Game ===============
 void AKingOfTheHillCharacter::StartAddPoints()
 {
 	GetWorldTimerManager().SetTimer(
 		PointTimerHandle,
 		this,
 		&AKingOfTheHillCharacter::AddPoint,
 		1.0f,
 		true
 	);
 }

 void AKingOfTheHillCharacter::CancelAddPoints()
 {
 	GetWorldTimerManager().ClearTimer(PointTimerHandle);
 }

 void AKingOfTheHillCharacter::AddPoint()
 {
	AKOTHGameState* GS = GetWorld()->GetGameState<AKOTHGameState>();
	if (GS && GetPlayerState())
	{
		GS->AwardPointToPlayer(GetPlayerState());
	}
 }

void AKingOfTheHillCharacter::DisableCharacterMovement()
{
	GetCharacterMovement()->DisableMovement();
}

void AKingOfTheHillCharacter::Push()
{
	Multicast_EmpujeAnimation();
	
	GetWorldTimerManager().SetTimer(
		TimerAttack,
		this,
		&AKingOfTheHillCharacter::Server_Empuje,
		0.35f,
		false);
	
}

void AKingOfTheHillCharacter::Server_Empuje_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Empujo");
	TArray<AActor*> HitActors;
	MeleeDetector->GetOverlappingActors(HitActors);

	for (AActor* Actor : HitActors)
	{
		if (Actor && Actor != this)
		{
			AKingOfTheHillCharacter* OtherCharacter =Cast<AKingOfTheHillCharacter>(Actor);

			if (OtherCharacter)
			{
				FVector Direccion =(OtherCharacter->GetActorLocation() - GetActorLocation()).GetSafeNormal();

				OtherCharacter->LaunchCharacter(Direccion * PushForce,true,true);
			}
		}
	}
}

void AKingOfTheHillCharacter::Multicast_EmpujeAnimation_Implementation()
{
	if (AttackMontageMelee)
	{
		PlayAnimMontage(AttackMontageMelee);
	}
}