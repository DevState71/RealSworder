// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/PlayerAnimInstance.h"

#include "GameFramework/Character.h"

// For UE_Log
#include "../../Sworder.h"

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	APawn* owner = TryGetPawnOwner();
	if (owner) {
		Speed = owner->GetVelocity().Size();
		Direction = this->CalculateDirection(owner->GetVelocity(), owner->GetActorRotation());
		Velocity = owner->GetVelocity();

		
	}
}

void UPlayerAnimInstance::AttackAnimation_Implementation()
{
	if (AttackSequence) {
		UE_LOG(Game, Warning, TEXT("Attack Animation Called"));

		UAnimMontage* attack = nullptr;

		if (bIsFullBody) {
			attack = this->PlaySlotAnimationAsDynamicMontage(AttackSequence, FName("Full_Body"));
		}
		else
			attack = this->PlaySlotAnimationAsDynamicMontage(AttackSequence, FName("Attack"));
		this->Montage_SetEndDelegate(AttackEnded, attack);
	}
	else
		UE_LOG(Game, Error, TEXT("Attack animation called without the animation sequence asset!"));
}

void UPlayerAnimInstance::SetAttackAnimation(UAnimSequence* NewAttack, bool FullBody)
{
	AttackSequence = NewAttack;
	bIsFullBody = FullBody;
}
