// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/EscapeSurvivorWeapon.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/EscapeSurvivorCharacter.h"
#include "AbilitySystem/EscapeAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/PlayerController.h"
#include "AbilitySystem/EscapeAttributeSet.h"

#include "EscapeGameplayTags.h"
#include "EscapeDebugHelper.h"

AEscapeSurvivorWeapon::AEscapeSurvivorWeapon()
{
    CachedOwner = nullptr;
    CachedAnimInstance = nullptr;
    CachedFireMontage = nullptr;
    CachedOwningPawnASC = nullptr;
    CachedOwnerPlayerController = nullptr;
    CachedFireSound = nullptr;
}

void AEscapeSurvivorWeapon::BeginPlay()
{
    Super::BeginPlay();

    CachedOwner = Cast<AEscapeSurvivorCharacter>(GetOwner());
    if (CachedOwner)
    {
        CachedOwningPawnASC = CachedOwner->GetEscapeAbilitySystemComponent();
        CachedAnimInstance = CachedOwner->GetMesh()->GetAnimInstance();
        CachedOwnerPlayerController = Cast<APlayerController>(CachedOwner->GetController());
        SetEquippedWeaponData(SurvivorWeaponData);
    }
}

void AEscapeSurvivorWeapon::ApplyRangedDamage(AActor* TargetActor)
{
    if (TargetActor)
    {
        if (CachedOwningPawnASC)
        {
            FGameplayEffectContextHandle EffectContext = CachedOwningPawnASC->MakeEffectContext();
            EffectContext.AddSourceObject(CachedOwningPawnASC);
            EffectContext.AddInstigator(CachedOwner, CachedOwner);

            FGameplayEffectSpecHandle EffectSpecHandle = CachedOwningPawnASC->MakeOutgoingSpec(DamageEffect, 1, EffectContext);
            EffectSpecHandle.Data->SetSetByCallerMagnitude(EscapeGameplayTags::Shared_SetByCaller_BaseDamage, SurvivorWeaponData.WeaponBaseDamage.GetValue());

            if (TargetActor->IsA(AEscapeBaseCharacter::StaticClass()))
            {
                UAbilitySystemComponent* TargetPawnASC = Cast<AEscapeBaseCharacter>(TargetActor)->GetEscapeAbilitySystemComponent();
                if (TargetPawnASC)
                {
                    CachedOwningPawnASC->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data, TargetPawnASC);


                    if (GameplayCueTagHitImpcat.IsValid())
                    {
                        CachedOwningPawnASC->ExecuteGameplayCue(GameplayCueTagHitImpcat);
                    }
                  
                    FGameplayEventData Data;
                    Data.Instigator = GetOwner();
                    Data.Target = TargetActor;
                    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(TargetActor, EscapeGameplayTags::Shared_Event_HitReact, Data);

                    //UAbilitySystemBlueprintLibrary::
                }
            }
        }
    }
}

void AEscapeSurvivorWeapon::AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InSpecHandles)
{
	GrantedAbilitySpecHandles = InSpecHandles;
}

TArray<FGameplayAbilitySpecHandle> AEscapeSurvivorWeapon::GetGrantedAbilitySpecHandles() const
{
	return GrantedAbilitySpecHandles;
}

FVector AEscapeSurvivorWeapon::GetMuzzleLocation() const
{
	if (WeaponSKMesh)
	{
		return WeaponSKMesh->GetSocketLocation(TEXT("Muzzle"));
	}
	return FVector::ZeroVector;
}

void AEscapeSurvivorWeapon::FireWeapon()
{
    if (!GetOwner())
    {
        return;
    }

    FVector MuzzleLocation = GetMuzzleLocation(); 
    FRotator MuzzleRotation = WeaponSKMesh->GetSocketRotation(TEXT("Muzzle"));

    const float SpreadAngleDegrees = 3.0f;
    const float SpreadAngleRadians = FMath::DegreesToRadians(SpreadAngleDegrees);

    FVector ShotDirection = FMath::VRandCone(MuzzleRotation.Vector(), SpreadAngleRadians);

    FVector EndTrace = MuzzleLocation + (ShotDirection * 500.0f);

    //FRotator AimRotation = CachedOwner->GetControlRotation();
    //FVector EndTrace = MuzzleLocation + (AimRotation.Vector() * 500.0f);

    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(GetOwner()); 
    QueryParams.bReturnPhysicalMaterial = true; 

    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult, MuzzleLocation, EndTrace, ECC_Pawn, QueryParams
    );

    if (CachedOwner->GetEscapeAttributeSet()->GetCurrentWeaponAmmunitionAmount() > 0.f)
    {
        FGameplayEffectContextHandle EffectContext = CachedOwningPawnASC->MakeEffectContext();
        EffectContext.AddSourceObject(CachedOwningPawnASC);
        EffectContext.AddInstigator(CachedOwner, CachedOwner);

        FGameplayEffectSpecHandle EffectSpecHandle = CachedOwningPawnASC->MakeOutgoingSpec(DamageEffect, 1, EffectContext);
        EffectSpecHandle.Data->SetSetByCallerMagnitude(EscapeGameplayTags::Player_SetByCaller_Weapon_Ammunition_Use, 1.f);

        CachedOwningPawnASC->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data, CachedOwningPawnASC);

        if (bHit)
        {
            AActor* HitActor = HitResult.GetActor();
            if (HitActor && HitActor != GetOwner())
            {

                // OnWeaponHitTarget.ExecuteIfBound(HitActor);
                ApplyRangedDamage(HitActor);

                // Play impact effect
                //PlayImpactEffect(HitResult);
            }
        }

        MulticastPlayFireWeaponMontage();

        //Camera shake effect
        if (CachedOwnerPlayerController && CameraShake)
        {
            CachedOwnerPlayerController->ClientStartCameraShake(CameraShake);
        }

        CachedOwningPawnASC->ExecuteGameplayCue(GameplayCueTagFire);
        // Play muzzle flash effect
        //PlayMuzzleEffect();

#if WITH_EDITOR
        MulticastDrawWeaponTrace(MuzzleLocation, EndTrace, bHit);
#endif

    }
}

void AEscapeSurvivorWeapon::SetEquippedWeaponData(const FEscapeSurvivorWeaponData& NewData)
{
    for (const FEscapeSurvivorWeaponCustomData& WeaponData : SurvivorWeaponData.DefaultWeaponCustomData)
    {
        if (WeaponData.WeaponFireMontage)
        {
            CachedFireMontage = WeaponData.WeaponFireMontage;
            CachedFireSound = WeaponData.FireSound;
            break;
        }
    }
}

void AEscapeSurvivorWeapon::MulticastPlayFireWeaponMontage_Implementation()
{
    if (!CachedAnimInstance || !CachedFireMontage)
    {
        return;
    }

    CachedAnimInstance->Montage_Play(CachedFireMontage);
}

void AEscapeSurvivorWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEscapeSurvivorWeapon, SurvivorWeaponData);
}


void AEscapeSurvivorWeapon::MulticastDrawWeaponTrace_Implementation(FVector start, FVector end, bool isHit)
{
    DrawDebugLine(GetWorld(), start, end, isHit ? FColor::Red : FColor::Blue, false, 2.0f, 0, 1.0f);

}
