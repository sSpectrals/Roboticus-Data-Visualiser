#ifndef FRAMEDECODER_H
#define FRAMEDECODER_H

#include "parser/FrameTypes.h"
#include <QByteArray>
#include <QVariant>
#include <msgpack.h>

class FrameDecoder {
public:
  DecodedFrame decodeMsgPackFrame(const QByteArray &data) const;
};

#endif // FRAMEDECODER_H
