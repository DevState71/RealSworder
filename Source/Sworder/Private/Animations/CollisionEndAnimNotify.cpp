// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/CollisionEndAnimNotify.h"

#include "Characters/BasePlayer.h"


void UCollisionEndAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	ABasePlayer* player = Cast<ABasePlayer>(MeshComp->GetOwner());
	if (player) {
		player->SetDamageCollision(false);
	}
}
