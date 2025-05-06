// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/EscapeCharacterAnimInstance.h"
//Character
#include"Characters/EscapeBaseCharacter.h"
//Components
#include "GameFramework/CharacterMovementComponent.h"
//Kismet
#include "Kismet/KismetMathLibrary.h"
#include "KismetAnimationLibrary.h"
//Debug Helper
#include "EscapeDebugHelper.h"


void UEscapeCharacterAnimInstance::NativeInitializeAnimation()
{
	OwningCharacter = Cast<AEscapeBaseCharacter>(TryGetPawnOwner());
	if (OwningCharacter)
	{
		OwningMovementComponent = OwningCharacter->GetCharacterMovement();
	}
}

void UEscapeCharacterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	if (!OwningCharacter ||!OwningMovementComponent)
	{
		return;
	}

	GroundSpeed = OwningCharacter->GetVelocity().Size2D();
	bHasAcceleration = OwningMovementComponent->GetCurrentAcceleration().SizeSquared2D() > 0.f;
	bIsFalling = OwningMovementComponent->IsFalling();

	LocomotionDirection = UKismetAnimationLibrary::CalculateDirection(OwningCharacter->GetVelocity(), OwningCharacter->GetActorRotation());

	OwningCharacter->UpdateAimOffset(DeltaSeconds);

	AimOffsetYaw = FMath::FInterpTo(AimOffsetYaw, OwningCharacter->GetAimOffsetYaw(), DeltaSeconds, 6.f);
	AimOffsetPitch = FMath::FInterpTo(AimOffsetPitch, OwningCharacter->GetAimOffsetPitch(), DeltaSeconds, 6.f);

	//Debug::Print(OwningCharacter->GetActorNameOrLabel() + FString::Printf(TEXT("AimOffsetYaw: %f Network Role: %s"), AimOffsetYaw, OwningCharacter->HasAuthority()? TEXT("Server") : TEXT("Client")));

	/*const FRotator ControlRotation = OwningCharacter->GetControlRotation();
	const FRotator ActorRotation = OwningCharacter->GetActorRotation();
	const FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(ControlRotation, ActorRotation);

	if (FMath::Abs(DeltaRot.Yaw) <= 160.f)
	{
		AimOffsetYaw = FMath::FInterpTo(AimOffsetYaw, DeltaRot.Yaw, DeltaSeconds, 6.f);
		AimOffsetYaw = FMath::Clamp(AimOffsetYaw, -160.f, 160.f);
	}
	else
	{
		AimOffsetYaw = FMath::FInterpTo(AimOffsetYaw, 0.f, DeltaSeconds, 6.f);
	}
	
	AimOffsetPitch = FMath::FInterpTo(AimOffsetPitch, DeltaRot.Pitch, DeltaSeconds, 6.f);
	AimOffsetPitch = FMath::Clamp(AimOffsetPitch, -90.f, 90.f);*/
}
