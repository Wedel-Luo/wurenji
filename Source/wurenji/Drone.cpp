// Fill out your copyright notice in the Description page of Project Settings.


#include "Drone.h"


#include "Kismet/KismetMathLibrary.h"
#include "PhysicsEngine/PhysicsThrusterComponent.h"

// Sets default values
ADrone::ADrone()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 将OutCollision碰撞体设置为根组件
	OutCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("OutCollision"));
	RootComponent = OutCollision;
	// 注意：以下设置在继承该C++类的蓝图类中依然可以进行修改
	// 设置外框大小
	OutCollision->SetBoxExtent(FVector(92.0f, 92.0f, 44.0f));
	// 设置模拟物理效果
	OutCollision->SetSimulatePhysics(true);
	// 给主体实例.锁定X轴旋转
	OutCollision->BodyInstance.bLockXRotation = true;
	// 给主体实例.锁定Y轴旋转
	OutCollision->BodyInstance.bLockYRotation = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(OutCollision);
	Paddle1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Paddle1"));
	Paddle2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Paddle2"));
	Paddle3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Paddle3"));
	Paddle4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Paddle4"));

	// 将子组件挂载到网格上，并指定插槽位置。
	Paddle1->SetupAttachment(Mesh,TEXT("Socket1"));
	Paddle2->SetupAttachment(Mesh,TEXT("Socket2"));
	Paddle3->SetupAttachment(Mesh,TEXT("Socket3"));
	Paddle4->SetupAttachment(Mesh,TEXT("Socket4"));
	// Paddle1->AttachToComponent(Mesh);(该挂载组件的方法不能在构造函数中使用)
	// 将旋翼装入数组中，方便方法中统一调用
	Paddles.Add(Paddle1);
	Paddles.Add(Paddle2);
	Paddles.Add(Paddle3);
	Paddles.Add(Paddle4);

	// 设置推动力，注意该力是持续的力
	UpThruster = CreateDefaultSubobject<UPhysicsThrusterComponent>(TEXT("UpThruster"));
	// 将推动力挂载到根物体上，如果不设置挂载，则自动在物体中心
	UpThruster->SetupAttachment(RootComponent);
	// 设置作用力大小
	// 抵消重力加速度————悬浮
	UpThruster->ThrustStrength = 980.0f;
	// 设置是否默认启动
	UpThruster->SetAutoActivate(true);
	// 设置力的方向
	UpThruster->SetWorldRotation(UKismetMathLibrary::MakeRotFromX(-this->GetActorUpVector()/*得到自己朝上的向量，-表示反方向*/));


	ForwardThruster = CreateDefaultSubobject<UPhysicsThrusterComponent>(TEXT("ForwardThruster"));
	ForwardThruster->SetupAttachment(RootComponent);
	ForwardThruster->ThrustStrength = 0.0f;
	ForwardThruster->SetAutoActivate(true);
	ForwardThruster->SetWorldRotation(
		UKismetMathLibrary::MakeRotFromX(-this->GetActorForwardVector()/*得到自己朝前的向量，-表示反方向*/));
}

void ADrone::DoFire()
{
	// 获取变换对象，下面的方法需要变换参数，实际上就是导弹生成的位置和方向。
	FTransform SocketTransform = Mesh->GetSocketTransform(TEXT("FiringSocket"));
	// 开火实际上就是生成一个导弹对象
	// 生成对象在世界中，使用世界对象的SpawnActor（生成Actor）方法
	GetWorld()->SpawnActor<AMissle>(Bullet, SocketTransform);
}

// Called when the game starts or when spawned
void ADrone::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 当松开按钮时，将上升力恢复到悬停状态
	if (GetInputAxisValue(TEXT("Lift")) == 0.0f)
	{
		UpThruster->ThrustStrength = 980.0f;
	}

	// 当松开按钮时，将前进力恢复到0
	if (GetInputAxisValue(TEXT("Forward")) == 0.0f)
	{
		ForwardThruster->ThrustStrength = 0.0f;
	}

	// 调用旋翼旋转函数，将DeltaTime当做参数传递
	RotatePaddle(DeltaTime);

	// 倾斜度复原
	if (Mesh->GetRelativeRotation().Pitch != 0.0f)
	{
		// 获取当前Pitch值
		float currentPitch = Mesh->GetRelativeRotation().Pitch;
		// float sign = FMath::Sign(currentPitch);
		// 不断给当前相对旋转量增加一个相反的值
		Mesh->AddRelativeRotation(FRotator(-currentPitch * DeltaTime, .0f, .0f));
		// 如果当前旋转量已经到一个很小的值，则直接重设为0
		if (FMath::Abs(Mesh->GetRelativeRotation().Pitch) <= KINDA_SMALL_NUMBER)
		{
			Mesh->SetRelativeRotation(FRotator(.0f, .0f, .0f));
		}
	}
}

// Called to bind functionality to input
void ADrone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 设置操作输入（单次按键，常用于跳跃等动作）
	// PlayerInputComponent->BindAction()
	// 设置轴向输入（持续按键，常用于移动），参数分别是：1. 绑定的轴向映射名称 2. 被绑定对象 3. 操作函数
	PlayerInputComponent->BindAxis(TEXT("Lift"), this, &ADrone::Lift);
	PlayerInputComponent->BindAxis(TEXT("Forward"), this, &ADrone::Forward);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ADrone::Turn);
	// 设置动作输入，针对Fire操作，按下绑定键触发，针对本类对象触发，触犯函数为DoFire
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ADrone::DoFire);
}

void ADrone::Lift(float val)
{
	// 向上时，设置向上的力为：参数 * 向上力系数 * 当前帧时间
	UpThruster->ThrustStrength += val * LiftAcc * GetWorld()->DeltaTimeSeconds;
	// 设置最大力，防止溢出
	UpThruster->ThrustStrength = FMath::Clamp(UpThruster->ThrustStrength, -LiftThrustMax, LiftThrustMax);
}

void ADrone::Forward(float val)
{
	ForwardThruster->ThrustStrength += val * ForwardAcc * GetWorld()->DeltaTimeSeconds;
	ForwardThruster->ThrustStrength =
		FMath::Clamp(ForwardThruster->ThrustStrength, -ForwardThrustMax, ForwardThrustMax);

	// 获取Mesh的旋转角度
	float pitch = Mesh->GetRelativeRotation().Pitch;
	// 如果倾斜角在30°以内，则可以继续倾斜
	if (FMath::Abs(pitch) < 30.0f)
	{
		// 给Mesh网格组件增加相对旋转，而不是给外包装增加
		Mesh->AddRelativeRotation(FRotator(FRotator(-50.0f * GetWorld()->DeltaTimeSeconds * val, .0f, .0f)));
	}
}

void ADrone::Turn(float val)
{
	// 设置旋转强度
	OutCollision->AddTorqueInDegrees(-this->GetActorUpVector() * val * TurnStrength);
}

void ADrone::RotatePaddle(float Delta)
{
	for (auto Paddle : Paddles)
	{
		// 设置旋转参数，Pitch和Roll都是0，只有Yaw方向有值（时间 * 旋转速度），所以只在水平面旋转
		Paddle->AddRelativeRotation(FRotator(.0f, Delta * PaddleRotateSpeed, .0f));
	}
}
