// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/HUDPlayer.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "GameState/KOTHGameState.h"
#include "PlayerState/KOTHPlayerState.h"
#include "Widget/PlayerRowWidget.h"

void UHUDPlayer::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	AKOTHGameState* GS = GetWorld()->GetGameState<AKOTHGameState>();
	if (GS && TimeText)
	{
		FString TimeString = FString::Printf(TEXT("%02i"), GS->RemainingTime);
		TimeText->SetText(FText::FromString(TimeString));
	}

	if (GS && WinnerText && ImageWinner)
	{
		WinnerText->SetText(FText::FromString(GS->WinnerText));

		ImageWinner->SetVisibility(GS->WinnerText.IsEmpty()? ESlateVisibility::Hidden: ESlateVisibility::Visible);
	}
}

void UHUDPlayer::NativeConstruct()
{
	Super::NativeConstruct();
	
	GetWorld()->GetTimerManager().SetTimer(
		UpdateListTimer,
		this,
		&UHUDPlayer::UpdatePlayerList,
		1.0f,
		true
	);
}

void UHUDPlayer::UpdatePlayerList()
{
	AKOTHGameState* GS = GetWorld()->GetGameState<AKOTHGameState>();

	if (!GS || !PositionList || !PlayerRowClass)return;

	PositionList->ClearChildren();

	int32 IndiceJugador = 0;

	for (APlayerState* PS : GS->PlayerArray)
	{
		AKOTHPlayerState* KOTHPS = Cast<AKOTHPlayerState>(PS);

		if (KOTHPS)
		{
			UPlayerRowWidget* FilaWidget = CreateWidget<UPlayerRowWidget>(GetOwningPlayer(),PlayerRowClass);

			if (FilaWidget)
			{
				FString NombreFinal = KOTHPS->GetPlayerName();
				
				if (FilaWidget->TextName)
				{
					FilaWidget->TextName->SetText(FText::FromString(NombreFinal));
				}

				if (FilaWidget->TextPoints)
				{
					FString PuntosString = FString::Printf(TEXT("%i pts"),KOTHPS->ScorePoints);
					FilaWidget->TextPoints->SetText(FText::FromString(PuntosString));
				}
					PositionList->AddChild(FilaWidget);
			}
		}
		IndiceJugador++;
	}
}
