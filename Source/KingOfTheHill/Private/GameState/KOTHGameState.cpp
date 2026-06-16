// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/GameState/KOTHGameState.h"

#include "KingOfTheHillCharacter.h"
#include "Actors/PointZone.h"
#include "GameMode/KOTHGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "PlayerState/KOTHPlayerState.h"


void AKOTHGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AKOTHGameState, RemainingTime);

	DOREPLIFETIME(AKOTHGameState, WinnerText);
}

void AKOTHGameState::StartGameClock()
{
	if (HasAuthority()) 
	{
		GetWorldTimerManager().SetTimer(GameClockTimerHandle, this, &AKOTHGameState::AdvanceClock, 1.0f, true);
	}
}

void AKOTHGameState::AdvanceClock()
{
	if (RemainingTime > 0)
	{
		RemainingTime--;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Tiempo: %i"), RemainingTime));
	}
	else
	{
		GetWorldTimerManager().ClearTimer(GameClockTimerHandle);
		
		TArray<AActor*> Characters;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AKingOfTheHillCharacter::StaticClass(), Characters);
		
		for (AActor* Actor : Characters)
		{
			if (Actor && Actor->Implements<UPlayerInterface>())
			{
				IPlayerInterface::Execute_CancelAddPoints(Actor);
			}
		}
		
		EndMatch();
	}
}

void AKOTHGameState::AwardPointToPlayer(APlayerState* TargetPlayer)
{
	if (!HasAuthority()) return;

	AKOTHPlayerState* PS = Cast<AKOTHPlayerState>(TargetPlayer);
	if (PS)
	{
		PS->AddPoint();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Puntos: %i"), PS->ScorePoints));
	}
}

void AKOTHGameState::EndMatch()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "EndMatch");

	TArray<AActor*> AllCharacters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AKingOfTheHillCharacter::StaticClass(), AllCharacters);

	//desactivo movimiento
	for (AActor* Actor : AllCharacters)
	{
		if (Actor && Actor->Implements<UPlayerInterface>())
		{
			IPlayerInterface::Execute_CancelMovement(Actor);
		}
	}
	
	//Declaro ganador
	AKOTHPlayerState* Winner = nullptr;
	bool bEmpate = false;
	int32 BestScore = 0;

	for (APlayerState* PS : PlayerArray)
	{
		AKOTHPlayerState* KOTHPS = Cast<AKOTHPlayerState>(PS);

		if (!KOTHPS) continue;
       
		if (KOTHPS->ScorePoints > BestScore)
		{
			BestScore = KOTHPS->ScorePoints;
			Winner = KOTHPS;
			bEmpate = false;
		}
		else if (KOTHPS->ScorePoints == BestScore && BestScore > 0)
		{
			bEmpate = true;
		}
	}
	
	if (bEmpate || Winner == nullptr)
	{
		WinnerText = "Empate!";
	}
	else
	{
		WinnerText = FString::Printf(TEXT("¡El ganador es: %s!"),*Winner->GetPlayerName());
	}

	AKOTHGameMode* GM = Cast<AKOTHGameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		GetWorldTimerManager().SetTimer(
	   RestartTimer,                 
	   GM,                           
	   &AKOTHGameMode::EndMatch,     
	   3.f,                         
	   false
	   );
	}
}

