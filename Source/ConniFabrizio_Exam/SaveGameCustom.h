// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveGameCustom.generated.h"

/**
 * 
 */
UCLASS()
class CONNIFABRIZIO_EXAM_API USaveGameCustom : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite)
	FTransform PlayerTransform;

	UPROPERTY(BlueprintReadWrite)
	bool bPortIsOpen;
};
