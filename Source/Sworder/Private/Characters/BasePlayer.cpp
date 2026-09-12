// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BasePlayer.h"

// For UE_Log
#include "../../Sworder.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputAction.h"
#include "Utility/GamePlayerController.h"
#include "Animations/PlayerAnimInstance.h"


// Sets default values
ABasePlayer::ABasePlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(FName("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = false;

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(FName("PlayerCamera"));
	PlayerCamera->SetupAttachment(SpringArm);
	PlayerCamera->SetRelativeLocation(FVector(300, 13, 1000));
	PlayerCamera->SetRelativeRotation(FRotator(-90, 0, 0));

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	WeaponChildActor = CreateDefaultSubobject<UChildActorComponent>(FName("WeaponChildActor"));
	WeaponChildActor->SetupAttachment(GetMesh());


}

void ABasePlayer::EnableMovement(UAnimMontage* AnimMontage, bool bInterupted)
{
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	UE_LOG(Game, Warning, TEXT("Movement Enabled!"));

	// Character rotates in the direction that they are moving
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called when the game starts or when spawned
void ABasePlayer::BeginPlay()
{
	Super::BeginPlay();
	
	AnimInstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AttackStarted.AddDynamic(AnimInstance, &UPlayerAnimInstance::AttackAnimation);

		AnimInstance->AttackEnded.BindUObject(this, &ABasePlayer::EnableMovement);
	}

	bool bAttached = WeaponChildActor->AttachToComponent((GetMesh()), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocketName);
	WeaponChildActor->SetChildActorClass(WeaponClass);
	Weapon = Cast<ABaseWeapon>(WeaponChildActor->GetChildActor());
}

// Called every frame
void ABasePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABasePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* PlayerControl = Cast<APlayerController>(Controller)) {
		if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem< UEnhancedInputLocalPlayerSubsystem>(PlayerControl->GetLocalPlayer()))
		{
			subsystem->AddMappingContext(DefaultInputMappingContext, 0);
		}
	}

	UEnhancedInputComponent* enhancedInputComponent = Cast< UEnhancedInputComponent>(PlayerInputComponent);

	if (enhancedInputComponent)
	{
		// Movement Inputs
		enhancedInputComponent->BindAction(Input_Move, ETriggerEvent::Triggered, this, &ABasePlayer::InputMove);
		enhancedInputComponent->BindAction(Input_Look, ETriggerEvent::Triggered, this, &ABasePlayer::InputLook);
		enhancedInputComponent->BindAction(Input_Attack, ETriggerEvent::Triggered, this, &ABasePlayer::InputAttack);
	}

}

void ABasePlayer::InputMove(const FInputActionValue& Value)
{
	const FVector2D movement = Value.Get<FVector2D>();
	const FRotator moveRotation(0.0f, PlayerCamera->GetRelativeRotation().Yaw, 0.0f);


	AddMovementInput(FRotationMatrix(moveRotation).GetScaledAxis(EAxis::Y), movement.X);
	AddMovementInput(moveRotation.Vector(), movement.Y);

}

void ABasePlayer::InputLook(const FInputActionValue& Value)
{
	const FVector2D look = Value.Get<FVector2D>();
	AddControllerYawInput(look.X);
	AddControllerPitchInput(look.Y);
}

void ABasePlayer::InputAttack(const FInputActionValue& Value)
{

	// Looks in the direction of the mouse
	AGamePlayerController* PlayerController = Cast<AGamePlayerController>(GetController());
	if (PlayerController) {
		FVector MouseWorldPos;
		PlayerController->GetMouseWorldPosition(MouseWorldPos);
		FVector MouseDirection = MouseWorldPos - GetActorLocation();
		MouseDirection.Z = 0;
		SetActorRotation(MouseDirection.Rotation());


		// Stops Character From Rotating
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}

	// Starts Attack Animation
	AttackStarted.Broadcast();

}

