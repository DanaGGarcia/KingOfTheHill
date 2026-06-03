// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/GameMode/KOTHGameMode.h"

#include "KingOfTheHillCharacter.h"
#include "KingOfTheHillPlayerController.h"
#include "GameState/KOTHGameState.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerState/KOTHPlayerState.h"

class AKingOfTheHillPlayerController;

AKOTHGameMode::AKOTHGameMode()
{
	bDelayedStart = true;

	MatchTime = 20;
}

void AKOTHGameMode::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(
		StartMatchTimer,
		this,
		&AKOTHGameMode::StartMatch,
		MatchStart,
		false
	);
}

void AKOTHGameMode::StartMatch()
{
	Super::StartMatch();
		
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("KOTHGameMode::StartMatch()"));

	AKOTHGameState* GS = GetGameState<AKOTHGameState>();
	if (GS)
	{
		GS->RemainingTime = MatchTime;
		GS->StartGameClock(); 
	}
}
