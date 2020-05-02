#include "musicspectrumwidget.h"
#include "ui_musicspectrumwidget.h"
#include "musicuiobject.h"
#include "musicqmmputils.h"
#include "musicformats.h"
#include "musictoastlabel.h"
#include "musicwidgetheaders.h"
#include "musicsinglemanager.h"
#include "musicqmmputils.h"
#include "musictopareawidget.h"

#include <QPluginLoader>

///qmmp incldue
#include "florid.h"
#include "visualfactory.h"
#include "lightfactory.h"
#include "soundcore.h"

#define ITEM_DEFAULT_COUNT      3
#define ITEM_OFFSET             107

MusicSpectrumWidget::MusicSpectrumWidget(QWidget *parent)
    : MusicAbstractMoveWidget(false, parent),
      m_ui(new Ui::MusicSpectrumWidget)
{
    m_ui->setupUi(this);
    setFixedSize(size());

    setAttribute(Qt::WA_DeleteOnClose, true);
    setAttribute(Qt::WA_QuitOnClose, true);

    setStyleSheet(MusicUIObject::MQSSMenuStyle02);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_spectrumLayout = nullptr;
    m_ui->mainViewWidget->setStyleSheet(MusicUIObject::MQSSTabWidgetStyle01);

    m_ui->localFileButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
    m_ui->localFileButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->openFileButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
    m_ui->openFileButton->setCursor(QCursor(Qt::PointingHandCursor));

    m_ui->spectrumNormalLayoutButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle06);
    m_ui->spectrumPlusLayoutButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle06);
    m_ui->spectrumWaveLayoutButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle06);
    m_ui->spectrumFloridLayoutButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle06);

#ifdef Q_OS_UNIX
    m_ui->spectrumNormalLayoutButton->setFocusPolicy(Qt::NoFocus);
    m_ui->spectrumPlusLayoutButton->setFocusPolicy(Qt::NoFocus);
    m_ui->spectrumWaveLayoutButton->setFocusPolicy(Qt::NoFocus);
    m_ui->spectrumFloridLayoutButton->setFocusPolicy(Qt::NoFocus);
    m_ui->localFileButton->setFocusPolicy(Qt::NoFocus);
    m_ui->openFileButton->setFocusPolicy(Qt::NoFocus);
#endif

    connect(m_ui->localFileButton, SIGNAL(clicked()), SLOT(localFileButtonClicked()));
    connect(m_ui->openFileButton, SIGNAL(clicked()), SLOT(openFileButtonClicked()));
    connect(m_ui->spectrumNormalLayoutButton, SIGNAL(stateChanged(bool&,QString)), SLOT(spectrumNormalTypeChanged(bool&,QString)));
    connect(m_ui->spectrumPlusLayoutButton, SIGNAL(stateChanged(bool&,QString)), SLOT(spectrumPlusTypeChanged(bool&,QString)));
    connect(m_ui->spectrumWaveLayoutButton, SIGNAL(stateChanged(bool&,QString)), SLOT(spectrumWaveTypeChanged(bool&,QString)));
    connect(m_ui->spectrumFloridLayoutButton, SIGNAL(stateChanged(bool&,QString)), SLOT(spectrumFloridTypeChanged(bool&,QString)));
    connect(m_ui->mainViewWidget, SIGNAL(currentChanged(int)), SLOT(tabIndexChanged(int)));
}

MusicSpectrumWidget::~MusicSpectrumWidget()
{
    M_SINGLE_MANAGER_PTR->removeObject(getClassName());
    foreach(const MusicSpectrum &type, m_types)
    {
        MusicUtils::QMMP::enabledVisualPlugin(type.m_name, false);
    }
    delete m_ui;
}

