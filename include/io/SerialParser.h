#ifndef SERIALPARSER_H
#define SERIALPARSER_H

#include "../parser/FrameTypes.h"
#include "../parser/SerialFrameExtractor.h"

#include <QByteArray>

class SerialParser : public QObject {
  Q_OBJECT
public:
  explicit SerialParser(QObject *parent = nullptr);

signals:
  void frameDecoded(const DecodedFrame &frame);
  void dataReceived(const QByteArray &data);

public slots:
  void onRawDataReady(const QByteArray &data);

private:
  SerialFrameExtractor m_frameExtractor;

  void processMsgPackData(const QByteArray &jsonData);
  DecodedFrame decodeMsgPackFrame(const QByteArray &data) const;
};

#endif // SERIALPARSER_H
