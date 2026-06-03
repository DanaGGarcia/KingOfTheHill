// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDPlayer.generated.h"

/**
 * 
 */

class UImage;
class UTextBlock;
class UVerticalBox;

UCLASS()
class KINGOFTHEHILL_API UHUDPlayer : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TimeText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> PositionList;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PlayerRowClass;

	FTimerHandle UpdateListTimer;

	void UpdatePlayerList();

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> WinnerText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ImageWinner;
};
