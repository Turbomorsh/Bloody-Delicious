// Bloody Delisious. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BDSettingsWidget.generated.h"

class USoundClass;
class USlider;

UCLASS()
class BLOODYDELISIOUS_API UBDSettingsWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;

private:
    TMap<USlider*, TObjectPtr<USoundClass>> SliderToSoundClassMap;

    void BindSlider(USlider* Slider, USoundClass* SoundClass);

    void InitializeSliderValues();

    UFUNCTION()
    void OnSliderValueChanged(float Value);

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<USlider> VolumeSlider;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<USlider> MusicSlider;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<USlider> SFXSlider;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<USoundClass> VolumeSoundClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<USoundClass> MusicSoundClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<USoundClass> SFXSoundClass;
};
