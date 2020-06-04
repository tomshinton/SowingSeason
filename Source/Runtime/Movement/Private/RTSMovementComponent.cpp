// Sowing Season - Tom Shinton 2020

#include "Runtime/Movement/Public/RTSMovementComponent.h"
#include "Runtime/Movement/Public/MovementSettings.h"

#include <Runtime/Engine/Classes/GameFramework/SpringArmComponent.h>
#include <Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>

DEFINE_LOG_CATEGORY_STATIC(RTSMovementComponentLog, Log, Log)

namespace RTSMovementPrivate
{
	const float MoveFrequency = 0.04f;
	const float StaticZHeight = 0.f;
}

namespace RTSMovementBindings
{
	const FName MoveForwardBinding = TEXT("MoveForwards");
	const FName MoveRightBinding = TEXT("MoveRight");
	const FName TurnBinding = TEXT("Turn");
	const FName MiddleMouseMoveBinding = TEXT("MiddleMouseMove");
	const FName ZoomInBinding = TEXT("ZoomIn");
	const FName ZoomOutBinding = TEXT("ZoomOut");
}

URTSMovementComponent::URTSMovementComponent()
	: Super()
	, MovementSettings(GetDefault<UMovementSettings>())
	, CameraArm(nullptr)
	, OwningPawn(nullptr)
	, OwningController(nullptr)
	, TargetYaw(0.f)
	, TargetArmLength(MovementSettings->MaxArmLength)
	, CurrMousePos(FVector2D::ZeroVector)
	, StoredMousePos(FVector2D::ZeroVector)
	, ViewportSize(FVector2D::ZeroVector)
	, UsingMiddleMouseMovement(false)
	, MiddleMouseMoveTimer()
	, EdgeBands(FEdgeBands())
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URTSMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningPawn = GetOwner();
	check(OwningPawn);

	if (APawn* OwnerAsPawn = Cast<APawn>(OwningPawn))
	{
		OwningController = Cast<APlayerController>(OwnerAsPawn->GetController());
		check(OwningController);
	}

	CameraArm = OwningPawn->FindComponentByClass<USpringArmComponent>();

	check(CameraArm);

	if (FViewport* Viewport = GEngine->GameViewport->Viewport)
	{
		SetViewportSize(Viewport->GetSizeXY());

		Viewport->ViewportResizedEvent.AddLambda([WeakThis = TWeakObjectPtr<URTSMovementComponent>(this)](FViewport* ResizedViewport, uint32)
		{
			if (WeakThis.IsValid())
			{
				WeakThis->SetViewportSize(ResizedViewport->GetSizeXY());
			}
		});
	}

	if (MovementSettings != nullptr)
	{
		const float MajorPadding = MovementSettings->EdgePadding_Major;
		const float BottomPadding = MovementSettings->EdgePadding_Bottom;

		EdgeBands.TopBand = FScreenEdge(0.f, 0.f, ViewportSize.X, ViewportSize.Y);
		EdgeBands.BottomBand = FScreenEdge(0.f, ViewportSize.Y - BottomPadding, ViewportSize.X, BottomPadding);
		EdgeBands.RightBand = FScreenEdge(ViewportSize.X - MajorPadding, 0, MajorPadding, ViewportSize.Y);
		EdgeBands.LeftBand = FScreenEdge(0.f, 0.f, MajorPadding, ViewportSize.Y);
	}

	InitialiseCameraView();
}

void URTSMovementComponent::SetViewportSize(const FVector2D InNewViewportSize)
{
	ViewportSize = InNewViewportSize;
}

void URTSMovementComponent::SetupComponentInputBindings(UInputComponent& PlayerInputComponent)
{
	Super::SetupComponentInputBindings(PlayerInputComponent);

	PlayerInputComponent.BindAxis(RTSMovementBindings::MoveForwardBinding, this, &URTSMovementComponent::MoveForwards);
	PlayerInputComponent.BindAxis(RTSMovementBindings::MoveRightBinding, this, &URTSMovementComponent::MoveRight);
	PlayerInputComponent.BindAxis(RTSMovementBindings::TurnBinding, this, &URTSMovementComponent::Turn);

	PlayerInputComponent.BindAction(RTSMovementBindings::MiddleMouseMoveBinding, IE_Pressed, this, &URTSMovementComponent::StartMiddleMouseMove);
	PlayerInputComponent.BindAction(RTSMovementBindings::MiddleMouseMoveBinding, IE_Released, this, &URTSMovementComponent::EndMiddleMouseMove);
	PlayerInputComponent.BindAction(RTSMovementBindings::ZoomInBinding, IE_Pressed, this, &URTSMovementComponent::ZoomIn);
	PlayerInputComponent.BindAction(RTSMovementBindings::ZoomOutBinding, IE_Pressed, this, &URTSMovementComponent::ZoomOut);
}

void URTSMovementComponent::InitialiseCameraView()
{
	if (CameraArm != nullptr)
	{
		CameraArm->TargetArmLength = TargetArmLength;
	}
}

const FEdgeBands* URTSMovementComponent::GetEdgeBands() const
{
	return &EdgeBands;
}

void URTSMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	TryEdgeMove(DeltaTime);
	RotateCamera(DeltaTime);
	BlendCameraZoom(DeltaTime);

	UpdateMouseLocation();
}

