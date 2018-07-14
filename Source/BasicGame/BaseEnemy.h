// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.generated.h"

UCLASS()
class BASICGAME_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseEnemy();

	//Damages Character
	UFUNCTION(BlueprintCallable, Category = "Combat")
		void UpdateHealth(float dmg);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Current Health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float CurrentHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float BaseHealth;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	
};
