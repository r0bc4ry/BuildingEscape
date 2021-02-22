// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoorComponent.h"

#include "Components/AudioComponent.h"

#define OUT

// Sets default values for this component's properties
UOpenDoorComponent::UOpenDoorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoorComponent::BeginPlay()
{
	Super::BeginPlay();

	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	OpenAngle += InitialYaw;

	FindAudio();

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has OpenDoorComponent but no PressurePlate set."), *GetOwner()->GetName())
	}

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void UOpenDoorComponent::FindAudio()
{
	Audio = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (Audio == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Audio not found on %s."), *GetOwner()->GetName());
	}
}


// Called every frame
void UOpenDoorComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TotalMassOfActors() > MassToOpenDoors)
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else
	{
		if (GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay)
		{
			CloseDoor(DeltaTime);
		}
	}
}

void UOpenDoorComponent::OpenDoor(float DeltaTime)
{
	CurrentYaw = FMath::FInterpTo(CurrentYaw, OpenAngle, DeltaTime, DoorOpenSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);

	if (!Audio || OpenDoorSound) return;
	UE_LOG(LogTemp, Warning, TEXT("Play"));
	Audio->Play();
	OpenDoorSound = true;
	CloseDoorSound = false;
}

void UOpenDoorComponent::CloseDoor(float DeltaTime)
{
	CurrentYaw = FMath::FInterpTo(CurrentYaw, InitialYaw, DeltaTime, DoorCloseSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);

	if (!Audio || CloseDoorSound) return;
	Audio->Play();
	OpenDoorSound = false;
	CloseDoorSound = true;
}

float UOpenDoorComponent::TotalMassOfActors() const
{
	float TotalMass = 0.0f;

	if (PressurePlate != nullptr)
	{
		TArray<AActor*> OverlappingActors;
		PressurePlate->GetOverlappingActors(OUT OverlappingActors);
		for (AActor* Actor : OverlappingActors)
		{
			TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		}
	}

	return TotalMass;
}
