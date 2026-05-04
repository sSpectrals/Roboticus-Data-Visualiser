#ifndef SERIALFRAMEEXTRACTOR_H
#define SERIALFRAMEEXTRACTOR_H

#include <QByteArray>
#include <QList>

class SerialFrameExtractor
{
public:
    void appendData(const QByteArray &chunk);
    QList<QByteArray> takeCompleteFrames();

private:
    QByteArray m_buffer;
};

#endif // SERIALFRAMEEXTRACTOR_H
