// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "JumpadBase.generated.h"

UCLASS()
class CONNIFABRIZIO_EXAM_API AJumpadBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJumpadBase();

	UFUNCTION(BlueprintCallable)
	void LaunchAction(AActor* Actor, FVector LaunchVelocity);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
