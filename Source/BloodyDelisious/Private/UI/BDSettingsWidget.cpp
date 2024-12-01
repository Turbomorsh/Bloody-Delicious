// Bloody Delisious. All Rights Reserved.

#include "UI/BDSettingsWidget.h"
#include "Components/Slider.h"
#include "Sound/SoundClass.h"
#include "Framework/BDSoundFuncLib.h"

DEFINE_LOG_CATEGORY_STATIC(LogBDSettingsWidget, All, All)

void UBDSettingsWidget::NativeConstruct()
{
    Super::NativeConstruct();

    BindSlider(VolumeSlider, VolumeSoundClass);
    BindSlider(MusicSlider, MusicSoundClass);
    BindSlider(SFXSlider, SFXSoundClass);

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
            Slider->SetValue(SoundClass->Properties.Volume);
        }
    }
}

void UBDSettingsWidget::OnSliderValueChanged(float Value)
{
    for (const auto& Pair : SliderToSoundClassMap)
    {
        USlider* Slider = Pair.Key;
        USoundClass* SoundClass = Pair.Value;

        if (Slider && Slider->GetValue() == Value && SoundClass)
        {
            UBDSoundFuncLib::SetSoundClassVolume(SoundClass, Value);
            UE_LOG(LogBDSettingsWidget, Display, TEXT("Updated volume for SoundClass: %s to Value: %f"), *SoundClass->GetName(), Value);
            break;
        }
    }
}