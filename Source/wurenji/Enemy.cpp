// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemy::AEnemy()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = Mesh;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetGenerateOverlapEvents(true);
	// 在本类销毁时，给本类（敌人类）增加动态效果，指向本类的销毁时调用函数
	this->OnDestroyed.AddDynamic(this, &AEnemy::DestroyHandler);
}

void AEnemy::DestroyHandler(AActor* DestroyedActor)
{
	// 生成特效，在世界的当前变换生成爆炸特效对象
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, this->GetActorTransform());
	// 同时生成爆炸声音对象(使用方法是带有位置的方法，另一个产生声音方法PlaySound2D是平面声音，在世界任何地方都一样，此处不适用)
	// 在本对象的当前位置播放爆炸声音，响度是1.0倍
	UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, this->GetActorLocation(), 2.0f);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// 自动向上移动
	this->AddActorWorldOffset(FVector(0.0f, 0.0f, 0.1f) * FloatSpeed * DeltaTime);
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
