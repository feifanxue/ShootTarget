// Fill out your copyright notice in the Description page of Project Settings.


#include "Target.h"

#include <string>

#include "MyGameState.h"

// Sets default values
ATarget::ATarget()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATarget::BeginPlay()
{
	Super::BeginPlay();

}

void ATarget::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (OtherActor)
	{
		OtherActor->Destroy();

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Explosion, OtherActor->GetActorLocation());

		Cast<AMyGameState>(GetWorld()->GetGameState())->Score++;
	}
}

// Called every frame
void ATarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
