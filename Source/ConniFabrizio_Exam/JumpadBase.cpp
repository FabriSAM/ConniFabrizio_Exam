// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpadBase.h"

// Sets default values
AJumpadBase::AJumpadBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AJumpadBase::LaunchAction(AActor* Actor, FVector LaunchVelocity)
{
	ACharacter* Character = Cast<ACharacter>(Actor);
	if (Character) 
	{
		Character->LaunchCharacter(LaunchVelocity, false, false);
	}
}

// Called when the game starts or when spawned
void AJumpadBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJumpadBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

