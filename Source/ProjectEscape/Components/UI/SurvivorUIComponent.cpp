// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/UI/SurvivorUIComponent.h"
#include "Widgets/EscapeWidgetBase.h"
#include "EscapeDebugHelper.h"


USurvivorUIComponent::USurvivorUIComponent() :
	bFadeHUDAnimHasPlayed(false)
{

}

void USurvivorUIComponent::Client_PlayFadeHUDAnim_Implementation(bool bShouldPlayInReverse)
{
	if (!bFadeHUDAnimHasPlayed)
	{
		if (OwningWidget)
		{
			bFadeHUDAnimHasPlayed = true;

			OwningWidget->BP_PlayFadeAnim(bShouldPlayInReverse);
		}
		else
		{
			Debug::Print(TEXT("Missing OwningWidget"));
		}
	}
}
