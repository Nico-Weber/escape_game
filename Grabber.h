// Copyright 2021 by Nico André Weber all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_ESCAPE_1_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	AController* PlayerController = nullptr;
	UPROPERTY(EditAnywhere)
		float RaycastReach = 140.0f;

	UPROPERTY()
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UPROPERTY()
	UInputComponent* InputComponent = nullptr;

	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;
	//FHitResult Hit;

	void Grab();
	void Released();
	void FindPhysicsHandle();
	void SetupInputComponent();
	void DebugViewingVector();

	//@return first Actor within reach with physics body.
	FHitResult GetFirstPhysicsBodyInReach();

	FVector GetPlayersReach();
	FVector GetPlayersWorldPos();
};
