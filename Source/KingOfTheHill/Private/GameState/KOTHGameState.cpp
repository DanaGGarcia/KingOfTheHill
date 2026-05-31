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
			AKingOfTheHillCharacter* Character = Cast<AKingOfTheHillCharacter>(Actor);
			if (Character)
			{
				Character->CancelAddPoints();
			}
		}
		
		if (AKOTHGameMode* GM = Cast<AKOTHGameMode>(GetWorld()->GetAuthGameMode()))
		{
			GM->EndMatch();
		}
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