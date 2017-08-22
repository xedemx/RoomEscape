// Copyright Adam Horvath-Voros 2017

#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROOMESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	//How far ahead of the player can we reach
	UPROPERTY(EditAnywhere)
	float Reach = 150.0f;
	
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	
	UInputComponent* InputComponent = nullptr;

	//Ray-cast and grab what's in reach
	void Grab();

	//Called when grab is released
	void Release();

	//Find attached physics handle, so we can move objects
	void FindPhysicsHandleComponent();

	//Setup (assumed) attached input component
	void SetupInputComponent();

	//Return hit for the first physics body we have in reach
	const FHitResult GetFirstPhysicsBodyInReach();

	//Returns current start of reach
	FVector GetReachLineStart();

	//Returns current end of reach
	FVector GetReachLineEnd();
};
