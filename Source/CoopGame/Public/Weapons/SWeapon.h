// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWeapon.generated.h"

class USkeletalMeshComponent;
class UDamageType;
class UCameraShake;

UCLASS()
class COOPGAME_API ASWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASWeapon();

	virtual void StartFire();
	virtual void StopFire();

private:
	virtual void Fire();

protected:

	virtual void BeginPlay() override;

	//Weapon implementations need to override this
	virtual void FireImpl();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerFire();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* MeshComp;

	//The variable for the hitscan damage type
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<UCameraShake> FireCamShake;

	void DoFireCamShake();

	FTimerHandle TimerHandle_TimeBetweenShots;

	float LastFiredTime;

	//RPM - bullets per minute fired
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float RateOfFire;

	//Derived from rate of fire
	float TimeBetweenShots;
};
