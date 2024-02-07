// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterHUD.h"
#include "CharacterOverlay.h"
#include "GameFramework/PlayerController.h"

void ABlasterHUD::DrawHUD()
{
	Super::DrawHUD();

	FVector2D ViewportSize;

	if (GEngine)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);

		const FVector2D ViewportCenter(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);

		float SpreadScaled = CrosshairSpreadMax * HUDPackage.CrosshairSpread;

		ReturnCrosshairs(ViewportCenter, SpreadScaled);
	}

}

void ABlasterHUD::BeginPlay()
{
	Super::BeginPlay();
	AddCharacterOverlay();
}

void ABlasterHUD::AddCharacterOverlay()
{
	APlayerController* PlayerController = GetOwningPlayerController();

	if (PlayerController && CharacterOverlayClass)
	{
		CharacterOverlay = CreateWidget<UCharacterOverlay>(PlayerController, CharacterOverlayClass);
		CharacterOverlay->AddToViewport();
	}
}

void ABlasterHUD::DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread, FLinearColor Color)
{
	const float TextureWidth = Texture->GetSizeX();
	const float TextureHeight = Texture->GetSizeY();

	const FVector2D TextureDrawPoint(
		ViewportCenter.X - (TextureWidth / 2.f) + Spread.X,
		ViewportCenter.Y - (TextureHeight / 2.f) + Spread.Y);

	DrawTexture(Texture, TextureDrawPoint.X, 
		TextureDrawPoint.Y, 
		TextureWidth, 
		TextureHeight,
		0.f,
		0.f,
		1.f,
		1.f,
		Color);


}

void ABlasterHUD::ReturnCrosshairs(FVector2D ViewportCenter, float SpreadScaled)
{
	FVector2D SpreadCenter(0.f, 0.f);
	FVector2D SpreadLeft(-SpreadScaled, 0.f);
	FVector2D SpreadRight(SpreadScaled, 0.f);
	FVector2D SpreadTop(0.f, -SpreadScaled);
	FVector2D SpreadDown(0.f, SpreadScaled);

	if (HUDPackage.CrosshairsCenter)
	{
		DrawCrosshair(HUDPackage.CrosshairsCenter, ViewportCenter, SpreadCenter, HUDPackage.CrosshairsColor);
	}
	if (HUDPackage.CrosshairsLeft)
	{
		DrawCrosshair(HUDPackage.CrosshairsLeft, ViewportCenter, SpreadLeft, HUDPackage.CrosshairsColor);
	}

	if (HUDPackage.CrosshairsRight)
	{
		DrawCrosshair(HUDPackage.CrosshairsRight, ViewportCenter, SpreadRight, HUDPackage.CrosshairsColor);
	}
	if (HUDPackage.CrosshairsTop)
	{
		DrawCrosshair(HUDPackage.CrosshairsTop, ViewportCenter, SpreadTop, HUDPackage.CrosshairsColor);
	}
	if (HUDPackage.CrosshairsDown)
	{
		DrawCrosshair(HUDPackage.CrosshairsDown, ViewportCenter, SpreadDown, HUDPackage.CrosshairsColor);
	}
	
}