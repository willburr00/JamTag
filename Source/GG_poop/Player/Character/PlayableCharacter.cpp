// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayableCharacter.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

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

    // Stamina
    currentStamina = maxStamina;
    currentThresholdStamina = 0.0f;

    // Dash
    dashNewTimer = 0.0f;
    dashLastest = 1.0f;
    dashCooldown = dashTime + 0.5f;

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

    if (isDashing)
    {
        Dash(DeltaTime);
        return;
    }
    
    FVector otherDir = GetAllPlayerDirection();
    FVector movingDirection = FVector::ZeroVector;

    // Don't want to move without stamina
    if (CanUseStamina())
    {
        // Get inputs values
        float fForwardValue = InputComponent->GetAxisValue(TEXT("MoveForward"));
        float fRightValue = InputComponent->GetAxisValue(TEXT("MoveRight"));

        movingDirection = FVector(fForwardValue, fRightValue, 0);

        // Normalize and save direction
        movingDirection.Normalize();
        m_vInputDirection = movingDirection;
    }
    else
    {
        m_vInputDirection = FVector::ZeroVector;
        currentThresholdStamina = thresholdStamina;
    }

    //If immobile, apply special coef
    if (movingDirection.IsNearlyZero())
    {
        movingDirection += (otherDir * stopCoefOthers);
        if (currentStamina < maxStamina)
        {
            currentStamina += DeltaTime * regenStamina;
            if (currentStamina >= currentThresholdStamina)
            {
                currentThresholdStamina = 0;
            }

            if (currentStamina >= maxStamina)
            {
                currentStamina = maxStamina;
            }
        }
    }
    else
    {
        currentStamina -= DeltaTime * consumeStamina;
        bool sameWay = FVector::DotProduct(movingDirection, otherDir) >= 0;

        // Add other direction according to a same/other way coef
        movingDirection += otherDir * (sameWay ? movingSameWayCoefOthers : movingOtherWayCoefOthers);
    }


    GEngine->AddOnScreenDebugMessage(0, 5.0f, FColor::Yellow, FString::FromInt(GetCurrentPlayerStamina()) + "/" + FString::FromInt(GetMaxPlayerStamina()));

    AddMovementInput(movingDirection);
}

// Called to bind functionality to input
void APlayableCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("MoveForward"));
    PlayerInputComponent->BindAxis(TEXT("MoveRight"));

    PlayerInputComponent->BindAction(TEXT("Dash"), IE_Pressed, this, &APlayableCharacter::OnDash);
}

void APlayableCharacter::OnDash()
{
    if (CanUseStamina() && currentStamina >= dashCost && !isDashing)
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, "Start dash");

        currentStamina -= dashCost;
        dashLastest = GetWorld()->TimeSeconds;
        dashNewTimer = dashLastest + dashCooldown;

        isDashing = true;
        dashStart = GetActorLocation();
        FVector direction = GetActorForwardVector();
        direction.Normalize();
        dashEnd = dashStart + direction * dashDistance;


        FCollisionQueryParams TraceParams(FName(TEXT("TraceUsableActor")), true, this);
        TraceParams.bTraceAsyncScene = true;
        TraceParams.bReturnPhysicalMaterial = false;
        TraceParams.AddIgnoredActor(this);
        FHitResult Hit(ForceInit);

        GetWorld()->LineTraceSingleByChannel(Hit, dashStart, dashEnd, ECC_WorldStatic, TraceParams);

        DrawDebugLine(GetWorld(), dashStart, dashEnd, FColor::Red, true, 10.0f);
    }
}

void APlayableCharacter::Dash(float deltaTime)
{
    SetActorLocation(FMath::Lerp<FVector>(dashStart, dashEnd, (GetWorld()->TimeSeconds - dashLastest) / dashTime));

    if (FVector::Distance(GetActorLocation(), dashEnd) < 10.0f)
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, "End dash");
        isDashing = false;
    }
}

FVector APlayableCharacter::GetAllPlayerDirection() const
{
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayableCharacter::StaticClass(), FoundActors);

    FVector allVec(0, 0, 0);

    for (auto actor : FoundActors)
    {
        APlayableCharacter* pCharacter = Cast<APlayableCharacter>(actor);
        if (pCharacter != NULL && pCharacter != this)
        {
            FVector pDir = pCharacter->GetInputDirection();
            if (!pDir.IsNearlyZero())
            {
                allVec += pDir;
            }
        }
    }

    if (!allVec.IsNormalized())
    {
        allVec.Normalize();
    }

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

int APlayableCharacter::GetMaxPlayerStamina() const
{
    return maxStamina;
}

int APlayableCharacter::GetCurrentPlayerStamina() const
{
    return currentStamina;
}

float APlayableCharacter::GetRatioPlayerStamina() const
{
    if (maxStamina <= 0)
    {
        return 0.0f;
    }
    return currentStamina / maxStamina;
}

bool APlayableCharacter::CanUseStamina() const
{
    return currentStamina > 0 && currentStamina >= currentThresholdStamina;
}