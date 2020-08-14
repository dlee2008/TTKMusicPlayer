#include "ayflyhelper.h"

AyflyHelper::AyflyHelper(const QString &path)
    : m_path(path)
{
    m_info = (ay_info_t*)calloc(sizeof(ay_info_t), 1);
}

AyflyHelper::~AyflyHelper()
{
    close();
}

void AyflyHelper::close()
{
    if(m_info) 
    {
        if(m_info->ay)
         {
            ay_closesong(&m_info->ay);
        }
        free(m_info);
    }
}

bool AyflyHelper::initialize()
{
    FILE *file = stdio_open(m_path.toLocal8Bit().constData());
    if(!file)
    {
        return false;
    }

    int size = stdio_length(file);
    if(size <= 0)
    {
        return false;
    }

    unsigned char *module = (unsigned char *)malloc(size);
    if(!module)
    {
        return false;
    }

    stdio_read(module, size, 1, file);
    stdio_close(file);

    if(!ay_initsongindirect(module, samplerate(), size))
    {
        free(module);
        return false;
    }
    free(module);

    m_info->ay = ay_initsong(m_path.toLocal8Bit().constData(), samplerate());
    if(!m_info->ay)
    {
        return false;
    }

    m_info->length = ay_getsonglength(m_info->ay) / 50 * 1000;
    m_info->rate = size * 8.0 / m_info->length;

    m_meta.insert("title", ay_getsongname(m_info->ay));
    m_meta.insert("artist", ay_getsongauthor(m_info->ay));

    return true;
}

int AyflyHelper::totalTime() const
{
    return m_info->length;
}

void AyflyHelper::seek(qint64 time)
{
    ay_seeksong(m_info->ay, time / 1000 * 50);
}

int AyflyHelper::bitrate() const
{
    return m_info->rate;
}

int AyflyHelper::samplerate() const
{
    return 44100;
}

int AyflyHelper::channels() const
{
    return 2;
}

int AyflyHelper::bitsPerSample() const
{
    return 16;
}

int AyflyHelper::read(unsigned char *buf, int size)
{
    return ay_rendersongbuffer(m_info->ay, buf, size);
}

QVariantMap AyflyHelper::readMetaTags()
{
    return m_meta;
}
