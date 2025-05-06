// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/EscapeEnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/Combat/EnemyCombatComponent.h"
#include "DataAssets/StartUpData/DataAsset_EnemyStartUpData.h"
#include "Engine/AssetManager.h"
#include "Components/UI/EnemyUIComponent.h"
#include "Components/WidgetComponent.h"
#include "Widgets/EscapeWidgetBase.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "EscapeDebugHelper.h"

AEscapeEnemyCharacter::AEscapeEnemyCharacter()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 180.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 1000.f;

	EnemyCombatComponent = CreateDefaultSubobject<UEnemyCombatComponent>("EnemyCombatComponent");
	EnemyUIComponent = CreateDefaultSubobject<UEnemyUIComponent>("EnemyUIComponent");
	EnemyHealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("EnemyHealthWidgetComponent");
	EnemyHealthWidgetComponent->SetupAttachment(GetMesh());
}

UPawnCombatComponent* AEscapeEnemyCharacter::GetPawnCombatComponent() const
{
	return EnemyCombatComponent;
}

UPawnUIComponent* AEscapeEnemyCharacter::GetPawnUIComponent() const
{
	return EnemyUIComponent;
}

UEnemyUIComponent* AEscapeEnemyCharacter::GetEnemyUIComponent() const
{
	return EnemyUIComponent;
}

void AEscapeEnemyCharacter::Multicast_EnemySendGameplayEventToActor_Implementation(AActor* InActor, FGameplayTag TagToSend, FGameplayEventData InEventData)
{
	if (InActor && TagToSend.IsValid())
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(InActor, TagToSend, InEventData);
		
	}
}

void AEscapeEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if(UEscapeWidgetBase* EnemyHealthWidget = Cast<UEscapeWidgetBase>(EnemyHealthWidgetComponent->GetUserWidgetObject()))
	{
		EnemyHealthWidget->InitEnemyCreatedWidget(this);
	}
}

void AEscapeEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitEnemyStartUpData();
}

void AEscapeEnemyCharacter::InitEnemyStartUpData()
{
	if (CharacterStartUpData.IsNull())
	{
		return;
	}

	UAssetManager::GetStreamableManager().RequestAsyncLoad(
		CharacterStartUpData.ToSoftObjectPath(), FStreamableDelegate::CreateLambda(
			[this]()
			{
				UDataAsset_StartUpDataBase* LoadedData = CharacterStartUpData.Get();
				if (LoadedData)
				{
					LoadedData->GiveToAbilitySystemComponent(EscapeAbilitySystemComponent);
				}
			}
		)
	);
}

void AEscapeEnemyCharacter::Server_ScalarParameterValueForMaterial_Implementation(FName ParameterName, float InValue)
{
	Multicast_ScalarParameterValueForMaterial(ParameterName, InValue);
}


void AEscapeEnemyCharacter::Multicast_ScalarParameterValueForMaterial_Implementation(FName ParameterName, float InValue)
{
	GetMesh()->SetScalarParameterValueOnMaterials(ParameterName, InValue);
}
