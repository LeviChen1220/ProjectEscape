// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/EscapeWidgetBase.h"
#include "Interfaces/PawnUIInterface.h"
#include "Components/UI/SurvivorUIComponent.h"

void UEscapeWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (IPawnUIInterface* PawnUIInterface = Cast<IPawnUIInterface>(GetOwningPlayerPawn()))
	{
		if (USurvivorUIComponent* SruvivorUIComponent = PawnUIInterface->GetSurvivorUIComponent())
		{
			BP_OnOwningSurvivorUIComponentInitialized(SruvivorUIComponent);
		}
	}
}

void UEscapeWidgetBase::InitEnemyCreatedWidget(AActor* OwningEnemyActor)
{
	if (IPawnUIInterface* PawnUIInterface = Cast<IPawnUIInterface>(OwningEnemyActor))
	{
		UEnemyUIComponent* EnemyUIComponent = PawnUIInterface->GetEnemyUIComponent();

		checkf(EnemyUIComponent, TEXT("EnemyUIComponent is return as nullptr"));
		
		BP_OnOwningEnemyIComponentInitialized(EnemyUIComponent);
	}
}

