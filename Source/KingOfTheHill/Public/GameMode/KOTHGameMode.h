// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "KOTHGameMode.generated.h"

/**
 * 
 */
UCLASS()
class KINGOFTHEHILL_API AKOTHGameMode : public AGameMode
{
	GENERATED_BODY()

public:

	AKOTHGameMode();

	virtual void BeginPlay() override;

	virtual void StartMatch() override;

	FTimerHandle MatchTimer;

	FTimerHandle StartMatchTimer;

	float MatchTime;

	UPROPERTY(EditDefaultsOnly)
	float MatchStart = 3;

	void UpdateTimer();

	void EndMatch();
	
};
