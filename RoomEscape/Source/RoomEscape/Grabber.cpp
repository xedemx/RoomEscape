// Copyright Adam Horvath-Voros 2017

#include "Grabber.h"
#include "Engine/World.h"
#include "Gameframework/Pawn.h"
#include "Gameframework/PlayerController.h"
#include "DrawDebugHelpers.h"

#define OUT  //Just a reminder when we receive value into function parameters

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
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

///Look for attached Physics Handle
void UGrabber::FindPhysicsHandleComponent()
{	
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No Physics Handle is attached to Pawn/Character: %s !!"), *(GetOwner()->GetName()))
	}
}

///Look for attached Input Component (only appears at run time)
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{//Bind the input axis
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Input Component is attached to Default Pawn: %s !"), *(GetOwner()->GetName()))
	}
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PhysicsHandle) { return; }
	if (PhysicsHandle->GrabbedComponent)
	{	// if the physics handle is attached, move the object what we're holding
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}
}


void UGrabber::Grab()
{
	//Debug only: grab pressed
	//UE_LOG(LogTemp, Warning, TEXT("Grab pressed."))

	///LINE TRACE and see if we reach any actors which physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent(); //we get a mesh component in this case
	auto ActorHit = HitResult.GetActor();

	///If we hit something attach physics handle
	if (ActorHit)
	{
		if (!PhysicsHandle) { return; }
		PhysicsHandle->GrabComponent(
		ComponentToGrab,
		NAME_None, //No bones needed, we are not grabbing characters just objects
		ComponentToGrab->GetOwner()->GetActorLocation(),
		true  //allow rotation
	);
	}
}

void UGrabber::Release()
{
	//Debug only: grab released
	//UE_LOG(LogTemp, Warning, TEXT("Grab released."))

	/// Release physics handle
	if (!PhysicsHandle) { return; }
	PhysicsHandle->ReleaseComponent();
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	///Line-trace(Ray-cast) out to reach distance
	FHitResult HitResult;
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), //we can only grab objects which have a physics body!
		TraceParameters
	);

	///See what we hit, if we hit something
	AActor* ActorHit = HitResult.GetActor();

	//Debug only: log out what we have hit
	//if (ActorHit)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("We have hit:  %s"), *(ActorHit->GetName()))
	//}

	return HitResult;
}

FVector UGrabber::GetReachLineStart()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT	PlayerViewPointRotation
	);

	return PlayerViewPointLocation;
}


FVector UGrabber::GetReachLineEnd()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation, 
		OUT	PlayerViewPointRotation  
	);

	//Debugging only: get the viewpoint's location and rotation
	//UE_LOG(LogTemp, Warning, TEXT("Location: %s, Rotation: %s"),
	//	*PlayerViewPointLocation.ToString(),
	//	*PlayerViewPointRotation.ToString()
	//)

	///Debugging only: Draw a red trace in the world to visualize reach 
	//DrawDebugLine(
	//	GetWorld(),
	//	PlayerViewPointLocation,
	//	LineTraceEnd,
	//	FColor(255, 0, 0),
	//	false,
	//	0.0f,
	//	0.0f,
	//	8.0f
	//);

	return PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);
}