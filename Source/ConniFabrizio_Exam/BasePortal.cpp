// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePortal.h"
#include "UnrealClient.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Blueprint/WidgetLayoutLibrary.h"


// Sets default values
ABasePortal::ABasePortal()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	PortalPlane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Portal Plane"));
	PortalPlane->SetupAttachment(root);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> GetMeshFromAssets(TEXT("/Script/Engine.StaticMesh'/Engine/EditorMeshes/EditorPlane.EditorPlane'"));
	PortalPlane->SetStaticMesh(GetMeshFromAssets.Object);

}

// Called when the game starts or when spawned
void ABasePortal::BeginPlay()
{
	Super::BeginPlay();
	SetTickGroup(TG_PostUpdateWork);
}

UTextureRenderTarget2D* ABasePortal::CreateRenderTexture(FVector2D Viewport, UArrowComponent* InArrow)
{
	UTextureRenderTarget2D* RetunrValue = nullptr;

	UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(PortalMaterial, PortalPlane);
	if (DynamicMaterial)
	{
		UE_LOG(LogTemp, Warning, TEXT("Material Created"));
		PortalPlane->SetMaterial(0, DynamicMaterial);

		RetunrValue = UKismetRenderingLibrary::CreateRenderTarget2D(this, static_cast<int32>(Viewport.X), static_cast<int32>(Viewport.Y), RTF_RGBA16f, FLinearColor::Blue);
		OldViewport = Viewport;
		if (RetunrValue)
		{
			UE_LOG(LogTemp, Warning, TEXT("RenderTarget Created"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("RenderTarget Not Created"));
		}
		DynamicMaterial->SetTextureParameterValue(TEXT("Texture"), RetunrValue);

		FVector Forward = InArrow->GetForwardVector();
		Forward *= -6;
		FLinearColor CustoColor = UKismetMathLibrary::MakeColor(Forward.X, Forward.Y, Forward.Z, 1.0);
		
		DynamicMaterial->SetVectorParameterValue(TEXT("OffsetDistance"), CustoColor);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Material Not Created"));
	}
	Arrow = InArrow;
	return RetunrValue;
}

void ABasePortal::SetClipPlanes(USceneCaptureComponent2D* PortalCamera)
{
	PortalCamera->bEnableClipPlane = true;

	FVector PlaneLocation = PortalPlane->GetComponentLocation();
	FVector ForwardDirection = Arrow->GetForwardVector();
	ForwardDirection *= -3.0f;
	FVector ClipPlaneBaseVector = PlaneLocation + ForwardDirection;
	PortalCamera->ClipPlaneBase = ClipPlaneBaseVector;
	PortalCamera->ClipPlaneNormal = ForwardDirection;

}

void ABasePortal::UpdateSceneCapture(ABasePortal* Target, USceneCaptureComponent2D* PortalCamera)
{
	TargetPortal = Target;
	FTransform Transform = this->GetActorTransform();
	Transform.SetScale3D(FVector(Transform.GetScale3D().X * -1, Transform.GetScale3D().Y * -1, Transform.GetScale3D().Z));

	APlayerCameraManager* Camera = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	FVector CameraLocation = Camera->GetTransformComponent()->GetComponentLocation();
	FVector InverseTransform = UKismetMathLibrary::InverseTransformLocation(Transform, CameraLocation);

	FTransform TargetTransform = Target->GetActorTransform();
	FVector TargetCameraLocation = UKismetMathLibrary::TransformLocation(TargetTransform, InverseTransform);
	FRotator CameraRotation = Camera->GetTransformComponent()->GetComponentRotation();
	FRotator TargetCameraRotation = RotationCorrection(CameraRotation, Transform, TargetTransform);

	PortalCamera->SetWorldLocationAndRotation(TargetCameraLocation, TargetCameraRotation);
}

FVector ABasePortal::InversionRotation(FVector Direction, FTransform ActorTransform, FTransform TargetTransform)
{
	FVector Inversion = UKismetMathLibrary::InverseTransformDirection(ActorTransform, Direction);
	FVector MirrorX = UKismetMathLibrary::MirrorVectorByNormal(Inversion, FVector(1, 0, 0));
	FVector MirrorY = UKismetMathLibrary::MirrorVectorByNormal(MirrorX, FVector(0, 1, 0));
	FVector FinalDirection = UKismetMathLibrary::TransformDirection(TargetTransform, MirrorY);

	return FinalDirection;
}

FRotator ABasePortal::RotationCorrection(FRotator Rotator, FTransform Transform, FTransform TargetTransform)
{	
	FVector X;
	FVector Y;
	FVector Z;
	UKismetMathLibrary::BreakRotIntoAxes(Rotator, X, Y, Z);
	FRotator TargetCameraRotation = UKismetMathLibrary::MakeRotationFromAxes(
		InversionRotation(X, Transform, TargetTransform),
		InversionRotation(Y, Transform, TargetTransform),
		InversionRotation(Z, Transform, TargetTransform));
	return TargetCameraRotation;
}

void ABasePortal::ResizeSceneCapture(UTextureRenderTarget2D* RenderTarget, FVector2D Viewport)
{
	if (OldViewport.X == static_cast<int32>(Viewport.X) && OldViewport.Y == static_cast<int32>(Viewport.Y))
	{
		return;
	}

	RenderTarget->ResizeTarget(static_cast<int32>(Viewport.X), static_cast<int32>(Viewport.Y));
	OldViewport = Viewport;
}

void ABasePortal::PortalTeleport(UBoxComponent* BoxCollision, FVector TragetLocation)
{
	ABaseCharacter* Character = CanTeleport(BoxCollision);
	if (Character)
	{
		UCameraComponent* CharCamera = Character->GetComponentByClass<UCameraComponent>();
		if (CharCamera)
		{
			//FVector Point = CharCamera->GetComponentLocation();
			FVector Point = Character->GetActorLocation();

			FVector PortalLocation = GetActorLocation();
			FVector PortalNormal = Arrow->GetForwardVector();
			PortalNormal.Y = 0;
			PortalNormal.Z = 0;
			bool result = IsPointCrossingPlatform(Point, PortalLocation, PortalNormal);
			if (result)
			{
				TeleportPlayer();
				UpdateVelocity();
				UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->SetGameCameraCutThisFrame();
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Camera Not Found"));
		}
	}
}

ABaseCharacter* ABasePortal::CanTeleport(UBoxComponent* BoxCollision)
{
	TArray<AActor*> OverlapActor;
	BoxCollision->GetOverlappingActors(OverlapActor, ABaseCharacter::StaticClass());
	if (OverlapActor.Num() > 0)
	{
		return Cast<ABaseCharacter>(OverlapActor[0]);
	}
	return nullptr;
}

bool ABasePortal::IsPointCrossingPlatform(FVector Point, FVector PortalLocation, FVector PortalNormal)
{
	FVector ResultPoint = Point - PortalLocation;
	double DotResult = PortalNormal.Dot(ResultPoint);
	bIsInFront = (DotResult >= 0);
	
	FPlane Plane = UKismetMathLibrary::MakePlaneFromPointAndNormal(PortalLocation, PortalNormal);
	float T = 0;
	FVector Intercept = FVector::ZeroVector;
	bIsIntersect = UKismetMathLibrary::LinePlaneIntersection(LastPosition, Point, Plane, T, Intercept);

	bIsCrossing = (bIsIntersect && !bIsInFront && bIsLastInFront);
	bIsLastInFront = bIsInFront;
	LastPosition = Point;

	return bIsCrossing;
}

void ABasePortal::TeleportPlayer()
{
	FTransform Transform = this->GetActorTransform();
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	
	Transform.SetScale3D(FVector(Transform.GetScale3D().X * -1, Transform.GetScale3D().Y * -1, Transform.GetScale3D().Z));

	FVector PlayerLocation = Player->GetActorLocation();

	FVector InverseTransform = UKismetMathLibrary::InverseTransformLocation(Transform, PlayerLocation);
	FTransform LinkedPortalTransform = TargetPortal->GetActorTransform();
	FVector PlayerFinalLocation = UKismetMathLibrary::TransformLocation(LinkedPortalTransform, InverseTransform);

	FRotator PlayerRotation = Player->GetActorRotation();
	FRotator InverseRotation = RotationCorrection(PlayerRotation, Transform, LinkedPortalTransform);
	Player->SetActorLocationAndRotation(PlayerFinalLocation + Arrow->GetForwardVector()*40, InverseRotation);

	FRotator PlayerControllerRotator = PlayerController->GetControlRotation();
	FRotator CorrectionRotation = RotationCorrection(PlayerControllerRotator, Transform, LinkedPortalTransform);

	PlayerController->SetControlRotation(CorrectionRotation);
}

void ABasePortal::UpdateVelocity()
{
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	UPawnMovementComponent* PlayerMovementComponent = Player->GetMovementComponent();	
	
	FTransform Transform = this->GetActorTransform();
	FTransform LinkedPortalTransform = TargetPortal->GetActorTransform();
	FVector Velocity = PlayerMovementComponent->Velocity;
	Velocity.Normalize(0.0001);	
	FVector InverseDirection = InversionRotation(Velocity, Transform, LinkedPortalTransform);
	
	float VelocityLenght = PlayerMovementComponent->Velocity.Length();
	InverseDirection *= VelocityLenght;
	PlayerMovementComponent->Velocity = InverseDirection;
}

// Called every frame
void ABasePortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

