// Bloody Delisious. All Rights Reserved.

#include "UI/BDSettingsWidget.h"
#include "Components/Slider.h"
#include "Sound/SoundClass.h"
#include "Framework/BDSoundFuncLib.h"

DEFINE_LOG_CATEGORY_STATIC(LogBDSettingsWidget, All, All)

void UBDSettingsWidget::NativeConstruct()
{
    Super::NativeConstruct();

    BindSlider(MusicSlider, MusicSoundClass);
    BindSlider(SFXSlider, SFXSoundClass);
    BindSlider(VolumeSlider, VolumeSoundClass);

    InitializeSliderValues();
}

void UBDSettingsWidget::BindSlider(USlider* Slider, USoundClass* SoundClass)
{
    if (Slider && SoundClass)
    {
        SliderToSoundClassMap.Add(Slider, SoundClass);
        Slider->OnValueChanged.AddDynamic(this, &ThisClass::OnSliderValueChanged);
    }
}

void UBDSettingsWidget::InitializeSliderValues()
{
    for (const auto& Pair : SliderToSoundClassMap)
    {
        USlider* Slider = Pair.Key;
        USoundClass* SoundClass = Pair.Value;

        if (Slider && SoundClass)
        {
            float DefaultVolume = SoundClass->Properties.Volume;
            Slider->SetValue(DefaultVolume);

            UE_LOG(LogBDSettingsWidget, Display, TEXT("Initialized slider for %s with default volume: %f"), *SoundClass->GetName(),
                DefaultVolume);
        }
    }
}

void UBDSettingsWidget::OnSliderValueChanged(float Value)
{
    for (const auto& Pair : SliderToSoundClassMap)
    {
        USlider* Slider = Pair.Key;
        USoundClass* SoundClass = Pair.Value;

        if (Slider && SoundClass)
        {
            if (FMath::IsNearlyEqual(Slider->GetValue(), Value, KINDA_SMALL_NUMBER))
            {
                float VolumeToSet = FMath::Clamp(Value, 0.01f, 1.0f);
                UBDSoundFuncLib::SetSoundClassVolume(SoundClass, VolumeToSet);

                UE_LOG(LogBDSettingsWidget, Display, TEXT("Updated volume for SoundClass: %s to Value: %f"), *SoundClass->GetName(),
                    VolumeToSet);
                break;
            }
        }
    }
}