// Fill out your copyright notice in the Description page of Project Settings.


#include "RailGun.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "ShootTarget/ShootTarget.h"
#include "TimerManager.h"

#include "IMessageTracer.h"

// Sets default values
ARailGun::ARailGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	MuzzleSocketName = "MuzzleSocket";
	TracerTargetName = "Target";

	BaseDamage = 20.0f;

	RateOfFire = 600;

}

// Called when the game starts or when spawned
void ARailGun::BeginPlay()
{
	Super::BeginPlay();

	TimeBetweenShots = 60 / RateOfFire;
}

// Called every frame
void ARailGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// 按住左键开火
void ARailGun::StartFire()
{
	// 全自动开火模式
	float FirstDelay = FMath::Max(LastFireTime + TimeBetweenShots - GetWorld()->TimeSeconds, 0.0f);
	GetWorldTimerManager().SetTimer(TimerHandle_TimeBetweenShots, this, &ARailGun::Fire, TimeBetweenShots, true, FirstDelay);
}

// 松开左键停火
void ARailGun::StopFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_TimeBetweenShots);
}

// 开火后的各种逻辑
void ARailGun::Fire()
{
	AActor* MyOwner = GetOwner();
	if(MyOwner)
	{
		FVector EyeLocation;
		FRotator EyeRotation;
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector ShotDirection = EyeRotation.Vector();
		
		FVector TraceEnd = EyeLocation + (ShotDirection * 10000);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		QueryParams.AddIgnoredActor(MyOwner);
		QueryParams.bTraceComplex = true;
		QueryParams.bReturnPhysicalMaterial = true;

		FVector TracerEndPoint = TraceEnd;

		FHitResult Hit;
		if(GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, COLLISION_WEAPON, QueryParams))
		{
			// 子弹被阻挡
			AActor* HitActor = Hit.GetActor();
			
			EPhysicalSurface SurfaceType = UGameplayStatics::GetSurfaceType(Hit);

			float ActualDamage = BaseDamage;
			if(SurfaceType == SURFACE_FLESHVULNERABLE)
			{
				ActualDamage *= 4.0f;
			}

			UGameplayStatics::ApplyPointDamage(HitActor, ActualDamage, ShotDirection, Hit, MyOwner->GetInstigatorController(), this, DamageType);
			
			UParticleSystem* SelectEffect = nullptr;
			// 子弹打到不同表面材质时，显示不同的特效
			switch (SurfaceType)
			{
			case SURFACE_FLESHDEFAULT:
				SelectEffect = FleshImpactEffect;
				break;
			case SURFACE_FLESHVULNERABLE:
				SelectEffect = FleshImpactEffect;
				break;
			default:
				SelectEffect = ImpactEffect;
				break;
			}

			if(SelectEffect)
			{
				// 击中敌人的特效
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
			}
			
			TracerEndPoint = Hit.ImpactPoint;
		}
		
		PlayFireEffect(TracerEndPoint);

		LastFireTime = GetWorld()->TimeSeconds;
	}
}

// 开火特效
void ARailGun::PlayFireEffect(FVector TraceEnd)
{
	if(MuzzleEffect)
	{
		// 枪口火花特效
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName);
	}
		
	if(TracerEffect)
	{
		// 子弹路径特效
		FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
		UParticleSystemComponent* TracerComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect, MuzzleLocation);
		if(TracerComp)
		{
			TracerComp->SetVectorParameter(TracerTargetName, TraceEnd);
		}
	}
}

