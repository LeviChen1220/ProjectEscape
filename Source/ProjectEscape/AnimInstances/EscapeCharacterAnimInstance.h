// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/EscapeBaseAnimInstance.h"
#include "EscapeCharacterAnimInstance.generated.h"

class AEscapeBaseCharacter;
class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API UEscapeCharacterAnimInstance : public UEscapeBaseAnimInstance
{
	GENERATED_BODY()
	
public:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:

	UPROPERTY()
	AEscapeBaseCharacter* OwningCharacter;
	UPROPERTY()
	UCharacterMovementComponent* OwningMovementComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float GroundSpeed;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bHasAcceleration;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bIsFalling;
	UPROPERTY(BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float LocomotionDirection;

	UPROPERTY(BlueprintReadOnly, Category = "AimOffset")
	float AimOffsetYaw;
	UPROPERTY(BlueprintReadOnly, Category = "AimOffset")
	float AimOffsetPitch;

private:

};
