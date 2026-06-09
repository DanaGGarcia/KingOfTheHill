// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/MenuLan.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"

void UMenuLan::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Button_Quit)
	{
		Button_Quit->OnClicked.AddDynamic(this, &UMenuLan::OnButtonQuitClicked);
	}
	
	Button_Host->OnClicked.AddDynamic(this, &UMenuLan::OnButtonHostClicked);
	Button_Join->OnClicked.AddDynamic(this, &UMenuLan::OnButtonJoinClicked);
	
	FInputModeUIOnly InputMode;
	
	GetOwningPlayer()->SetInputMode(InputMode);
	GetOwningPlayer()->SetShowMouseCursor(true);
	
}

void UMenuLan::OnButtonHostClicked()
{
	FInputModeGameOnly InputMode;
	GetOwningPlayer()->SetInputMode(InputMode);
	GetOwningPlayer()->SetShowMouseCursor(false);
	
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, Level, true, TEXT("listen"));
}

void UMenuLan::OnButtonJoinClicked()
{
	FInputModeGameOnly InputMode;
	GetOwningPlayer()->SetInputMode(InputMode);
	GetOwningPlayer()->SetShowMouseCursor(false);
	
	const FString IP = EditableTextBox_IPAdress->GetText().ToString();
	UGameplayStatics::OpenLevel(this, *IP);
}

void UMenuLan::OnButtonQuitClicked()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}
