// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyController.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTING_API AEnemyController : public AAIController
{
	GENERATED_BODY()
	
public:
	AEnemyController();
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(BlueprintReadWrite)
	class UBlackboardComponent* BlackboardComponent;

	UPROPERTY(BlueprintReadWrite)
	class UBehaviorTreeComponent* BehaviorTreeComponent;

private:

public:
	FORCEINLINE UBlackboardComponent* GetBlackboardComponent() const { return BlackboardComponent; }
};
