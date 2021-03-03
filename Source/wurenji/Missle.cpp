// Fill out your copyright notice in the Description page of Project Settings.


#include "Missle.h"


#include "Enemy.h"
#include "wurenjiGameModeBase.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AMissle::AMissle()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 将网格作为根组件
	RootComponent = Mesh;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	// 启用穿插事件（不希望子弹与物体有真实的碰撞，期望透体而过）
	Mesh->SetGenerateOverlapEvents(true);
	// 在组件上重叠，添加动态
	// 当启用穿插事件时，就会自动调用OncomponentBeginOverlap事件，并且会自动传入一些参数
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AMissle::OverlapHandler);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MoveComp"));
	// 由于想要在射击时子弹就有一个速度，所以给定一个初始值500.0f
	MovementComp->InitialSpeed = 500.0f;
}

/**
 * @description description
 * @author Wedel
 * @param OverlappedComponent 重叠组件
 * @param OtherActor 碰撞对象
 * @param OtherComp 碰撞对象的组件
 * @param OtherBodyIndex
 * @param bFromSweep
 * @param SweepResult
 * @date 2021.03.01 18:59
 */
void AMissle::OverlapHandler(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                             const FHitResult& SweepResult)
{
	// 如果碰撞对向是敌人对象
	if (OtherActor->IsA<AEnemy>())
	{
		// 销毁敌方目标和导弹本身
		OtherActor->Destroy();
		this->Destroy();

		// 获取GameMode(使用Cast<T>将世界中的GameMode强转为我的GameMode类)
		AwurenjiGameModeBase* GameMode = Cast<AwurenjiGameModeBase>(GetWorld()->GetAuthGameMode());
		// 获取全局变量points属性++
		if (GameMode != nullptr)
		{
			GameMode->points += 10000.0f;
		}
	}
}

// Called when the game starts or when spawned
void AMissle::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMissle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMissle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
