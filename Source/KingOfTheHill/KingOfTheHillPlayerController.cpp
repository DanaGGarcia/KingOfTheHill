// Copyright Epic Games, Inc. All Rights Reserved.


#include "KingOfTheHillPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "KingOfTheHillCameraManager.h"
#include "Blueprint/UserWidget.h"
#include "KingOfTheHill.h"
#include "KingOfTheHillCharacter.h"
#include "Widget/HUDPlayer.h"
#include "Widget/Switcher.h"
#include "Widgets/Input/SVirtualJoystick.h"

AKingOfTheHillPlayerController::AKingOfTheHillPlayerController()
{
	// set the player camera manager class
	PlayerCameraManagerClass = AKingOfTheHillCameraManager::StaticClass();
}

void AKingOfTheHillPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	//==========Widget=============
	if (!IsLocalController())return;

	if (SwitcherWidgetClass)
	{
		SwitcherWidget = CreateWidget<USwitcher>(this, SwitcherWidgetClass);
		if (SwitcherWidget)
		{
			SwitcherWidget->AddToViewport(0);
		}
	}
	
	// only spawn touch controls on local player controllers
	if (ShouldUseTouchControls() && IsLocalPlayerController())
	{
		// spawn the mobile controls widget
		MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);

		if (MobileControlsWidget)
		{
			// add the controls to the player screen
			MobileControlsWidget->AddToPlayerScreen(0);

		} else {

			UE_LOG(LogKingOfTheHill, Error, TEXT("Could not spawn mobile controls widget."));

		}

	}
}

void AKingOfTheHillPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Context
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}

			// only add these IMCs if we're not using mobile touch input
			if (!ShouldUseTouchControls())
			{
				for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
				{
					Subsystem->AddMappingContext(CurrentContext, 0);
				}
			}
		}

		if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
		{
			EIC->BindAction(AttackAction, ETriggerEvent::Started, this, &AKingOfTheHillPlayerController::Attack);
		}
	}
	
}

bool AKingOfTheHillPlayerController::ShouldUseTouchControls() const
{
	// are we on a mobile platform? Should we force touch?
	return SVirtualJoystick::ShouldDisplayTouchInterface() || bForceTouchControls;
}



void AKingOfTheHillPlayerController::Attack()
{
	AKingOfTheHillCharacter* MYCharacter = Cast<AKingOfTheHillCharacter>(GetPawn());
	if (MYCharacter)
	{
		MYCharacter->Server_Push();
	}
}

void AKingOfTheHillPlayerController::Client_IniciarPartidaVisual_Implementation()
{
	if (SwitcherWidget) 
	{
		SwitcherWidget->ChangeWidget();
	}
}