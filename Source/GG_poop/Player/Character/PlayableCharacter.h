// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayableCharacter.generated.h"

UCLASS()
class GG_POOP_API APlayableCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    APlayableCharacter();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // Player ID
    UFUNCTION(BlueprintCallable, Category = "Player")
        int GetPlayerID() const;

    // Tile info
    UFUNCTION(BlueprintCallable, Category = "Player")
        int GetTileCount() const;
    UFUNCTION(BlueprintCallable, Category = "Player")
        void SetTileCount(int newCount);
    UFUNCTION(BlueprintCallable, Category = "Player")
        void RemoveFromTileCount(int number);
    UFUNCTION(BlueprintCallable, Category = "Player")
        void AddFromTileCount(int number);

    // Stamina
    UFUNCTION(BlueprintCallable, Category = "Player|Stamina")
        int GetMaxPlayerStamina() const;
    UFUNCTION(BlueprintCallable, Category = "Player|Stamina")
        int GetCurrentPlayerStamina() const;
    UFUNCTION(BlueprintCallable, Category = "Player|Stamina")
        float GetRatioPlayerStamina() const;

    FVector GetInputDirection() const
    {
        return m_vInputDirection;
    }

private:
    FVector GetAllPlayerDirection() const;

    // Trigger Method
    void OnDash();
    // Update Methode
    void Dash(float deltaTime);

    int PlayerId;
    int TileCount;
    FVector m_vInputDirection;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Coef", meta = (AllowPrivateAccess = "true"))
        float stopCoefOthers;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Coef", meta = (AllowPrivateAccess = "true"))
        float movingSameWayCoefOthers;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Coef", meta = (AllowPrivateAccess = "true"))
        float movingOtherWayCoefOthers;

    // Dash
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Dash", meta = (AllowPrivateAccess = "true"))
        int dashCost;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Dash", meta = (AllowPrivateAccess = "true"))
        float dashDistance;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Dash", meta = (AllowPrivateAccess = "true"))
        float dashTime;
    float dashCooldown;
    float dashLastest;
    float dashNewTimer;
    bool isDashing;
    FVector dashStart;
    FVector dashEnd;

    // Stamina
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Stamina", meta = (AllowPrivateAccess = "true"))
        int regenStamina;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Stamina", meta = (AllowPrivateAccess = "true"))
        int consumeStamina;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Stamina", meta = (AllowPrivateAccess = "true"))
        float maxStamina;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Stamina", meta = (AllowPrivateAccess = "true"))
        float thresholdStamina;
    float currentStamina;
    float currentThresholdStamina;
    bool CanUseStamina() const;
};
