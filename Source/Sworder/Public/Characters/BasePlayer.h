// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BasePlayer.generated.h"

UCLASS()
class SWORDER_API ABasePlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABasePlayer();

protected:
	// Variables
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* PlayerCamera;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



	// ----------------------------------------------------------------------------------- Inputs -----------------------------------------------------------------------------------

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	class UInputMappingContext* DefaultInputMappingContext;

	// Movement
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	class UInputAction* Input_Move;

	void InputMove(const struct FInputActionValue& Value);

	// Camera
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	class UInputAction* Input_Look;

	void InputLook(const FInputActionValue& Value);

	// Attack
	// Movement
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	class UInputAction* Input_Attack;

	void InputAttack(const FInputActionValue& Value);

};
