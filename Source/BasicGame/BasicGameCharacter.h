// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BasicGameCharacter.generated.h"

UCLASS(config=Game)
class ABasicGameCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	ABasicGameCharacter();

	//Called every frame
	virtual void Tick(float DeltaTime) override;

	//Constantly increasing speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float BaseSpeed;

	//Shows if character parried
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		bool Parried;

	//Damage Stat
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		float DamageStat;
	

	//Number of Attacks (1 = PARRY)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		int AttackNum;

	//Increase AttackNum by 1
	UFUNCTION(BlueprintCallable, Category = "Combat")
		void NextAttack();

	//Checks if correct keys are pressed
	UFUNCTION(BlueprintCallable, Category = "Combat")
		void CombatSystem(float CombatValue, int EnemyHealth);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float CurrentSpeed;

	UFUNCTION(BlueprintPure, Category = "Stats")
		float GetCurrentSpeed();

	UFUNCTION(BlueprintPure, Category = "Stats")
		float GetBaseSpeed();

	UFUNCTION(BlueprintCallable, Category = "Stats")
		void UpdateSpeed(float SpeedUpdate);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float HurtSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		bool WinCondition;

	//Max speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float MaxSpeed;

	//Health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float CharacterHealth;

	UFUNCTION(BlueprintPure, Category = "Stats")
		float GetStartingHealth();

	UFUNCTION(BlueprintPure, Category = "Stats")
		float GetCharacterHealth();

	UFUNCTION(BlueprintCallable, Category = "Stats")
		void UpdateHealth(float Damage);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
		float SlowTimeTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		float CombatValue;

	UFUNCTION(BlueprintCallable, Category = "Combat")
		float GetCombatValue();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		bool InCombat;


	//Current Random Number
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		int CurrentKeyNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		int ActiveKeyNum1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		int ActiveKeyNum2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		int ActiveKeyNum3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		int ActiveKeyNum4;


	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
		float StartingHealth;
	
	UFUNCTION(BlueprintCallable, Category = "Combat")
		void CombatQ();

	UFUNCTION(BlueprintCallable, Category = "Combat")
		void CombatE();
	
	UFUNCTION(BlueprintCallable, Category = "Combat")
		void CombatZ();

	UFUNCTION(BlueprintCallable, Category = "Combat")
		void CombatC();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		bool IsAlive;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		bool IsDead;

	UFUNCTION(BlueprintCallable, Category = "Combat")
		int KeyNumber();

	UFUNCTION(BlueprintCallable, Category = "Combat")
		void AssignCurrnetKeyNum(int AKN);

	UFUNCTION(BlueprintCallable, Category = "Combat")
		bool CombatWin();
	//Gets the active keys
	UFUNCTION(BlueprintCallable, Category = "Combat")
		void CreateActiveKeys();

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

