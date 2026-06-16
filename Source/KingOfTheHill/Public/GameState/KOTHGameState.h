// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "KOTHGameState.generated.h"

/**
 * 
 */

class AKOTHGameMode;
class AKOTHPlayerState;

UCLASS()
class KINGOFTHEHILL_API AKOTHGameState : public AGameState
{
	GENERATED_BODY()

public:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(Replicated, BlueprintReadOnly)
	int32 RemainingTime;
	
	void StartGameClock();
	
	void AwardPointToPlayer(APlayerState* TargetPlayer);
	
	FTimerHandle GameClockTimerHandle;

	void AdvanceClock();

	UPROPERTY(Replicated, BlueprintReadOnly)
	FString WinnerText;

	UFUNCTION()
	void EndMatch();

	FTimerHandle RestartTimer;
};
