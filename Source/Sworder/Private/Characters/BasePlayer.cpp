// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BasePlayer.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputAction.h"


// Sets default values
ABasePlayer::ABasePlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(FName("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(FName("PlayerCamera"));
	PlayerCamera->SetupAttachment(SpringArm);
	PlayerCamera->SetRelativeLocation(FVector(300, 13, 1000));
	//PlayerCamera->SetRelativeRotation(FQuat(FRotator(0, -90, 0)));
	PlayerCamera->SetRelativeRotation(FRotator(-90, 0, 0));

	//SpringArm->bUsePawnControlRotation = true;

}

// Called when the game starts or when spawned
void ABasePlayer::BeginPlay()
{
	Super::BeginPlay();
	
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
		//enhancedInputComponent->BindAction(Input_Look, ETriggerEvent::Triggered, this, &ABasePlayer::InputLook);
	}

}

void ABasePlayer::InputMove(const FInputActionValue& Value)
{
	const FVector2D movement = Value.Get<FVector2D>();
	const FRotator moveRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);


	AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetScaledAxis(EAxis::Y), movement.X);
	AddMovementInput(FRotator(0.0f, GetControlRotation().Yaw, 0.0f).Vector(), movement.Y);
}

