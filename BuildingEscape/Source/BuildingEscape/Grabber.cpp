// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Components/PrimitiveComponent.h"

#define OUT
#define IN

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// ...
	FString ObjectName = GetOwner()->GetName();
	UE_LOG(LogTemp, Warning, TEXT("%s reporting for duty!"), *ObjectName);

	FindPhysicsHandleComponent();
	SetupInputComponent();
}

void UGrabber::AttemptGrab() {
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed!"));

	// Try to reach (line trace) any actors with PhysicsBody collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	// If we hit something then attach a physics handle
	// TODO attach physics handle
	if (ActorHit) {
		/*FVector ComponentActorLocation = ComponentToGrab->GetOwner()->GetActorLocation();*/
		
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None,
			HitResult.Location,
			true
		);
	}
}

void UGrabber::ReleaseGrab() {
	UE_LOG(LogTemp, Warning, TEXT("Grab released!"));
	// Release physics handle
	PhysicsHandle->ReleaseComponent();
}

// Checks if the grabbed item is still grabbable
bool UGrabber::CheckGrab() {
	// Try to reach (line trace) any actors with PhysicsBody collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();
	
	bool result = false; 

	if (ActorHit != nullptr) {
		if (ComponentToGrab == PhysicsHandle->GrabbedComponent) {
			result = true;
		}
	}

	// if result is still false, we shouldn't be grabbing the object still!
	return result;
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	// if physics handle is attached
	if (PhysicsHandle->GrabbedComponent) {
		// check if the object is still in the reach range
		//if (CheckGrab()) {
			// then move the object we're holding
			// Get the beginning of the line
		FVector PlayerViewPointLocation = GetReachLineStart();
		// Calculate the end of the line
		FVector LineTraceEnd = GetReachLineEnd();
		// Set the target to the end
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
	// else nothing
}

// Returns start of reach line (Player's location really)

FVector UGrabber::GetReachLineStart() {
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation;
}


// Get end of line trace, needs starting location and rotation
FVector UGrabber::GetReachLineEnd() {
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
	/*DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor(255, 0, 0),
		false,
		0.0f,
		0,
		2.0f
	);*/
}

// Look for attached PhysicsHandle
void UGrabber::FindPhysicsHandleComponent() {
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle) {

	} else {
		UE_LOG(LogTemp, Error, TEXT("%s is missing PhysicsHandle!"), *GetOwner()->GetName());
	}
}

void UGrabber::SetupInputComponent() {
	// Look for attached Input Component
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		// Bind the input action
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::AttemptGrab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::ReleaseGrab);
	} else {
		UE_LOG(LogTemp, Error, TEXT("%s is missing InputComponent!"), *GetOwner()->GetName());
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach() {
	/// Draws a line own from the user's Pawn

	//DrawDebugLine(
	//	GetWorld(),
	//	PlayerViewPointLocation,
	//	LineTraceEnd,
	//	FColor(255, 0, 0),
	//	false,
	//	0.0f,
	//	0,
	//	2.0f
	//	);

	/// Setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	///  Line-trace (AKA Ray-casting) out to reach distance
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	return HitResult;
}
