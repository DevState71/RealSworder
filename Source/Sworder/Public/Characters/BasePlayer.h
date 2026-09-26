// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Weapons/BaseWeapon.h"
#include "Components/StatusComponent.h"
#include "Components/TagManager.h"
#include "BasePlayer.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEventDispatch);

UCLASS()
class SWORDER_API ABasePlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABasePlayer();

	UFUNCTION(BlueprintCallable)
	void HandleStatusTag(FGameplayTag tag, bool bAdded);

protected:
	// Variables
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* PlayerCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")	
	UChildActorComponent* WeaponChildActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	ABaseWeapon* Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<ABaseWeapon> WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName WeaponSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	UStatusComponent* StatusComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	UTagManager* TagManager;




	UFUNCTION()
	void EnableMovement(UAnimMontage* AnimMontage, bool bInterupted);

	// Rotates the player to face whereever the mouse is on screen
	UFUNCTION(BlueprintCallable)
	void RotatePlayerTowardMouse();

	// Player attack Collision variables / functions
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UBoxComponent* DamageCollision;

	bool bIsAttacking : 1;

	UFUNCTION()
	void PlayerDamageCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:
	void SetDamageCollision(bool bCollision);

	// ----------------------------------------------------------------------------------- Animation -----------------------------------------------------------------------------------
protected:
	class UPlayerAnimInstance* AnimInstance;

	FEventDispatch AttackStarted;

	// Variables that allow for attack animations to be changed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<class UAnimSequence*> AttackAnimations;
	int AttackAnimationIndex = 0;
	void UpdateAttackAnimation();

	// ----------------------------------------------------------------------------------- End Animation -----------------------------------------------------------------------------------


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
