// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Actors/PointZone.h"

#include "KingOfTheHillCharacter.h"
#include "Components/BoxComponent.h"
#include "GameMode/KOTHGameMode.h"


// Sets default values
APointZone::APointZone()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetReplicates(true);
	
	PointZoneBox = CreateDefaultSubobject<UBoxComponent>("PointZoneBox");
	RootComponent = PointZoneBox;
	PointZoneBox->SetBoxExtent(FVector(200.f, 200.f, 200.f));
	PointZoneBox->SetHiddenInGame(false);
}

// Called when the game starts or when spawned
void APointZone::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APointZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APointZone::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (!HasAuthority()) return;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Overlap");

	AKingOfTheHillCharacter* Character = Cast<AKingOfTheHillCharacter>(OtherActor);
	if (Character)
	{
		Character->StartAddPoints();
	}
}

void APointZone::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	if (!HasAuthority()) return;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "EndOverlap");
	
	AKingOfTheHillCharacter* Character = Cast<AKingOfTheHillCharacter>(OtherActor);
	if (Character)
	{
		Character->CancelAddPoints();
	}
	
}




