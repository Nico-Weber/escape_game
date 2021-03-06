// Copyright 2021 by Nico André Weber all rights reserved.


#include "OpenDoor.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Components/AudioComponent.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// never call GetOwner() in constructor
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	OpenAngle += InitialYaw;

	PressurePlateCheck();
	FindAudioComponent();
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//if +- is hindering <-- only absolute numbers (betrag)
	float CurrentYawAbsolute = abs(CurrentYaw);
	float OpenAngleAbsolute = abs(OpenAngle);

	switch (Condition) {
		case NORMAL:
			PressurePlate->GetOverlappingActors(OUT ActorsInTrigger);

			if (PressurePlate && ActorsInTrigger.Num() > 0) {
				DoorLastOpened = GetWorld()->GetTimeSeconds();
				//UE_LOG(LogTemp, Warning, TEXT("Player in Trigger"));
				isOpening = true;
			}
			if (!(CurrentYawAbsolute > OpenAngleAbsolute - OpenAngleAbsolute * 0.10 && CurrentYawAbsolute < OpenAngleAbsolute + OpenAngleAbsolute * 0.10) && isOpening) { //open door till fully opened (%tolerancezone of OpenAngle) TODO fix negative rotations
				OpenDoor(DeltaTime);
				//UE_LOG(LogTemp, Warning, TEXT("CurrentYaw: %f; OpenAngle: %f"), CurrentYaw, OpenAngle);
			} 
			else if (!isOpening && GetWorld()->GetTimeSeconds() >= DoorLastOpened + DoorCloseDelay) {
					CloseDoor(DeltaTime);
			}
			else {
				isOpening = false;
			}
			break;

		case MASS:
			if (PressurePlate && TotalMassOfActors() > MassToOpenDoors) { //1. null check 2. check if both overlap
				OpenDoor(DeltaTime);
				DoorLastOpened = GetWorld()->GetTimeSeconds();
			}
			else {
				if (GetWorld()->GetTimeSeconds() >= DoorLastOpened + DoorCloseDelay)
				CloseDoor(DeltaTime);
			}
			break;
	}
}

void UOpenDoor::OpenDoor(float DeltaTime){

	CurrentYaw = FMath::FInterpTo(CurrentYaw, OpenAngle, DeltaTime, DoorOpenSpeed); // Lerp Function with DeltaTime
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);

	if (!AudioComponent) { return; }
	if (!IsOpened && OnOpenSound)
	AudioComponent->Play();
	IsOpened = true;
}

void UOpenDoor::CloseDoor(float DeltaTime){
	CurrentYaw = FMath::FInterpTo(CurrentYaw, InitialYaw, DeltaTime, DoorCloseSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);

	if (!AudioComponent) { return; }
	if(IsOpened && OnCloseSound)
	AudioComponent->Play();
	IsOpened = false;
}

float UOpenDoor::TotalMassOfActors() const{
	float TotalMass = 0.0f;
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) { return TotalMass; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	for (AActor* Actor : OverlappingActors) {
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	/*
	for (int i = 0; i < OverlappingActors.Num(); i++) {
		TotalMass += OverlappingActors[i]->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	*/
		// UE_LOG(LogTemp, Display, TEXT("%d"), (int)TotalMass); // Player Mass is always MassInKg + 40... why?
	return TotalMass;
}

void UOpenDoor::FindAudioComponent() {
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();

	if (!AudioComponent) {
		UE_LOG(LogTemp, Error, TEXT("%s missing audio components!"), *GetOwner()->GetName());
	}
}

void UOpenDoor::PressurePlateCheck() {
	if (!PressurePlate) { //if PressurePlate == null
		UE_LOG(LogTemp, Error, TEXT("%s has the open door component on it, but no pressureplate set!"), *GetOwner()->GetName());
	}
}