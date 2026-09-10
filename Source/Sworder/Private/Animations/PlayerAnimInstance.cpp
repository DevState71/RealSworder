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
	if (AttackMontage) {
		UE_LOG(Game, Warning, TEXT("Attack Animation Called"));

		this->Montage_Play(AttackMontage);
		this->Montage_SetEndDelegate(AttackEnded, AttackMontage);
	}
	else
		UE_LOG(Game, Error, TEXT("Attack Animation Called without the attack animation asset"));

	
}
