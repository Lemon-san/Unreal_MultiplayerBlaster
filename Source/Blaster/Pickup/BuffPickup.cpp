
#include "BuffPickup.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/Components/BuffComponent.h"



ABuffPickup::ABuffPickup()
{
	OverlapSphere->SetSphereRadius(100.f);

	OverlapSphere->AddLocalOffset(FVector(0.f, 0.f, 25.f));
	PickupMesh->SetRenderCustomDepth(true);

}



void ABuffPickup::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(OtherActor);
	if (BlasterCharacter)
	{
		UBuffComponent* Buff = BlasterCharacter->GetBuffComponent();

		if (Buff)
		{
			Buff->Heal(HealAmount,HealingTime);
		}
	}

	Destroy();
}
