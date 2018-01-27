// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayableCharacter.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"


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

    // Get inputs values
    float fForwardValue = InputComponent->GetAxisValue(TEXT("MoveForward"));
    float fRightValue = InputComponent->GetAxisValue(TEXT("MoveRight"));

    FVector movingDirection(fForwardValue, fRightValue, 0);

    // Normalize and save direction
    movingDirection.Normalize();
    m_vInputDirection = movingDirection;

    FVector otherDir = GetAllPlayerDirection();

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(0, 15.0f, FColor::Red, otherDir.ToString());
        GEngine->AddOnScreenDebugMessage(1, 15.0f, FColor::Yellow, movingDirection.ToString());
    }
    
    //If immobile, apply special coef
    if (movingDirection.IsNearlyZero())
    {
        movingDirection += (otherDir * stopCoefOthers);
    }
    else
    {
        bool sameWay = FVector::DotProduct(movingDirection, otherDir) >= 0;

        // Add other direction according to a same/other way coef
        movingDirection += otherDir * (sameWay ? movingSameWayCoefOthers : movingOtherWayCoefOthers);
    }

    AddMovementInput(movingDirection);
}

// Called to bind functionality to input
void APlayableCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("MoveForward"));
    PlayerInputComponent->BindAxis(TEXT("MoveRight"));
}

FVector APlayableCharacter::GetAllPlayerDirection() const
{
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayableCharacter::StaticClass(), FoundActors);

    FVector allVec(0, 0, 0);

    int actorFound = 0;
    for (auto actor : FoundActors)
    {
        APlayableCharacter* pCharacter = Cast<APlayableCharacter>(actor);
        if (pCharacter != NULL && pCharacter != this)
        {
            FVector pDir = pCharacter->GetInputDirection();
            if (!pDir.IsNearlyZero())
            {
                allVec += pDir;
                actorFound++;
            }
        }
    }

    if (!allVec.IsNormalized())
    {
        allVec.Normalize();
    }

    return allVec;
}