// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Enemy.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

UCLASS()
class WURENJI_API AEnemySpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemySpawner();

	/**
	* 敌人生成区域，使用Box外框
	*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="My Mesh")
	class UBoxComponent* SpawnArea;

	/*
	 * 泛型为敌人类的敌人种类
	 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="My Property")
	TSubclassOf<AEnemy> EnemyType;

	/*
	 * 设置生成间隔时间
	 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="My Property")
	float SpawnSpan = 1.0f;
	
	UFUNCTION()
	void SpawnEnemy();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	FTimerHandle SpawnHandle;
};
