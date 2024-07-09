// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadTriggerBox.h"
#include "BaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGameCustom.h"

// Sets default values
ALoadTriggerBox::ALoadTriggerBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
}

// Called when the game starts or when spawned
void ALoadTriggerBox::BeginPlay()
{
	Super::BeginPlay();
	ALoadTriggerBox::Load(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

// Called every frame
void ALoadTriggerBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALoadTriggerBox::Load(AActor* Actor)
{
	ABaseCharacter* Character = Cast<ABaseCharacter>(Actor);
	if (!Character)
	{
		return;
	}

	USaveGameCustom* LoadGame = Cast<USaveGameCustom>(UGameplayStatics::LoadGameFromSlot("ConniFabrizioSaving", 0));
	if (LoadGame)
	{
		if (Character->GetActorLocation() == LoadGame->PlayerTransform.GetLocation())
		{
			return;
		}

		Character->SetActorTransform(LoadGame->PlayerTransform);
		Character->GetController()->SetControlRotation(LoadGame->PlayerTransform.GetRotation().Rotator());

		if (Door)
		{
			if (LoadGame->bPortIsOpen) 
			{
				Door->Destroy();
			}
		}
	}
}

