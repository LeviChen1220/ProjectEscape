// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GEExecCalc/GEExecCalc_DamageTaken.h"
#include "AbilitySystem/EscapeAttributeSet.h"
#include "EscapeGameplayTags.h"
#include "EscapeDebugHelper.h"

struct FEscapeDamageCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageTaken)
	DECLARE_ATTRIBUTE_CAPTUREDEF(TotalAmmunitionConsumed)

	FEscapeDamageCapture()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UEscapeAttributeSet, AttackPower, Source, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UEscapeAttributeSet, DefensePower, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UEscapeAttributeSet, DamageTaken, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UEscapeAttributeSet, TotalAmmunitionConsumed, Source, false)
	}
};

static const FEscapeDamageCapture& GetEscapeDamageCapture()
{
	static FEscapeDamageCapture EscapeDamageCapture;
	return EscapeDamageCapture;
}
UGEExecCalc_DamageTaken::UGEExecCalc_DamageTaken()
{
	RelevantAttributesToCapture.Add(GetEscapeDamageCapture().AttackPowerDef);
	RelevantAttributesToCapture.Add(GetEscapeDamageCapture().DefensePowerDef);
	RelevantAttributesToCapture.Add(GetEscapeDamageCapture().DamageTakenDef);
	RelevantAttributesToCapture.Add(GetEscapeDamageCapture().TotalAmmunitionConsumedDef);
}

void UGEExecCalc_DamageTaken::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();

	/*EffectSpec.GetContext().GetSourceObject();
	EffectSpec.GetContext().GetAbility();
	EffectSpec.GetContext().GetInstigator();
	EffectSpec.GetContext().GetEffectCauser();*/

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();

	float SourceAttackPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetEscapeDamageCapture().AttackPowerDef, EvaluateParameters, SourceAttackPower);
	
	float BaseDamage = 0.f; //weapon
	float AmmunitionUsed = 0.f;

	for (const TPair<FGameplayTag, float>& TagMagnitude : EffectSpec.SetByCallerTagMagnitudes)
	{
		if (TagMagnitude.Key.MatchesTagExact(EscapeGameplayTags::Shared_SetByCaller_BaseDamage))
		{
			int AdditionalDamage = FMath::FRandRange(0.0f, 10.0f);
			BaseDamage = TagMagnitude.Value + AdditionalDamage;
			//Debug::Print(TEXT("BaseDamage"), BaseDamage);
		}

		if (TagMagnitude.Key.MatchesTagExact(EscapeGameplayTags::Player_SetByCaller_Weapon_Ammunition_Use))
		{
			AmmunitionUsed = TagMagnitude.Value;

			OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetEscapeDamageCapture().TotalAmmunitionConsumedProperty,
				EGameplayModOp::Override,
				AmmunitionUsed));
		}

		//ComboCount logic should be implement here as well.
	}
	
	float TargetDefencePower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetEscapeDamageCapture().DefensePowerDef, EvaluateParameters, TargetDefencePower);
	//Debug::Print(TEXT("TargetDefencePower"), TargetDefencePower);

	const float FinalDamageDone = BaseDamage * (1.0f / (1.0f + TargetDefencePower));
	Debug::Print(TEXT("FinalDamageDone"), FinalDamageDone);

	if (FinalDamageDone > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetEscapeDamageCapture().DamageTakenProperty,
			EGameplayModOp::Override,
			FinalDamageDone));
	}
}
