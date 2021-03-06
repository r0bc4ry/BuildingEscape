// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"

#include "OpenDoorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UOpenDoorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	void FindAudio();
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float TotalMassOfActors() const;

private:
	float InitialYaw;
	float CurrentYaw;
	UAudioComponent* Audio = nullptr;
	bool OpenDoorSound = false;
	bool CloseDoorSound = true;

	UPROPERTY(EditAnywhere)
	float MassToOpenDoors = 50.0f;

	UPROPERTY(EditAnywhere)
	float OpenAngle = 90.0f;

	float DoorLastOpened = 0.0f;
	
	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 0.5f;
	
	UPROPERTY(EditAnywhere)
	float DoorOpenSpeed = 2.0f;

	UPROPERTY(EditAnywhere)
	float DoorCloseSpeed = 4.0f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
	AActor* ActorThatOpens = nullptr;
};
