// Copyright Adam Horvath-Voros 2017

#pragma once
#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDoorEvent); //Create a class so we can call C++ from Blueprint

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROOMESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable)  //Create a class so we can call C++ from Blueprint
	FDoorEvent OnOpen; 

	UPROPERTY(BlueprintAssignable)  //Create a class so we can call C++ from Blueprint
	FDoorEvent OnClose;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


private:	
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;
	
	AActor* Owner; //The owning door

	//Returns total mass in kgs
	float GetTotalMassOfActorsOnPlate();

	UPROPERTY(EditAnywhere)
	float TriggerMass = 490.0f;
};
