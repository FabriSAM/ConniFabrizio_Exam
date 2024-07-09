// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "Components/SceneCaptureComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "BasePortal.generated.h"

UCLASS()
class CONNIFABRIZIO_EXAM_API ABasePortal : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USceneComponent* root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* PortalPlane;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material", meta = (AllowPrivateAccess = "true"))
	UMaterial* PortalMaterial;

	bool bIsInFront;
	bool bIsLastInFront;
	bool bIsCrossing;
	bool bIsIntersect;
	FVector LastPosition;

	UArrowComponent* Arrow;
	ABasePortal* TargetPortal;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	ABasePortal* LinkedPortal;

	FVector2D OldViewport;
public:	
	// Sets default values for this actor's properties
	ABasePortal();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	UTextureRenderTarget2D* CreateRenderTexture(FVector2D Viewport, UArrowComponent* InArrow);

	UFUNCTION(BlueprintCallable)
	void SetClipPlanes(USceneCaptureComponent2D* PortalCamera);

	UFUNCTION(BlueprintCallable)
	void UpdateSceneCapture(ABasePortal* Target, USceneCaptureComponent2D* PortalCamera);

	FVector InversionRotation(FVector Direction, FTransform ActorTransform, FTransform TargetTransform);

	FRotator RotationCorrection(FRotator Rotator, FTransform Transform, FTransform TargetTransform);

	UFUNCTION(BlueprintCallable)
	void ResizeSceneCapture(UTextureRenderTarget2D* RenderTarget, FVector2D Viewport);
	
	UFUNCTION(BlueprintCallable)
	void PortalTeleport(UBoxComponent* BoxCollision, FVector TragetLocation);

	ABaseCharacter* CanTeleport(UBoxComponent* BoxCollision);

	bool IsPointCrossingPlatform(FVector Point, FVector PortalLocation, FVector PortalNormal);

	void TeleportPlayer();
	void UpdateVelocity();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
