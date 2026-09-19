// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/CollisionAnimNotify.h"

#include "Characters/BasePlayer.h"


void UCollisionAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	ABasePlayer* player = Cast<ABasePlayer>(MeshComp->GetOwner());
	if (player) {
		player->SetDamageCollision(true);
	}
}
