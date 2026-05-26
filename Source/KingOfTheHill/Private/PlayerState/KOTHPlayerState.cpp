// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/PlayerState/KOTHPlayerState.h"

#include "Net/UnrealNetwork.h"

AKOTHPlayerState::AKOTHPlayerState()
{
	bReplicates = true;
	ScorePoints = 0;
}

void AKOTHPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AKOTHPlayerState, ScorePoints);
}

void AKOTHPlayerState::AddPoint()
{
	ScorePoints++;
}



