// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SWORDER_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	

protected:
	UPROPERTY(EditDefaultsOnly, BluePrintReadOnly, Category = Default)
	float Speed;
	UPROPERTY(EditDefaultsOnly, BluePrintReadOnly, Category = Default)
	float Direction;
	UPROPERTY(EditDefaultsOnly, BluePrintReadOnly, Category = Default)
	FVector Velocity;

	UPROPERTY(EditDefaultsOnly, BluePrintReadOnly, Category = "Assets")
	UAnimSequenceBase* AttackAsset;

	void NativeUpdateAnimation(float DeltaSeconds) override;


public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AttackAnimation();
	virtual void AttackAnimation_Implementation();
};
