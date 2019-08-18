// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeapon.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "TimerManager.h"

// Sets default values
ASWeapon::ASWeapon()
{
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	RateOfFire = 600;

	SetReplicates(true); //Make sure weapons will always spawn on clients

	//Make sure Unreal Net code will update weapons even if they changes aren't occurring
	//Example: if player is shooting the same spot, then unreal will "filter" out events
	NetUpdateFrequency = 66.0f;
	MinNetUpdateFrequency = 33.0f;
}

void ASWeapon::StartFire()
{
	float FirstDelay = FMath::Max(LastFiredTime + TimeBetweenShots - GetWorld()->TimeSeconds, 0.0f); //Choose value that can be no less than 0
	GetWorldTimerManager().SetTimer(TimerHandle_TimeBetweenShots, this, &ASWeapon::Fire, TimeBetweenShots, true, FirstDelay);
	LastFiredTime = GetWorld()->TimeSeconds;
}

void ASWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_TimeBetweenShots);
}


void ASWeapon::BeginPlay()
{
	Super::BeginPlay();

	TimeBetweenShots = 60 / RateOfFire;
}


void ASWeapon::FireImpl()
{
	//OVERRIDE ME!!!
}

void ASWeapon::Fire()
{
	if (Role < ROLE_Authority)
	{
		ServerFire();
		FireImpl();
	}

	FireImpl();
	DoFireCamShake();
}


void ASWeapon::ServerFire_Implementation()
{
	Fire();
}


//Validate (mainly for anitcheat)
bool ASWeapon::ServerFire_Validate()
{
	return true;
}

void ASWeapon::DoFireCamShake()
{
	APawn* MyOwner = Cast<APawn>(GetOwner());
	if (MyOwner)
	{
		APlayerController* PC = Cast<APlayerController>(MyOwner->GetController());
		if (PC && FireCamShake)
		{
			PC->ClientPlayCameraShake(FireCamShake);
		}
	}
}
