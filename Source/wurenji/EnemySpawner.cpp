// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"


#include "Enemy.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// SpawnArea
	SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea"));
	RootComponent = SpawnArea;
	// 注意：以下设置在继承该C++类的蓝图类中依然可以进行修改
	// 设置敌人生成地区的大小
	SpawnArea->SetBoxExtent(FVector(10000.0f, 10000.0f, 100.0f));
}

void AEnemySpawner::SpawnEnemy()
{
	// 随机位置
	FVector Origin;
	FVector Extends;
	GetActorBounds(false, Origin, Extends);
	FVector SpawnPoint = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extends);
	// 生成对象在世界中，使用世界对象的SpawnActor（生成Actor）方法
	GetWorld()->SpawnActor<AEnemy>(EnemyType, SpawnPoint, FRotator::ZeroRotator/** 默认方向 */);
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(SpawnHandle, this, &AEnemySpawner::SpawnEnemy, SpawnSpan, true);
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
