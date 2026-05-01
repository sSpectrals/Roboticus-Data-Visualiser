#include "parser/FrameDecoder.h"

DecodedFrame FrameDecoder::decodeMsgPackFrame(const QByteArray &data) const {
  DecodedFrame decoded;

  const QVariant unpacked = MsgPack::unpack(data);
  if (!unpacked.isValid() || unpacked.isNull()) {
    return decoded;
  }

  const QVariantList frame = unpacked.toList();
  if (frame.size() < 3) {
    return decoded;
  }

  decoded.sensors = frame.at(0).toList();
  decoded.vectors = frame.at(1).toList();
  decoded.timestamp = frame.at(2).toLongLong();
  decoded.isValid = !(decoded.sensors.isEmpty() && decoded.vectors.isEmpty());
  return decoded;
}
