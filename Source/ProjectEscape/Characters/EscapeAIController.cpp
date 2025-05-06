// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/EscapeAIController.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EscapeFunctionLibrary.h"
#include "EscapeGameplayTags.h"

#include "EscapeDebugHelper.h"

AEscapeAIController::AEscapeAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent"))
{
	
	AISenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("EnemySenseConfig_Sight");
	AISenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;
	AISenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = false;
	AISenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = false;
	AISenseConfig_Sight->SightRadius = 1500.f;
	AISenseConfig_Sight->LoseSightRadius = 0.f;
	AISenseConfig_Sight->PeripheralVisionAngleDegrees = 360.f;

	EnemyPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("EnemyPerceptionComponent");
	EnemyPerceptionComponent->ConfigureSense(*AISenseConfig_Sight);
	EnemyPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());
	EnemyPerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &ThisClass::OnEnemyPerceptionUpdated);

	SetGenericTeamId(FGenericTeamId(1));
}

void AEscapeAIController::BeginPlay()
{
	Super::BeginPlay();

	if (UCrowdFollowingComponent* CrowdComponent = Cast< UCrowdFollowingComponent>(GetPathFollowingComponent()))
	{
		//Debug::Print(TEXT("CrowdComponent is valid"), FColor::Green);
		CrowdComponent->SetCrowdSimulationState(bEnableDetourCrowdAvoidance ? ECrowdSimulationState::Enabled : ECrowdSimulationState::Disabled);

		switch (DetourCrowdAvoidanceQuality)
		{
		case 1: 
			CrowdComponent->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Low);
			break;
		case 2:  
			CrowdComponent->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Medium);
			break;
		case 3:
			CrowdComponent->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Good);
			break;
		case 4:
			CrowdComponent->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::High);
			break;

		default:
			break;
		}

		CrowdComponent->SetAvoidanceGroup(1);
		CrowdComponent->SetGroupsToAvoid(1);
		CrowdComponent->SetCrowdCollisionQueryRange(CollisionQueryRange);
	}
}

ETeamAttitude::Type AEscapeAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const APawn* PawnToCheck = Cast<const APawn>(&Other);

	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(PawnToCheck->GetController());

	if (OtherTeamAgent && OtherTeamAgent->GetGenericTeamId() < GetGenericTeamId())
	{
		return ETeamAttitude::Hostile;
	}

	return ETeamAttitude::Friendly;
}

void AEscapeAIController::OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (UBlackboardComponent* BlackboardComponent = GetBlackboardComponent())
	{
		AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject(FName("TargetActor")));

		if (!TargetActor)
		{
			if (Stimulus.WasSuccessfullySensed() && Actor)
			{
				if (!UEscapeFunctionLibrary::NativeDoesActorHaveTag(Actor, EscapeGameplayTags::Shared_Status_Dead) && !UEscapeFunctionLibrary::NativeDoesActorHaveTag(GetPawn(), EscapeGameplayTags::Shared_Status_Dead))
				{
					if (TargetActorIsWithinDistance(Actor))
					{
						BlackboardComponent->SetValueAsObject(FName("TargetActor"), Actor);
					}
				}
			}
		}
		else
		{
			if (!TargetActorIsWithinDistance(TargetActor))
			{
				BlackboardComponent->SetValueAsObject(FName("TargetActor"), nullptr);
			}
		}
	}
}

bool AEscapeAIController::TargetActorIsWithinDistance(AActor* TargetActor)
{
	if (GetPawn() && TargetActor)
	{
		if (GetPawn()->GetDistanceTo(TargetActor) <= 1500.f)
		{
			return true;
		}
	}
	return false;
}
