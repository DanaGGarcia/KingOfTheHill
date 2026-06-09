// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Switcher.h"

#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "GameMode/KOTHGameMode.h"
#include "GameState/KOTHGameState.h"
#include "PlayerState/KOTHPlayerState.h"

class UTextBlock;
class AKOTHGameMode;

void USwitcher::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (WidgetSwitcher)
	{
		WidgetSwitcher->SetActiveWidgetIndex(0);
	}

	if (APlayerController* PC = GetOwningPlayer())
	{
		
		FInputModeUIOnly ModoUI;
		ModoUI.SetWidgetToFocus(TakeWidget()); 
		PC->SetInputMode(ModoUI);
		PC->SetShowMouseCursor(true);
	}
	
	if (Button_StartPlay)
	{
		if (GetOwningPlayer() && GetOwningPlayer()->HasAuthority())
		{
			Button_StartPlay->SetVisibility(ESlateVisibility::Visible);
			Button_StartPlay->OnClicked.AddDynamic(this, &USwitcher::OnPlayButtonClicked);
		}
		else
		{
			Button_StartPlay->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	GetWorld()->GetTimerManager().SetTimer(
		UpdateListTimer,
		this,
		&USwitcher::UpdatePlayerList,
		1.0f,
		true
	);
}

void USwitcher::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
}

void USwitcher::OnPlayButtonClicked()
{
	bPlay = true;
	
	if (GetWorld() && GetWorld()->GetAuthGameMode())
	{
		AKOTHGameMode* GM = Cast<AKOTHGameMode>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			GM->StartMatch();
		}
	}

	if (bPlay)
	{
		ChangeWidget();
	}
}

void USwitcher::ChangeWidget()
{
	if (WidgetSwitcher)
	{
		WidgetSwitcher->SetActiveWidgetIndex(1);
	}

	if (APlayerController* PC = GetOwningPlayer())
	{
		FInputModeGameOnly ModoJuego;
		PC->SetInputMode(ModoJuego);
		PC->SetShowMouseCursor(false); 
	}
}

void USwitcher::UpdatePlayerList()
{
	AKOTHGameState* GS = GetWorld()->GetGameState<AKOTHGameState>();

	if (!GS || !ListPlayers) return;
	ListPlayers->ClearChildren();

	int32 IndiceJugador = 0;
	
	for (APlayerState* PS : GS->PlayerArray)
	{
		AKOTHPlayerState* KOTHPS = Cast<AKOTHPlayerState>(PS);
		FString NombreFinal = KOTHPS->GetPlayerName();
		if (KOTHPS)
		{
			UTextBlock* Players = NewObject<UTextBlock>(this);
			if (Players)
			{
				
				Players->SetText(FText::FromString(NombreFinal));
				
				ListPlayers->AddChild(Players);
			}
		}
		IndiceJugador++;
	}
}