void MusicSpectrumWidget::tabIndexChanged(int index)
{
    switch(index)
    {
        case 0:
            adjustWidgetLayout(m_ui->spectrumNormalAreaLayout->count() - ITEM_DEFAULT_COUNT);
            break;
        case 1:
            adjustWidgetLayout(m_ui->spectrumPlusAreaLayout->count() - ITEM_DEFAULT_COUNT);
            break;
        case 2:
            adjustWidgetLayout(m_ui->spectrumWaveLayout->count() - ITEM_DEFAULT_COUNT);
            break;
        case 3:
            adjustWidgetLayout(m_ui->spectrumFloridLayout->count() - ITEM_DEFAULT_COUNT);
            break;
        case 4:
            adjustWidgetLayout(m_ui->spectrumLightLayout->count() - ITEM_DEFAULT_COUNT);
            break;
        default:
            break;
    }
}

void MusicSpectrumWidget::spectrumNormalTypeChanged(bool &state, const QString &name)
{
    createSpectrumWidget(state, name, m_ui->spectrumNormalAreaLayout);
    adjustWidgetLayout(m_ui->spectrumNormalAreaLayout->count() - ITEM_DEFAULT_COUNT);
}

void MusicSpectrumWidget::spectrumPlusTypeChanged(bool &state, const QString &name)
{
    createSpectrumWidget(state, name, m_ui->spectrumPlusAreaLayout);
    adjustWidgetLayout(m_ui->spectrumPlusAreaLayout->count() - ITEM_DEFAULT_COUNT);
}

void MusicSpectrumWidget::spectrumWaveTypeChanged(bool &state, const QString &name)
{
    if(name == "lightenvelope")
    {
        createLightWidget(state, name, m_ui->spectrumWaveAreaLayout);
    }
    else
    {
        createSpectrumWidget(state, name, m_ui->spectrumWaveAreaLayout);
    }
    adjustWidgetLayout(m_ui->spectrumWaveAreaLayout->count() - ITEM_DEFAULT_COUNT);
}

void MusicSpectrumWidget::spectrumFloridTypeChanged(bool &state, const QString &name)
{
    createFloridWidget(state, name, m_ui->spectrumFloridAreaLayout);
}

void MusicSpectrumWidget::show()
{
    setBackgroundPixmap(m_ui->background, size());
    MusicAbstractMoveWidget::show();
}

void MusicSpectrumWidget::localFileButtonClicked()
{
    bool state = true;
    createLightWidget(state, "lightspectrum", m_ui->spectrumLightAreaLayout);
}

void MusicSpectrumWidget::openFileButtonClicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setNameFilters(MusicFormats::supportFormatsSpekString());
    if(dialog.exec())
    {
        const QString &path = dialog.selectedFiles().last();
        bool state = true;
        createLightWidget(state, "lightspectrum", m_ui->spectrumLightAreaLayout, path);
    }
}

void MusicSpectrumWidget::fullscreenByUser(QWidget *widget, bool state)
{
    if(state)
    {
        QWidget *parent = TTKStatic_cast(QWidget*, widget->parent());
        if(parent)
        {
            m_spectrumLayout = parent->layout();
            widget->setParent(nullptr);
            widget->setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
            widget->showFullScreen();
        }
    }
    else
    {
        if(m_spectrumLayout)
        {
            widget->showNormal();
            widget->setWindowFlags(Qt::Window);
            m_spectrumLayout->addWidget(widget);
        }
    }
}

void MusicSpectrumWidget::createSpectrumWidget(bool &state, const QString &name, QLayout *layout)
{
    if(state)
    {
        const int before = Visual::visuals()->count();
        MusicUtils::QMMP::enabledVisualPlugin(name, true);
        const QList<Visual*> *vs = Visual::visuals();
        if(before == vs->count())
        {
            showMessageBoxWidget();
            state = false;
            return;
        }

        if(!vs->isEmpty())
        {
            MusicSpectrum sp;
            sp.m_name = name;
            sp.m_obj = vs->last();
            layout->addWidget(sp.m_obj);
            m_types << sp;
            sp.m_obj->setStyleSheet(MusicUIObject::MQSSMenuStyle02);
            connect(sp.m_obj, SIGNAL(fullscreenByUser(QWidget*,bool)), SLOT(fullscreenByUser(QWidget*,bool)));
        }
        else
        {
            showMessageBoxWidget();
            state = false;
        }
    }
    else
    {
        const int index = findSpectrumWidget(name);
        if(index != -1)
        {
            MusicSpectrum t = m_types.takeAt(index);
            layout->removeWidget(t.m_obj);
            MusicUtils::QMMP::enabledVisualPlugin(name, false);
        }
    }
}

