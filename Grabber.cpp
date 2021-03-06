// Copyright 2021 by Nico André Weber all rights reserved.


#include "Grabber.h" //stays bottom one all the time
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = GetWorld()->GetFirstPlayerController(); //for getting players viewing location/rotation
	
	FindPhysicsHandle();
	SetupInputComponent();
}

//Checks for PhysicsHandleComponent on all actors that own it (in this case our player-pawn), it allows the player to hold/grab things
void UGrabber::FindPhysicsHandle() {
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle) {
		UE_LOG(LogTemp, Error, TEXT("No UPhysicsHandleComponent was found on %s"), *GetOwner()->GetName());
	}
}

//Checks for inputbindings (which button is pressed, released ect.) and calls functions based on it
void UGrabber::SetupInputComponent() {
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Released);
	}
}

//When Grab-button is pressed, he raycasts for grabbable physic-objects. When the grabbed object isn't null, grab that object
void UGrabber::Grab() {
	//transfers raycasted Object of FHitResult to UPrimitiveComponent* so GrabComponentAtLocation() can use it
	FHitResult Hit = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = Hit.GetComponent();
	AActor* ActorHit = Hit.GetActor();

	if (ActorHit) {
		if (!PhysicsHandle) { return; }
		PhysicsHandle->GrabComponentAtLocation(ComponentToGrab, NAME_None, GetPlayersReach()); //similar to boolean Grab = true;
	}
}

//When Grab-Button is released, release grabbed objects
void UGrabber::Released() {
	if (!PhysicsHandle) { return; }
	PhysicsHandle->ReleaseComponent(); //similar to boolean Grab = false;
}

void UGrabber::DebugViewingVector() {
	DrawDebugLine(
		GetWorld(),
		GetPlayersWorldPos(),
		GetPlayersReach(),
		FColor(255, 0, 0),
		false,
		0.0f,
		0,
		5
	);
}

//Gets the first object hit by raycast
FHitResult UGrabber::GetFirstPhysicsBodyInReach(){

	FHitResult Hit;
	//FCollisionQueryParams(FName InTraceTag ,bool bInTraceComplex, const AActor* InIgnoreActor)
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner()); //-InTraceTag: Tagname maybe... -bInTraceComplex: use complex collision for raycasting checks -IgnoreActor: it doesn't raycast the player itself

	GetWorld()->LineTraceSingleByObjectType( //raycasting function
		OUT Hit, //var to be fed with raycasted object
		GetPlayersWorldPos(), //startpoint
		GetPlayersReach(), //endpoint
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),//checks for objects of collisiongroup PhysicsBody (everything that has physics on)
		TraceParams
	);
	return Hit;
};

FVector UGrabber::GetPlayersReach() {
	PlayerController->GetPlayerViewPoint(OUT PlayerViewpointLocation, OUT PlayerViewpointRotation); //out parameters are GetByReferences
	//UE_LOG(LogTemp, Warning, TEXT(" \nPlayerLocation: %s\nPlayerDirection: %s"), *PlayerViewpointLocation.ToString(), *PlayerViewpointRotation.ToString());
	return (PlayerViewpointLocation + PlayerViewpointRotation.Vector() * RaycastReach);
};

FVector UGrabber::GetPlayersWorldPos() {
	FVector loc;
	FRotator rot;
	GetOwner()->GetActorEyesViewPoint(loc, rot);

	return loc;
	
};

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	GetPlayersReach();
	
	if (!PhysicsHandle) { return; }
	if (PhysicsHandle->GrabbedComponent) { //if something(target) is grabbed set grabbed item to be infront of player
		PhysicsHandle->SetTargetLocation(GetPlayersReach());
	}
	//DebugViewingVector();

}

