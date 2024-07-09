// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "BaseGate.h"
#include "GameFramework/Actor.h"
#include "LoadTriggerBox.generated.h"

UCLASS()
class CONNIFABRIZIO_EXAM_API ALoadTriggerBox : public AActor
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* BoxComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ABaseGate* Door;
public:	
	// Sets default values for this actor's properties
	ALoadTriggerBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void Load(AActor* Actor);
};
