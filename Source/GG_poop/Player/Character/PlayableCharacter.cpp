// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayableCharacter.h"


// Sets default values
APlayableCharacter::APlayableCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayableCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayableCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float fForwardValue = InputComponent->GetAxisValue(TEXT("MoveForward"));
	float fRightValue = InputComponent->GetAxisValue(TEXT("MoveRight"));

	FVector movingDirection(fForwardValue, fRightValue, 0);
	movingDirection.Normalize();

	AddMovementInput(movingDirection);
}

// Called to bind functionality to input
void APlayableCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"));
	PlayerInputComponent->BindAxis(TEXT("MoveRight"));

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "Player controller Axis attached on " + GetName());
	}
}

FVector APlayableCharacter::GetAllPlayerDirection() const
{

}