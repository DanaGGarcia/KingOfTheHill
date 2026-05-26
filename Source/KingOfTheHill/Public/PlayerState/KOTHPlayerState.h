// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "KOTHPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class KINGOFTHEHILL_API AKOTHPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AKOTHPlayerState();

	UPROPERTY(Replicated, BlueprintReadOnly)
	int32 ScorePoints;

	void AddPoint();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	
};
