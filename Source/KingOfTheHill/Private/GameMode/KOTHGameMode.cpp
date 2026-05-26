// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/GameMode/KOTHGameMode.h"

#include "KingOfTheHillCharacter.h"
#include "GameState/KOTHGameState.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerState/KOTHPlayerState.h"

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

	GEngine->AddOnScreenDebugMessage(-1, 5.f,FColor::Red, TEXT("KOTHGameMode::StartMatch()"));

	AKOTHGameState* GS = GetGameState<AKOTHGameState>();

	if (GS)
	{
		GS->RemainingTime = MatchTime;
	}

	GetWorldTimerManager().SetTimer(
		MatchTimer,
		this,
		&AKOTHGameMode::UpdateTimer,
		1.0f,
		true
	);
}

void AKOTHGameMode::UpdateTimer()
{
	AKOTHGameState* GS = GetGameState<AKOTHGameState>();

	if (!GS) return;

	if (GS->RemainingTime <= 0)
	{
		EndMatch();
		return;
	}
	
	GS->RemainingTime--;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("%i"),GS->RemainingTime));
	
	
}

void AKOTHGameMode::EndMatch()
{
	GetWorldTimerManager().ClearTimer(MatchTimer);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "EndMatch");

	//Freno los puntos
	TArray<AActor*> Characters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),AKingOfTheHillCharacter::StaticClass(),Characters);
	for (AActor* Actor : Characters)
	{
		AKingOfTheHillCharacter* Character = Cast<AKingOfTheHillCharacter>(Actor);

		if (Character)
		{
			Character->CancelAddPoints();
		}
	}

	//Declaro ganador
	AKOTHPlayerState* Winner = nullptr;
	bool bEmpate = false;
	int32 BestScore = 0;
	int32 PlayerNumber = 0;
	int32 WinnerNumber = -1;

	for (APlayerState* PS : GameState->PlayerArray)
	{
		AKOTHPlayerState* KOTHPS = Cast<AKOTHPlayerState>(PS);

		if (!KOTHPS) continue;
		
		if (KOTHPS->ScorePoints > BestScore)
		{
			BestScore = KOTHPS->ScorePoints;
			Winner = KOTHPS;
			WinnerNumber = PlayerNumber;
			bEmpate = false;
		}
		else if (KOTHPS->ScorePoints == BestScore && Winner)
		{
			bEmpate = true;
		}

		PlayerNumber++;
	}

	if (bEmpate)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, TEXT("Empate!"));
	}
	else
	{
		FString WinnerText;

		if (WinnerNumber == 0)
		{
			WinnerText = "Gano el Servidor";
		}
		else
		{
			WinnerText = FString::Printf(TEXT("Gano el Cliente %i"),WinnerNumber);
		}

		GEngine->AddOnScreenDebugMessage(-1,10.f,FColor::Green,WinnerText);
	}
}