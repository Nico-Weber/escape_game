// Copyright 2021 by Nico André Weber all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_ESCAPE_1_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float TotalMassOfActors() const;
	void FindAudioComponent();
	void PressurePlateCheck();

private:
	float InitialYaw;
	float CurrentYaw; 
	float DoorLastOpened = 0.0f;
	bool IsOpened = false;
	TArray <AActor*>ActorsInTrigger;
	bool isOpening = false;

	enum DoorOpenMode {
		NORMAL,
		MASS,
	};

	UPROPERTY(EditAnywhere)
	int Condition = 0; //UPROPERTY(EditAnywhere, Category); DoorOpenMode mode = MASS; //wont work

	UPROPERTY(EditAnywhere) //make it editable in Unreal Editor
	ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY(EditAnywhere) //make it editable in Unreal Editor
	float OpenAngle = 90.0f;

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 2.0f;

	UPROPERTY(EditAnywhere)
	float DoorOpenSpeed = 1.5f;

	UPROPERTY(EditAnywhere)
	float DoorCloseSpeed = 1.5f;

	UPROPERTY(EditAnywhere)
		float MassToOpenDoors = 50.0f;

	UPROPERTY(EditAnywhere)
		bool OnOpenSound = true;

	UPROPERTY(EditAnywhere)
		bool OnCloseSound = false;

	UPROPERTY()
	UAudioComponent* AudioComponent = nullptr; //the "U" stands for UPROPERTY(EditAnywhere) I guess


};
