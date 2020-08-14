#include "settingsdialog.h"
#include "soxresampler.h"
#include "effectsoxrfactory.h"

const EffectProperties EffectSoXRFactory::properties() const
{
    EffectProperties properties;
    properties.name = tr("SoX Resampler Plugin");
    properties.shortName = "soxr";
    properties.hasSettings = true;
    properties.priority = EffectProperties::EFFECT_PRIORITY_HIGH;
    return properties;
}

Effect *EffectSoXRFactory::create()
{
    return new SoXResampler();
}

void EffectSoXRFactory::showSettings(QWidget *parent)
{
    SettingsDialog *s = new SettingsDialog(parent);
    s->show();
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(soxr, EffectSoXRFactory)
#endif
