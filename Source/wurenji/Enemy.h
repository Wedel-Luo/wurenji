// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Enemy.generated.h"

UCLASS()
class WURENJI_API AEnemy : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemy();

	/**
	* 网格，根网格组件，挂载子弹主体
	*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="My Mesh")
	class UStaticMeshComponent* Mesh;

	/*
	 * 对象销毁时的处理程序
	 */
	UFUNCTION()
	void DestroyHandler(AActor* DestroyedActor);

	/*
	 * 添加爆炸特效插槽
	 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="My Special Effects")
	class UParticleSystem* ExplosionParticle;

	/*
	 * 添加爆炸声音对象
	 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="My Special Effects")
	class USoundBase* ExplosionSound;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
