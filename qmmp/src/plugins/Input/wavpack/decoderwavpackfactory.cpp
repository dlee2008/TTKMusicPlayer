#include <qmmp/cueparser.h>
#include "wavpackmetadatamodel.h"
#include "decoder_wavpack.h"
#include "decoderwavpackfactory.h"

bool DecoderWavPackFactory::canDecode(QIODevice *input) const
{
    char buf[4];
    return (input->peek(buf, 4) == 4 && !memcmp(buf, "wvpk", 4));
}

DecoderProperties DecoderWavPackFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("WavPack Plugin");
    properties.filters << "*.wv";
    properties.description = tr("WavPack Files");
    properties.contentTypes << "audio/x-wavpack";
    properties.shortName = "wavpack";
    properties.noInput = true;
    properties.protocols << "file" << "wvpack";
    return properties;
}

Decoder *DecoderWavPackFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderWavPack(path);
}

QList<TrackInfo*> DecoderWavPackFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *ignoredFiles)
{
    Q_UNUSED(ignoredFiles);

    int track = -1; //cue track
    QString filePath = path;

    if(path.contains("://")) //is it cue track?
    {
        filePath.remove("wvpack://");
        filePath.remove(QRegExp("#\\d+$"));
        track = path.section("#", -1).toInt();
        parts = TrackInfo::AllParts; //extract all metadata for single cue track
    }

    TrackInfo *info = new TrackInfo(filePath);

    if(parts == TrackInfo::Parts())
    {
        return QList<TrackInfo*>() << info;
    }

    char err[80] = { 0 };

#if defined(Q_OS_WIN) && defined(OPEN_FILE_UTF8)
    WavpackContext *ctx = WavpackOpenFileInput(filePath.toUtf8().constData(), err, OPEN_WVC | OPEN_TAGS | OPEN_FILE_UTF8, 0);
#else
    WavpackContext *ctx = WavpackOpenFileInput(filePath.toLocal8Bit().constData(), err, OPEN_WVC | OPEN_TAGS, 0);
#endif
    if(!ctx)
    {
        qWarning("DecoderWavPackFactory: error: %s", err);
        delete info;
        return QList<TrackInfo*>();
    }

    if(parts & TrackInfo::Properties)
    {
        info->setValue(Qmmp::BITRATE, WavpackGetAverageBitrate(ctx, 1));
        info->setValue(Qmmp::SAMPLERATE, WavpackGetSampleRate(ctx));
        info->setValue(Qmmp::CHANNELS, WavpackGetNumChannels(ctx));
        info->setValue(Qmmp::BITS_PER_SAMPLE, WavpackGetBitsPerSample(ctx));
        info->setValue(Qmmp::FORMAT_NAME, "WavPack");
        info->setValue(Qmmp::FILE_SIZE, QFileInfo(filePath).size()); //adds file size for cue tracks
        info->setDuration((qint64)WavpackGetNumSamples(ctx) * 1000 / WavpackGetSampleRate(ctx));
    }

    if(parts & TrackInfo::ReplayGainInfo)
    {
        char value[200] = { 0 };
        WavpackGetTagItem(ctx, "REPLAYGAIN_TRACK_GAIN", value, sizeof(value));
        info->setValue(Qmmp::REPLAYGAIN_TRACK_GAIN, value);
        WavpackGetTagItem(ctx, "REPLAYGAIN_TRACK_PEAK", value, sizeof(value));
        info->setValue(Qmmp::REPLAYGAIN_TRACK_PEAK, value);
        WavpackGetTagItem(ctx, "REPLAYGAIN_ALBUM_GAIN", value, sizeof(value));
        info->setValue(Qmmp::REPLAYGAIN_ALBUM_GAIN, value);
        WavpackGetTagItem(ctx, "REPLAYGAIN_ALBUM_PEAK", value, sizeof(value));
        info->setValue(Qmmp::REPLAYGAIN_ALBUM_PEAK, value);
    }

    if(parts & TrackInfo::MetaData)
    {
        int cue_len = WavpackGetTagItem (ctx, "cuesheet", nullptr, 0);
        if(cue_len > 0)
        {
            char value[cue_len + 1];
            memset(value, 0, cue_len + 1);
            WavpackGetTagItem(ctx, "cuesheet", value, cue_len + 1);

            CueParser parser(value);
            parser.setDuration(info->duration());
            parser.setProperties(info->properties());
            parser.setUrl("wvpack", filePath);

            WavpackCloseFile(ctx);
            delete info;
            return (track > 0) ? parser.createPlayList(track) : parser.createPlayList();
        }
        else
        {
            char value[200] = { 0 };
            WavpackGetTagItem (ctx, "Album", value, sizeof(value));
            info->setValue(Qmmp::ALBUM, QString::fromUtf8(value));
            WavpackGetTagItem (ctx, "Artist", value, sizeof(value));
            info->setValue(Qmmp::ARTIST, QString::fromUtf8(value));
            WavpackGetTagItem (ctx, "Album Artist", value, sizeof(value));
            info->setValue(Qmmp::ALBUMARTIST, QString::fromUtf8(value));
            WavpackGetTagItem (ctx, "Comment", value, sizeof(value));
            info->setValue(Qmmp::COMMENT, QString::fromUtf8(value));
            WavpackGetTagItem (ctx, "Genre", value, sizeof(value));
            info->setValue(Qmmp::GENRE, QString::fromUtf8(value));
            WavpackGetTagItem (ctx, "Title", value, sizeof(value));
            info->setValue(Qmmp::TITLE, QString::fromUtf8(value));
            WavpackGetTagItem (ctx, "Composer", value, sizeof(value));
            info->setValue(Qmmp::COMPOSER, QString::fromUtf8(value));
            WavpackGetTagItem (ctx, "Year", value, sizeof(value));
            info->setValue(Qmmp::YEAR, QString::fromUtf8(value).toInt());
            WavpackGetTagItem (ctx, "Track", value, sizeof(value));
            info->setValue(Qmmp::TRACK, QString::fromUtf8(value).toInt());
            WavpackGetTagItem (ctx, "Disc", value, sizeof(value));
            info->setValue(Qmmp::DISCNUMBER, QString::fromUtf8(value).toInt());
        }
    }

    WavpackCloseFile (ctx);
    return QList<TrackInfo*>() << info;
}

MetaDataModel* DecoderWavPackFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    if(!path.contains("://") || path.startsWith("wvpack://"))
        return new WavPackMetaDataModel(path, readOnly);
    else
        return nullptr;
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(wavpack, DecoderWavPackFactory)
#endif
