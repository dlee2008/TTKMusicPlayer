#include "wavevolume.h"
#include "inlines.h"

#include <QMenu>
#include <QPainter>
#include <math.h>
#include <qmmp/soundcore.h>

WaveVolume::WaveVolume(QWidget *parent)
    : Visual(parent)
{
    setWindowTitle(tr("Wave Volume Widget"));
    setMinimumSize(2 * 300 - 30, 105);
}

WaveVolume::~WaveVolume()
{
    if(m_x_scale)
    {
        delete[] m_x_scale;
    }
}

void WaveVolume::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(), Qt::black);
    draw(&painter);
}

void WaveVolume::contextMenuEvent(QContextMenuEvent *)
{
    QMenu menu(this);
    menu.addAction(m_screenAction);
    menu.exec(QCursor::pos());
}

void WaveVolume::process(float *left, float *right)
{
    const int rows = height();
    const int cols = width();

    if(m_rows != rows || m_cols != cols)
    {
        m_rows = rows;
        m_cols = cols;

        if(m_intern_vis_data)
        {
            delete[] m_intern_vis_data;
        }

        if(m_x_scale)
        {
            delete[] m_x_scale;
        }

        m_intern_vis_data = new int[2]{0};
        m_x_scale = new int[2]{0};

        for(int i = 0; i < 2; ++i)
        {
            m_x_scale[i] = pow(pow(255.0, 1.0 / m_cols), i);
        }
    }

    short dest_l[256];
    short dest_r[256];

    calc_freq(dest_l, left);
    calc_freq(dest_r, right);

    const double y_scale = (double) 1.25 * m_rows / log(256);

    short yl = 0;
    short yr = 0;
    int magnitude_l = 0;
    int magnitude_r = 0;

    if(m_x_scale[0] == m_x_scale[1])
    {
        yl = dest_l[0];
        yr = dest_r[0];
    }

    for(int k = m_x_scale[0]; k < m_x_scale[1]; ++k)
    {
        yl = qMax(dest_l[k], yl);
        yr = qMax(dest_r[k], yr);
    }

    yl >>= 7; //256
    yr >>= 7;

    if(yl)
    {
        magnitude_l = int(log(yl) * y_scale);
        magnitude_l = qBound(0, magnitude_l, m_rows);
    }

    if(yr)
    {
        magnitude_r = int(log(yr) * y_scale);
        magnitude_r = qBound(0, magnitude_r, m_rows);
    }

    m_intern_vis_data[0] -= m_analyzer_falloff * m_rows / 15;
    m_intern_vis_data[0] = magnitude_l > m_intern_vis_data[0] ? magnitude_l : m_intern_vis_data[0];

    m_intern_vis_data[1] -= m_analyzer_falloff * m_rows / 15;
    m_intern_vis_data[1] = magnitude_r > m_intern_vis_data[1] ? magnitude_r : m_intern_vis_data[1];

}

void WaveVolume::draw(QPainter *p)
{
    p->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    QLinearGradient line(0, 0, width(), 0);
    line.setColorAt(0.0f, QColor(0, 0xff, 0).dark());
    line.setColorAt(0.65f, QColor(0xff, 0xff, 0).dark());
    line.setColorAt(1.0f, QColor(0xff, 0, 0).dark());
    p->fillRect(0, 0, width(), height(), line);

    line.setColorAt(0.0f, QColor(0, 0xff, 0));
    line.setColorAt(0.65f, QColor(0xff, 0xff, 0));
    line.setColorAt(1.0f, QColor(0xff, 0, 0));

    if(m_intern_vis_data)
    {
        float left = 1.0f, right = 1.0f;
        if(SoundCore::instance())
        {
            left = SoundCore::instance()->leftVolume() / 100.0;
            right = SoundCore::instance()->rightVolume() / 100.0;
        }

        const int wid = ceil(m_rows / 2);
        p->fillRect(0, 0, m_intern_vis_data[0] * left * m_cols/m_rows, wid, line);
        p->fillRect(0, wid, m_intern_vis_data[1] * right * m_cols/m_rows, wid, line);
    }

    p->setPen(Qt::white);
    p->drawText(10, height() / 4, "L");
    p->drawText(10, height() * 3 / 4, "R");
}