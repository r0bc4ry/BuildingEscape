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

	// ...
}


// Called when the game starts
void UGrabberComponent::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"));
}


// Called every frame
void UGrabberComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get players viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotator;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotator);

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotator.Vector() * Reach;
	DrawDebugLine(GetWorld(), PlayerViewPointLocation, LineTraceEnd, FColor(0, 255, 0), false, 0.0f, 0, 5.0f);

	// See what it hits
}