void MusicSpectrumWidget::createFloridWidget(bool &state, const QString &name, QLayout *layout)
{
    const int index = findSpectrumWidget(m_lastFloridName);
    if(index != -1)
    {
        MusicSpectrum t = m_types.takeAt(index);
        layout->removeWidget(t.m_obj);
        MusicUtils::QMMP::enabledVisualPlugin(m_lastFloridName, false);
    }

    if(!state)
    {
        m_lastFloridName.clear();
        return;
    }

    const int before = Visual::visuals()->count();
    MusicUtils::QMMP::enabledVisualPlugin(name, true);
    const QList<Visual*> *vs = Visual::visuals();
    if(before == vs->count())
    {
        showMessageBoxWidget();
        state = false;
        return;
    }

    if(!vs->isEmpty())
    {
        m_lastFloridName = name;
        MusicSpectrum sp;
        sp.m_name = name;
        sp.m_obj = vs->last();
        TTKStatic_cast(Florid*, sp.m_obj)->setPixmap(MusicTopAreaWidget::instance()->getRendererPixmap());
        layout->addWidget(sp.m_obj);
        m_types << sp;
        sp.m_obj->setStyleSheet(MusicUIObject::MQSSMenuStyle02);
        connect(sp.m_obj, SIGNAL(fullscreenByUser(QWidget*,bool)), SLOT(fullscreenByUser(QWidget*,bool)));
    }
    else
    {
        showMessageBoxWidget();
        state = false;
    }
}

void MusicSpectrumWidget::createLightWidget(bool &state, const QString &name, QLayout *layout, const QString &url)
{
    if(state)
    {
        if(findSpectrumWidget(name) == -1)
        {
            QPluginLoader loader;
            loader.setFileName(MusicUtils::QMMP::pluginPath("Light", name));
            const QObject *obj = loader.instance();
            LightFactory *factory = nullptr;
            if(obj && (factory = TTKObject_cast(LightFactory*, obj)))
            {
                Light *lightWidget = factory->create(this);
                MusicSpectrum sp;
                sp.m_name = name;
                sp.m_obj = lightWidget;
                m_types << sp;
                layout->addWidget(lightWidget);
            }
        }

        const int index = findSpectrumWidget(name);
        Light *light = TTKStatic_cast(Light*, m_types[index].m_obj);
        light->open(url.isEmpty() ? SoundCore::instance()->path() : url);
    }
    else
    {
        const int index = findSpectrumWidget(name);
        if(index != -1)
        {
            MusicSpectrum t = m_types.takeAt(index);
            layout->removeWidget(t.m_obj);
            delete t.m_obj;
        }
    }
}

void MusicSpectrumWidget::adjustWidgetLayout(int offset)
{
    if(offset < 0)
    {
        offset = 0;
    }
    offset *= ITEM_OFFSET;

    setFixedHeight(offset + 418);
    m_ui->background->setFixedHeight(offset + 418);
    m_ui->backgroundMask->setFixedHeight(offset + 389);
    m_ui->mainViewWidget->setFixedHeight(offset + 390);

    setBackgroundPixmap(m_ui->background, size());
}

int MusicSpectrumWidget::findSpectrumWidget(const QString &name)
{
    if(name.isEmpty())
    {
        return -1;
    }

    for(int i=0; i<m_types.count(); ++i)
    {
        if(m_types[i].m_name.contains(name))
        {
            return i;
        }
    }

    return -1;
}

void MusicSpectrumWidget::showMessageBoxWidget()
{
    MusicToastLabel *toast = new MusicToastLabel(this);
    toast->defaultLabel(this, tr("Spectrum Init Error!"));
}
