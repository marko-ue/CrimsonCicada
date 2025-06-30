#include "Widgets/HUD/HealthWidget.h"
#include "Components/Image.h"
#include "Systems/Inventory/InventoryComponent.h"
#include "Combat/Weapons/AllWeaponsBase.h"
#include "Systems/Stats/StatsComponent.h"

void UHealthWidget::NativeConstruct()
{
	Super::NativeConstruct();

	StatsComp = GetWorld()->GetFirstPlayerController()->GetPawn()->FindComponentByClass<UStatsComponent>();
}

void UHealthWidget::UpdateHealthDigits(int32 CurrentHealth)
{
	CurrentHealth = FMath::Clamp(CurrentHealth, 0, 99);

	int32 LeftDigit = CurrentHealth / 10;
	int32 RightDigit = CurrentHealth % 10;
	
	switch (LeftDigit)
	{
		case 0: LeftDigitImage->SetBrushFromTexture(LeftTexture0); break;
		case 1: LeftDigitImage->SetBrushFromTexture(LeftTexture1); break;
		case 2: LeftDigitImage->SetBrushFromTexture(LeftTexture2); break;
		case 3: LeftDigitImage->SetBrushFromTexture(LeftTexture3); break;
		case 4: LeftDigitImage->SetBrushFromTexture(LeftTexture4); break;
		case 5: LeftDigitImage->SetBrushFromTexture(LeftTexture5); break;
		case 6: LeftDigitImage->SetBrushFromTexture(LeftTexture6); break;
		case 7: LeftDigitImage->SetBrushFromTexture(LeftTexture7); break;
		case 8: LeftDigitImage->SetBrushFromTexture(LeftTexture8); break;
		case 9: LeftDigitImage->SetBrushFromTexture(LeftTexture9); break;
		
		default:
			UE_LOG(LogTemp, Error, TEXT("Invalid digit"));
	}

	switch (RightDigit)
	{
		case 0: RightDigitImage->SetBrushFromTexture(RightTexture0); break;
		case 1: RightDigitImage->SetBrushFromTexture(RightTexture1); break;
		case 2: RightDigitImage->SetBrushFromTexture(RightTexture2); break;
		case 3: RightDigitImage->SetBrushFromTexture(RightTexture3); break;
		case 4: RightDigitImage->SetBrushFromTexture(RightTexture4); break;
		case 5: RightDigitImage->SetBrushFromTexture(RightTexture5); break;
		case 6: RightDigitImage->SetBrushFromTexture(RightTexture6); break;
		case 7: RightDigitImage->SetBrushFromTexture(RightTexture7); break;
		case 8: RightDigitImage->SetBrushFromTexture(RightTexture8); break;
		case 9: RightDigitImage->SetBrushFromTexture(RightTexture9); break;
		
		default:
			UE_LOG(LogTemp, Error, TEXT("Invalid digit"));
	}
}
