// Fill out your copyright notice in the Description page of Project Settings.


#include "GrabberComponent.h"

#include "DrawDebugHelpers.h"

#define OUT

// Sets default values for this component's properties
UGrabberComponent::UGrabberComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabberComponent::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();
	FindInput();
}

void UGrabberComponent::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No PhysicsHandleComponent on %s!"), *GetOwner()->GetName());
	}
}

void UGrabberComponent::FindInput()
{
	Input = GetOwner()->FindComponentByClass<UInputComponent>();
	if (Input)
	{
		UE_LOG(LogTemp, Warning, TEXT("Input found on %s."), *GetOwner()->GetName());
		Input->BindAction("Grab", EInputEvent::IE_Pressed, this, &UGrabberComponent::Grab);
		Input->BindAction("Grab", EInputEvent::IE_Released, this, &UGrabberComponent::Release);
	}
}

void UGrabberComponent::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab!"));

	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	if (HitResult.GetActor())
	{
		// Get players viewpoint
		FVector PlayerViewPointLocation;
		FRotator PlayerViewPointRotator;
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
			OUT PlayerViewPointLocation,
			OUT PlayerViewPointRotator
		);

		if (PhysicsHandle == nullptr) return;
		PhysicsHandle->GrabComponentAtLocation(
			ComponentToGrab,
			NAME_None,
			PlayerViewPointLocation + PlayerViewPointRotator.Vector() * Reach
		);
	}

	// Try and reach any Actor with physics body collisions channgel set
	// Attach physics handle
}

void UGrabberComponent::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Released!"));

	if (PhysicsHandle == nullptr) return;
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->ReleaseComponent();
	}
}

// Called every frame
void UGrabberComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandle == nullptr) return;
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetPlayerReach());
	}
}

FHitResult UGrabberComponent::GetFirstPhysicsBodyInReach() const
{
	// Raycast
	FHitResult HitResult;
	FCollisionQueryParams LineTraceParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		GetPlayerLocation(),
		GetPlayerReach(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		LineTraceParams
	);

	// See what raycast hits
	AActor* ActorHit = HitResult.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *ActorHit->GetName());
	}

	return HitResult;
}

FVector UGrabberComponent::GetPlayerReach() const
{
	// Get players viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotator;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotator
	);

	return PlayerViewPointLocation + PlayerViewPointRotator.Vector() * Reach;
}

FVector UGrabberComponent::GetPlayerLocation() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotator;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotator
	);

	return PlayerViewPointLocation;
}
