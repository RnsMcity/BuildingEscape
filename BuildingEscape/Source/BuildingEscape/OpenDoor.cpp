// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"


static float TotalRotation = 0.0f;

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	// ...
	TotalRotation = 0.0f;
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	UE_LOG(LogTemp, Warning, TEXT("Rotation: %f"), TotalRotation);
	if (TotalRotation < 60) {
		FRotator NewRotation = FRotator(0.0f, TotalRotation++, 0.0f);

		AActor * Owner = GetOwner();
		Owner->SetActorRotation(NewRotation);
	}
}

