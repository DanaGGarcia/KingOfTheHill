// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/GameState/KOTHGameState.h"

#include "Net/UnrealNetwork.h"


void AKOTHGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AKOTHGameState, RemainingTime);
}
