// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveTriggerBox.h"
#include "BaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "SaveGameCustom.h"

// Sets default values
ASaveTriggerBox::ASaveTriggerBox()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
}


// Called when the game starts or when spawned
void ASaveTriggerBox::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASaveTriggerBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASaveTriggerBox::Save(AActor* Actor)
{
	ABaseCharacter* Character = Cast<ABaseCharacter>(Actor);
	if (!Character)
	{
		return;
	}
	
	USaveGame* SaveGame = UGameplayStatics::CreateSaveGameObject(USaveGameCustom::StaticClass());
	if (SaveGame)
	{
		USaveGameCustom* MyCustomSaveGame = Cast<USaveGameCustom>(SaveGame);
		if (MyCustomSaveGame)
		{
			MyCustomSaveGame->PlayerTransform = Character->GetActorTransform();
			if (Door)
			{
				MyCustomSaveGame->bPortIsOpen = Door->bIsActive;
			}

			UGameplayStatics::SaveGameToSlot(MyCustomSaveGame, "ConniFabrizioSaving", 0);
		}
	}
}

