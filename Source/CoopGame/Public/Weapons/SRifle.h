// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWeapon.h"
#include "SRifle.generated.h"

UCLASS()
class COOPGAME_API ASRifle : public ASWeapon
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASRifle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Function that handle when the gun fires
	virtual void Fire() override;

	//Muzzle effects
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName MuzzleSocketName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName TracerTargetName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* MuzzleEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* ImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* TracerEffect;

};
