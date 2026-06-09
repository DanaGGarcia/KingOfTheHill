// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/GameMode/KOTHGameMode.h"

#include "KingOfTheHillCharacter.h"
#include "KingOfTheHillPlayerController.h"
#include "GameState/KOTHGameState.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerState/KOTHPlayerState.h"
#include "KingOfTheHillPlayerController.h"


class AKingOfTheHillPlayerController;

AKOTHGameMode::AKOTHGameMode()
{
	bDelayedStart = true;

	MatchTime = 20;
}

void AKOTHGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AKOTHGameMode::StartMatch()
{
	Super::StartMatch();

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		AKingOfTheHillPlayerController* KOTHPC = Cast<AKingOfTheHillPlayerController>(It->Get());

		if (KOTHPC)
		{
			KOTHPC->Client_IniciarPartidaVisual();
		}
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("KOTHGameMode::StartMatch()"));

	AKOTHGameState* GS = GetGameState<AKOTHGameState>();
	if (GS)
	{
		GS->RemainingTime = MatchTime;
		GS->StartGameClock(); 
	}
}
