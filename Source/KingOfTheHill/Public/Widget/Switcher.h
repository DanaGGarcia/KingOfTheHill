// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Switcher.generated.h"

class UVerticalBox;
class UButton;
class UWidgetSwitcher;
/**
 * 
 */
UCLASS()
class KINGOFTHEHILL_API USwitcher : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UFUNCTION()
	void OnPlayButtonClicked();
	
	void ChangeWidget();

	void UpdatePlayerList();

protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_StartPlay;

	UPROPERTY()
	bool bPlay = false;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> ListPlayers;

	FTimerHandle UpdateListTimer;
	
};
