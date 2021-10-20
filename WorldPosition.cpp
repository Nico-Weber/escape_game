// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldPosition.h"

// Sets default values for this component's properties
UWorldPosition::UWorldPosition()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWorldPosition::BeginPlay()
{
	Super::BeginPlay();

	FString objectName = GetOwner()->GetName();
	FString componentName = this->GetName();
	FString objectPos = GetOwner()->GetActorLocation().ToString();
	//alternative: GetOwner()->GetTransform().GetLocation().ToString();
	//			   GetOwner()->GetActorTransform().ToString() <-- whole transform [...] .GetLocation().ToString()
	
	UE_LOG(LogTemp, Warning, TEXT("%s component is attached to %s"), *componentName, *objectName);
	UE_LOG(LogTemp, Warning, TEXT("%s's position: %s "), *objectName, *objectPos);
	
}


// Called every frame
void UWorldPosition::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

