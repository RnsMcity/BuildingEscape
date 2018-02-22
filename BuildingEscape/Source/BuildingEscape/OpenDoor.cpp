// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor() {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay() {
	Super::BeginPlay();

	// ...
	Owner = GetOwner();

	if (PressurePlate == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("%s is missing pressure plate!"), *GetOwner()->GetName());
	}
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	// Poll trigger volume every frame
	// if actor that opens is in the volume, open door

	// if mass of actors on the plate is > X, then open door
	if (GetTotalMassOfActorsOnPlate() > 30) {
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	} else {
		CloseDoor();
	}

	/* Below is how the tutorial handled things; my implementation is preferred
	if (GetWorld()->GetTimeSeconds - LastDoorOpenTime > DoorCloseDelay) {
		CloseDoor();
	}
	*/
}

float UOpenDoor::GetTotalMassOfActorsOnPlate() {
	float TotalMass = 0.0f;

	if (PressurePlate == nullptr) {
		return TotalMass;
	}

	TArray<AActor*> OverlappingActors;
	// find all overlapping actor
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	// iterate through list of actors, add their masses
	for (const auto& actor : OverlappingActors) {
		TotalMass += actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}

void UOpenDoor::OpenDoor() {
	/*if (OpenAngle < 60) {
		FRotator NewRotation = FRotator(0.0f, OpenAngle++, 0.0f);

		Owner->SetActorRotation(NewRotation);
	}*/
	OnOpenRequest.Broadcast();
}

void UOpenDoor::CloseDoor() {
	/*if (OpenAngle > 0) {
		FRotator NewRotation = FRotator(0.0f, OpenAngle--, 0.0f);

		Owner->SetActorRotation(NewRotation);
	}*/
}


