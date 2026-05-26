// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PointZone.generated.h"

class UBoxComponent;

UCLASS()
class KINGOFTHEHILL_API APointZone : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APointZone();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	
private:
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UBoxComponent> PointZoneBox;
};
