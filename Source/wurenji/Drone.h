// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "Missle.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Pawn.h"
#include "Drone.generated.h"

UCLASS()
class WURENJI_API ADrone : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ADrone();

	/**
	 * 碰撞（体），外框
	 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="My Mesh")
	class UBoxComponent* OutCollision;

	/**
     * 网格，根网格组件，挂载无人机主体
     */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="My Mesh")
	class UStaticMeshComponent* Mesh;

	/**
	* 网格组件，挂载旋翼，插入Mesh的插槽1
	*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="My Mesh")
	class UStaticMeshComponent* Paddle1;

	/**
	* 网格组件，挂载旋翼，插入Mesh的插槽2
	*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="My Mesh")
	class UStaticMeshComponent* Paddle2;

	/**
	* 网格组件，挂载旋翼，插入Mesh的插槽3
	*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="My Mesh")
	class UStaticMeshComponent* Paddle3;

	/**
	* 网格组件，挂载旋翼，插入Mesh的插槽4
	*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="My Mesh")
	class UStaticMeshComponent* Paddle4;

	/**
	* 物理推进器组件，提供向前的推动力
	*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="My Mesh")
	class UPhysicsThrusterComponent* ForwardThruster;

	/**
	* 物理推进器组件，提供向上推动力
	*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="My Mesh")
	class UPhysicsThrusterComponent* UpThruster;

	/**
	* 设置上升力的强度参数，方便蓝图中进行修改
	*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="My Properties")
	float LiftAcc = 10000.0f;

	/**
	 * 设置上升力最大为2000.0f，可见不可变，蓝图只读
	 */
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="My Properties")
	float LiftThrustMax = 2000.0f;

	/**
	* 设置前进力的强度参数，方便蓝图中进行修改
	*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="My Properties")
	float ForwardAcc = 500.0f;


	/**
	* 设置前进力最大为2000.0f，可见不可变，蓝图只读
	*/
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="My Properties")
	float ForwardThrustMax = 2000.0f;

	/**
	* 设置旋转力的强度参数，方便蓝图中进行修改
	*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="My Properties")
	float TurnStrength = 500000.0f;

	/*
	 * 设置旋翼旋转速度
	 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="My Properties")
	float PaddleRotateSpeed = 2000.0f;

	/*
	 * 设置蓝图可编辑的，泛型为Missle的子弹类参数，继承该子弹类的类可以在蓝图中进行切换
	 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="My Properties")
	TSubclassOf<AMissle> Bullet;

	/*
	 * 发射方法
	 */
	UFUNCTION()
	void DoFire(); 
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	/*
	 * 上下移动方法
	 */
	UFUNCTION(BlueprintCallable)
	void Lift(float val);

	/*
	* 前后移动方法
	*/
	UFUNCTION(BlueprintCallable)
	void Forward(float val);

	/*
	* 旋转移动方法
	*/
	UFUNCTION(BlueprintCallable)
	void Turn(float val);

	/*
	 * 定义一个网格组件的数组，用来存放四个旋翼，方便操作
	 */
	TArray<UStaticMeshComponent*> Paddles;

	/*
	 * 旋转无人机的旋翼，由于不需要在编辑器及蓝图中使用，只是纯C++控制，所以无需宏
	 */
	void RotatePaddle(float Delta);
};
