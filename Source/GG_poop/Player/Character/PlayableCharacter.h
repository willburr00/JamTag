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

    UFUNCTION(BlueprintCallable, Category = "Player")
        int GetPlayerID() const;
    UFUNCTION(BlueprintCallable, Category = "Player")
        int GetTileCount() const;
    UFUNCTION(BlueprintCallable, Category = "Player")
        void SetTileCount(int newCount);
    UFUNCTION(BlueprintCallable, Category = "Player")
        void RemoveFromTileCount(int number);
    UFUNCTION(BlueprintCallable, Category = "Player")
        void AddFromTileCount(int number);

private:
    FVector GetAllPlayerDirection() const;

    int PlayerId;
    int TileCount;

};
