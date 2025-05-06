// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/Survivor/EscapeSurvivorLinkedAnimLayer.h"
#include "AnimInstances/Survivor/EscapeSurvivorAnimInstance.h"


UEscapeSurvivorAnimInstance* UEscapeSurvivorLinkedAnimLayer::GetSurvivorAnimInstance() const
{
    return Cast< UEscapeSurvivorAnimInstance>(GetOwningComponent()->GetAnimInstance());
}
