// Fill out your copyright notice in the Description page of Project Settings.


#include "EscapeFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/EscapeAbilitySystemComponent.h"
#include "Characters/EscapeBaseCharacter.h"
#include "AbilitySystem/EscapeAttributeSet.h"
#include "Interfaces/PawnCombatInterface.h"
#include "GenericTeamAgentInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "EscapeGameplayTags.h"

#include "EscapeDebugHelper.h"

UEscapeAbilitySystemComponent* UEscapeFunctionLibrary::NativeGetEscapeASCFromActor(AActor* InActor)
{
	checkf(InActor, TEXT("Invalid actor reference in EFL_GetEscapeASCFromActor"));

	return CastChecked<UEscapeAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
}

void UEscapeFunctionLibrary::AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd)
{
	checkf(InActor, TEXT("Invalid actor reference in EFL_AddGameplayTagToActorIfNone"));

	UEscapeAbilitySystemComponent*ASC = NativeGetEscapeASCFromActor(InActor);

	if (!ASC->HasMatchingGameplayTag(TagToAdd))
	{
		ASC->Server_AddGameplayTagIfNone(TagToAdd);
	}
}

void UEscapeFunctionLibrary::RemoveGameplayTagToActorIfFound(AActor* InActor, FGameplayTag TagToRemove)
{
	checkf(InActor, TEXT("Invalid actor reference in EFL_RemoveGameplayTagToActorIfFound"));

	UEscapeAbilitySystemComponent* ASC = NativeGetEscapeASCFromActor(InActor);

	if (ASC->HasMatchingGameplayTag(TagToRemove))
	{
		ASC->Server_RemoveGameplayTagIfFound(TagToRemove);
	}
}

void UEscapeFunctionLibrary::BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, EEscapeConfirmType& OutConfirmType)
{
	OutConfirmType = NativeDoesActorHaveTag(InActor, TagToCheck) ? EEscapeConfirmType::Yes : EEscapeConfirmType::No;
}

UPawnCombatComponent* UEscapeFunctionLibrary::NativeGetPawnCombatComponentFromActor(AActor* InActor)
{
	check(InActor);

	if (IPawnCombatInterface* PawnCombatInterface =Cast<IPawnCombatInterface>(InActor))
	{
		return PawnCombatInterface->GetPawnCombatComponent();
	}

	return nullptr;
}

UPawnCombatComponent* UEscapeFunctionLibrary::BP_GetPawnCombatComponentFromActor(AActor* InActor, EEscapeValidType& OutValidType)
{
	UPawnCombatComponent* CombatComponent = NativeGetPawnCombatComponentFromActor(InActor);
	OutValidType = CombatComponent ? EEscapeValidType::Valid : EEscapeValidType::Invalid;

	return CombatComponent;
}

bool UEscapeFunctionLibrary::IsTargetPawnHostile(APawn* QueryPawn, APawn* TargetPawn)
{
	check(QueryPawn && TargetPawn);

	IGenericTeamAgentInterface* QueryTeamAgent = Cast<IGenericTeamAgentInterface>(QueryPawn->GetController());
	IGenericTeamAgentInterface* TargetTeamAgent = Cast<IGenericTeamAgentInterface>(TargetPawn->GetController());

	if (QueryTeamAgent && TargetTeamAgent)
	{
		return QueryTeamAgent->GetGenericTeamId() != TargetTeamAgent->GetGenericTeamId();
	}

	return false;
}

FGameplayTag UEscapeFunctionLibrary::ComputeHitReactDirectionTag(AActor* InAttacker, AActor* InVictim, float& OutAngleDifference)
{
	if (InAttacker && InVictim)
	{
		const FVector VictomForward = InVictim->GetActorForwardVector();
		const FVector VictomToAttackerNormalized = (InAttacker->GetActorLocation() - InVictim->GetActorLocation()).GetSafeNormal();

		const float DotResult = FVector::DotProduct(VictomForward, VictomToAttackerNormalized);
		OutAngleDifference = UKismetMathLibrary::DegAcos(DotResult);

		const FVector CrossResult = FVector::CrossProduct(VictomForward, VictomToAttackerNormalized);

		if (CrossResult.Z < 0.f)
		{
			OutAngleDifference *= -1.f;
		}

		if (OutAngleDifference >= -45.f && OutAngleDifference <= 45.f)
		{
			return EscapeGameplayTags::Shared_Status_HitReact_Front;
		}
		else if (OutAngleDifference < -45.f && OutAngleDifference >= -135.f)
		{
			return EscapeGameplayTags::Shared_Status_HitReact_Left;
		}
		else if (OutAngleDifference < -135.f && OutAngleDifference > 135.f)
		{
			return EscapeGameplayTags::Shared_Status_HitReact_Back;
		}
		else if (OutAngleDifference > 45.f && OutAngleDifference <= 135.f)
		{
			return EscapeGameplayTags::Shared_Status_HitReact_Right;
		}

		return EscapeGameplayTags::Shared_Status_HitReact_Front;
	}
	else
	{
		return EscapeGameplayTags::Shared_Status_HitReact_Front;
	}

}

AEscapeBaseCharacter* UEscapeFunctionLibrary::NativeGetEscapeBaseCharacterFromActor(AActor* InActor)
{
	check(InActor);

	if (AEscapeBaseCharacter* EscapeBaseCharacter = Cast<AEscapeBaseCharacter>(InActor))
	{
		return EscapeBaseCharacter;
	}

	return nullptr;
}

AEscapeBaseCharacter* UEscapeFunctionLibrary::BP_GetEscapeBaseCharacterFromActor(AActor* InActor, EEscapeValidType& OutValidType)
{

	AEscapeBaseCharacter* EscapeBaseCharacter = NativeGetEscapeBaseCharacterFromActor(InActor);
	OutValidType = EscapeBaseCharacter ? EEscapeValidType::Valid : EEscapeValidType::Invalid;

	return EscapeBaseCharacter;
}

bool UEscapeFunctionLibrary::BP_CheckIsActorDead(AActor* InActor)
{
	AEscapeBaseCharacter* EscapeBaseCharacter = NativeGetEscapeBaseCharacterFromActor(InActor);
	if (EscapeBaseCharacter)
	{
		if (EscapeBaseCharacter->GetEscapeAttributeSet()->GetCurrentHealth() <= 0)
		{
			return true;
		}
	}
	return false;
}

bool UEscapeFunctionLibrary::NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck)
{
	checkf(InActor, TEXT("Invalid actor reference in EFL_NativeDoesActorHaveTag"));

	UEscapeAbilitySystemComponent* ASC = NativeGetEscapeASCFromActor(InActor);

	return ASC->HasMatchingGameplayTag(TagToCheck);
}
