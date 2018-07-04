// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "BasicGameCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

//////////////////////////////////////////////////////////////////////////
// ABasicGameCharacter

ABasicGameCharacter::ABasicGameCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	CharacterHealth = 5.0f;
	StartingHealth = 5.0f;
	IsAlive = true;
	Crouching = false;
	InCombat = false;
	CurrentKeyNum = 0;

	SlowTimeTime = 3.0f;


	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void ABasicGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	//Combat Key Values
	PlayerInputComponent->BindAction("CombatQ", IE_Pressed, this, &ABasicGameCharacter::CombatQ);
	PlayerInputComponent->BindAction("CombatE", IE_Pressed, this, &ABasicGameCharacter::CombatE);
	PlayerInputComponent->BindAction("CombatZ", IE_Pressed, this, &ABasicGameCharacter::CombatZ);
	PlayerInputComponent->BindAction("CombatC", IE_Pressed, this, &ABasicGameCharacter::CombatC);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABasicGameCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABasicGameCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ABasicGameCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ABasicGameCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ABasicGameCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ABasicGameCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ABasicGameCharacter::OnResetVR);
}


void ABasicGameCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ABasicGameCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ABasicGameCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ABasicGameCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ABasicGameCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ABasicGameCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ABasicGameCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ABasicGameCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

//Returns starting health
float ABasicGameCharacter::GetStartingHealth() {

	return StartingHealth;
}

float ABasicGameCharacter::GetCharacterHealth() {

	return CharacterHealth;
}

void ABasicGameCharacter::UpdateHealth(float Damage) {

	CharacterHealth = GetCharacterHealth() - Damage;

	if (CharacterHealth == 0) {
		IsAlive = false;
	}
}

void ABasicGameCharacter::CombatQ() {
	if ((InCombat == true) && (CurrentKeyNum == 1))
	{
		CombatValue = CombatValue + 1;
	}
	
}

void ABasicGameCharacter::CombatE() {
	if ((InCombat == true) && (CurrentKeyNum == 2))
	{
		CombatValue = CombatValue + 1;
	}

}

void ABasicGameCharacter::CombatZ() {
	if ((InCombat == true) && (CurrentKeyNum == 3))
	{
		CombatValue = CombatValue + 1;
	}

}

void ABasicGameCharacter::CombatC() {
	if ((InCombat == true) && (CurrentKeyNum == 4))
	{
		CombatValue = CombatValue + 1;
	}

}

//Gets how much damage you have done
float ABasicGameCharacter::GetCombatValue() {

	return CombatValue;
}

//Decides if you won
bool ABasicGameCharacter::CombatWin() {
	if (CombatValue > 5) {

		return true;
	}
	else {
		return false;
	}
}

//Randomizer for QTE
int ABasicGameCharacter::KeyNumber() {
	int keynum = rand() % 4 + 1;
	//Q = 1, E + 2, Z = 3, C = 4
	return keynum;
}

void ABasicGameCharacter::AssignCurrnetKeyNum() {
	CurrentKeyNum = KeyNumber();
}

char ABasicGameCharacter::Key() {
	if (CurrentKeyNum == 1) {
		return 'Q';
	}
	else if (CurrentKeyNum == 2) {
		return 'E';
	}
	else if (CurrentKeyNum == 3) {
		return 'Z';
	}
	else {
		return 'C';
	}
}