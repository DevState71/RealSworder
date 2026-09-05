// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/GamePlayerController.h"




void AGamePlayerController::BeginPlay()
{
	this->SetShowMouseCursor(true);

}

void AGamePlayerController::GetMouseWorldPosition(FVector& MouseWorldPosition)
{
	FVector WorldPosition;
	FVector WorldDirection;
	DeprojectMousePositionToWorld(WorldPosition, WorldDirection);

	float DistanceToGround = GetPawn()->GetActorLocation().Z - WorldPosition.Z / WorldDirection.Z;
	MouseWorldPosition = WorldPosition + WorldDirection * DistanceToGround;

}
