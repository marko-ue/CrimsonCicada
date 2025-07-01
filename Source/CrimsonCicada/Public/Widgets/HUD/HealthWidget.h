// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/HUD/HUDBaseWidget.h"
#include "HealthWidget.generated.h"


/**
 * 
 */
UCLASS()
class CRIMSONCICADA_API UHealthWidget : public UHUDBaseWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UTexture2D* LeftTexture0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UTexture2D* LeftTexture1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UTexture2D* LeftTexture2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UTexture2D* LeftTexture3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UTexture2D* LeftTexture4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UTexture2D* LeftTexture5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UTexture2D* LeftTexture6;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UTexture2D* LeftTexture7;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UTexture2D* LeftTexture8;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UTexture2D* LeftTexture9;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) UTexture2D* RightTexture0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UTexture2D* RightTexture1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UTexture2D* RightTexture2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UTexture2D* RightTexture3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UTexture2D* RightTexture4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UTexture2D* RightTexture5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UTexture2D* RightTexture6;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UTexture2D* RightTexture7;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UTexture2D* RightTexture8;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UTexture2D* RightTexture9;
	
	UPROPERTY(meta = (BindWidget))
	class UImage* LeftDigitImage;

	UPROPERTY(meta = (BindWidget))
	class UImage* RightDigitImage;
	
	UFUNCTION(BlueprintCallable)
	void UpdateHealthDigits(int32 CurrentHealth);

private:
	class UStatsComponent* StatsComp;
};
