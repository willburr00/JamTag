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

    // AHHHHHHHHHHHHH SO UGLY T_T
    TArray<FString> Out;
    GetName().ParseIntoArray(Out, TEXT("_"), true);
    PlayerId = FCString::Atoi(*Out[Out.Num() - 1]);

    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, GetName() + " with ID " + FString::FromInt(PlayerId));
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

    FVector allVec;

    for (auto actor : FoundActors)
    {
        APlayableCharacter* pCharacter = Cast<APlayableCharacter>(actor);
        if (pCharacter != NULL)
        {
            allVec += pCharacter->GetVelocity();
        }
    }

    allVec.Normalize();

    return allVec;
}

int APlayableCharacter::GetPlayerID() const
{
    return PlayerId;
}

int APlayableCharacter::GetTileCount() const
{
    return TileCount;
}

void APlayableCharacter::SetTileCount(int newCount)
{
    TileCount = newCount;
}

void APlayableCharacter::RemoveFromTileCount(int number)
{
    if (number < 0)
    {
        return;
    }

    TileCount -= number;
    if (TileCount < 0)
    {
        TileCount = 0;
    }
}

void APlayableCharacter::AddFromTileCount(int number)
{
    if (number < 0)
    {
        return;
    }

    TileCount += number;
}