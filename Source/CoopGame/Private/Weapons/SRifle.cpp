// Fill out your copyright notice in the Description page of Project Settings.


#include "SRifle.h"
#include "CoopGame.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASRifle::ASRifle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MuzzleSocketName = "MuzzleSocket";
	TracerTargetName = "Target";

	BaseDamage = 20;

}

// Called when the game starts or when spawned
void ASRifle::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASRifle::FireImpl()
{
	//Trace the world from pawn eyes to cross hair location (center screen)

	//Find who owns weapon
	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		//Get location of the actors "eyes"
		FVector EyeLocation;
		FRotator EyeRotation;
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		//Get the direction the shot came from
		FVector ShotDirection = EyeRotation.Vector();

		//Determine how far we should trace the line
		FVector TraceEnd = EyeLocation + (ShotDirection * 10000);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(MyOwner); //ignore the player who owns this
		QueryParams.AddIgnoredActor(this); //ignore the gun itself
		QueryParams.bTraceComplex = true; //more expensive but better
		QueryParams.bReturnPhysicalMaterial = true;

		//Particle "Target" parameter
		FVector TracerEndPoint = TraceEnd;

		//Do the line trace and get the info where the bullet will hit
		FHitResult Hit;
		if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, COLLISION_WEAPON, QueryParams))
		{
			//Returns true if we hit something (Blocking hit) ... process damage
			AActor* HitActor = Hit.GetActor(); //Get the actor that was hit by the shot

			//Get the surface type that the bullet hit
			EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

			//Headshot damage check
			float ActualDamage = BaseDamage;
			if (SurfaceType == SURFACE_FLESH_VULNERABLE)
			{
				ActualDamage *= 4.0f;
			}

			//Apply the damage to the actor
			UGameplayStatics::ApplyPointDamage(HitActor, ActualDamage, ShotDirection, Hit, MyOwner->GetInstigatorController(), this, DamageType);

			//Determine which impact effect should be used based on material we hit
			UParticleSystem* SelectedEffect = nullptr;
			switch (SurfaceType)
			{
			case SURFACE_FLESH_DEFAULT:
			case SURFACE_FLESH_VULNERABLE:
				SelectedEffect = FleshImpactEffect;
				break;
			default:
				SelectedEffect = DefaultImpactEffect;
				break;
			}

			//Bullet hit effect
			if (SelectedEffect)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectedEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
			}

			//Let tracer know where the bullet hit
			TracerEndPoint = Hit.ImpactPoint;
		}

		PlayFireEffects(TracerEndPoint);

		if (Role == ROLE_Authority)
		{
			HitScanTrace.TraceTo = TracerEndPoint;
		}

		//DEBUG: Draw the line trace
		//DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::White, false, 1.0f, 0, 1.0f);
			
	}
}

void ASRifle::PlayFireEffects(FVector TracerEndPoint)
{
	//Create the muzzle effect if it exists
	if (MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName);
	}

	//Create the tracer line that comes out of the muzzle all the way to impact point
	if (TracerEffect)
	{
		FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
		UParticleSystemComponent* TracerComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect, MuzzleLocation);
		if (TracerComp)
		{
			TracerComp->SetVectorParameter(TracerTargetName, TracerEndPoint);
		}
	}
}

void ASRifle::OnRep_HitScanTrace()
{
	// Play cosmetic FX on the clients when replication occurs
	PlayFireEffects(HitScanTrace.TraceTo);
}

void ASRifle::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Replicated to any relevant client that's connected
	DOREPLIFETIME_CONDITION(ASRifle, HitScanTrace, COND_SkipOwner); //Owner doesn't need this replicated to it
}

