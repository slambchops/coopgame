// Fill out your copyright notice in the Description page of Project Settings.


#include "SGrenadeLauncher.h"
#include "SProjectileGrenade.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASGrenadeLauncher::ASGrenadeLauncher()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	//RootComponent = MeshComp;

	MuzzleSocketName = "MuzzleSocket";
}

// Called when the game starts or when spawned
void ASGrenadeLauncher::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASGrenadeLauncher::Fire()
{
	AActor* MyOwner = GetOwner();
	if (MyOwner && ProjectileClass)
	{
		FVector EyeLocation;
		FRotator EyeRotation;
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		GetWorld()->SpawnActor<ASProjectileGrenade>(ProjectileClass, MuzzleLocation, EyeRotation, SpawnParams);
	}


	/*if (ProjectileClass)
	{
		FVector MuzzleLocation = RootComponent->GetSocketLocation(MuzzleSocketName);
		FRotator MuzzleRotation = RootComponent->GetSocketRotation(MuzzleSocketName);

		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		//ActorSpawnParams.Instigator = this;

		// spawn the projectile at the muzzle
		GetWorld()->SpawnActor<ASProjectileGrenade>(ProjectileClass, MuzzleLocation, MuzzleRotation, ActorSpawnParams);
	}*/

}

// Called every frame
void ASGrenadeLauncher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