void URTSMovementComponent::TryEdgeMove(const float InDeltaTime)
{
	if (MovementSettings != nullptr)
	{
		const float MajorPadding = MovementSettings->EdgePadding_Major;
		const float BottomPadding = MovementSettings->EdgePadding_Bottom;
		const float MaxStrength = MovementSettings->MaxEdgeMoveStrength;

		EdgeBands.TopBand.SetStrength(MaxStrength * FMath::Clamp((1 - (CurrMousePos.Y / MajorPadding)), 0.f, 1.f));
		EdgeBands.BottomBand.SetStrength(MaxStrength * FMath::Clamp(((CurrMousePos.Y - (ViewportSize.Y - BottomPadding)) / BottomPadding), 0.f, 1.f));
		EdgeBands.RightBand.SetStrength(MaxStrength * FMath::Clamp(((CurrMousePos.X - (ViewportSize.X - MajorPadding)) / MajorPadding), 0.f, 1.f));
		EdgeBands.LeftBand.SetStrength(MaxStrength * FMath::Clamp(1 - (CurrMousePos.X / MajorPadding), 0.f, 1.f));

		if (!UsingMiddleMouseMovement)
		{
			MoveForwards(EdgeBands.TopBand.GetStrength());
			MoveForwards(-EdgeBands.BottomBand.GetStrength());
			MoveRight(EdgeBands.RightBand.GetStrength());
			MoveRight(-EdgeBands.LeftBand.GetStrength());
		}
	}
}

void URTSMovementComponent::RotateCamera(const float InRotateDelta)
{
	if (OwningPawn)
	{
		const float CurrYaw = GetOwner()->GetActorRotation().Yaw;
		const float NewYaw = UKismetMathLibrary::FInterpTo(CurrYaw, TargetYaw, InRotateDelta, 5);

		const FRotator CurrRotation = OwningPawn->GetActorRotation();
		const FRotator NewRotation = FRotator(CurrRotation.Pitch, NewYaw, CurrRotation.Roll);
		OwningPawn->SetActorRotation(NewRotation);
	}
}

void URTSMovementComponent::MiddleMouseButtonMove()
{
	if (MovementSettings != nullptr)
	{
		MoveRight(-(((StoredMousePos.X - CurrMousePos.X) / ViewportSize.X) * MovementSettings->MiddleMouseButtonMoveStrength));
		MoveForwards(((StoredMousePos.Y - CurrMousePos.Y) / ViewportSize.Y) * MovementSettings->MiddleMouseButtonMoveStrength);
	}
}

void URTSMovementComponent::MoveForwards(const float InAxis)
{
	if (OwningPawn != nullptr && MovementSettings != nullptr)
	{
		const FVector2D NewXY(OwningPawn->GetActorLocation() + (OwningPawn->GetActorForwardVector() * (MovementSettings->MoveSpeed * InAxis)));
		OwningPawn->SetActorLocation(FVector(NewXY, RTSMovementPrivate::StaticZHeight));
	}
}

void URTSMovementComponent::MoveRight(const float InAxis)
{
	if (OwningPawn && MovementSettings != nullptr)
	{
		const FVector2D NewXY(OwningPawn->GetActorLocation() + (OwningPawn->GetActorRightVector() * (MovementSettings->MoveSpeed * InAxis)));
		OwningPawn->SetActorLocation(FVector(NewXY, RTSMovementPrivate::StaticZHeight));
	}
}

void URTSMovementComponent::Turn(const float InAxis)
{
	if (OwningPawn != nullptr && MovementSettings != nullptr)
	{
		TargetYaw = OwningPawn->GetActorRotation().Yaw + MovementSettings->RotateSpeed*InAxis;
	}
}

void URTSMovementComponent::StartMiddleMouseMove()
{
	if (UWorld* World = GetWorld())
	{
		StoredMousePos = CurrMousePos;
		UsingMiddleMouseMovement = true;
		World->GetTimerManager().SetTimer(MiddleMouseMoveTimer, this, &URTSMovementComponent::MiddleMouseButtonMove, RTSMovementPrivate::MoveFrequency, true);
	}
}

void URTSMovementComponent::EndMiddleMouseMove()
{
	UsingMiddleMouseMovement = false;
	GetWorld()->GetTimerManager().ClearTimer(MiddleMouseMoveTimer);
}

void URTSMovementComponent::UpdateMouseLocation()
{
	float MouseX;
	float MouseY;
	OwningController->GetMousePosition(MouseX, MouseY);

	CurrMousePos.X = MouseX;
	CurrMousePos.Y = MouseY;
}

void URTSMovementComponent::ZoomIn()
{
	if (MovementSettings != nullptr)
	{
		TargetArmLength = FMath::Clamp<float>(TargetArmLength - MovementSettings->ArmZoomRate, MovementSettings->MinArmLength, MovementSettings->MaxArmLength);
	}
}

void URTSMovementComponent::ZoomOut()
{
	if (MovementSettings != nullptr)
	{
		TargetArmLength = FMath::Clamp<float>(TargetArmLength + MovementSettings->ArmZoomRate, MovementSettings->MinArmLength, MovementSettings->MaxArmLength);
	}
}

void URTSMovementComponent::BlendCameraZoom(const float InDeltaTime)
{
	if (CameraArm != nullptr && MovementSettings != nullptr)
	{	
		const float CurrentLength = CameraArm->TargetArmLength;
		const float NewArmLength = FMath::FInterpTo(CurrentLength, TargetArmLength, RTSMovementPrivate::MoveFrequency, MovementSettings->CameraZoomSpeed * InDeltaTime);
		CameraArm->TargetArmLength = NewArmLength;
	}
}
