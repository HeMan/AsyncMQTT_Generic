/****************************************************************************************************************************
  Unsubscribe.cpp
  
  AsyncMqttClient_Generic is a library for ESP32, ESP8266, Protenta_H7, STM32F7, etc. with current AsyncTCP support
  
  Based on and modified from :
  
  1) async-mqtt-client (https://github.com/marvinroger/async-mqtt-client)
  
  Built by Khoi Hoang https://github.com/khoih-prog/AsyncMqttClient_Generic
 
  Version: 1.1.0
  
  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0    K Hoang     10/03/2022 Initial coding to support only ESP32 (with SSL) and ESP8266 (without SSL)
  1.0.1    K Hoang     10/03/2022 Fix Library Manager warnings
  1.1.0    K Hoang     11/03/2022 Add support to WT32_ETH01 (with or without TLS/SSL)
 *****************************************************************************************************************************/

#include "Unsubscribe.hpp"

using AsyncMqttClientInternals::UnsubscribeOutPacket;

UnsubscribeOutPacket::UnsubscribeOutPacket(const char* topic) 
{
  char fixedHeader[5];
  fixedHeader[0] = AsyncMqttClientInternals::PacketType.UNSUBSCRIBE;
  fixedHeader[0] = fixedHeader[0] << 4;
  fixedHeader[0] = fixedHeader[0] | AsyncMqttClientInternals::HeaderFlag.UNSUBSCRIBE_RESERVED;

  uint16_t topicLength = strlen(topic);
  char topicLengthBytes[2];
  topicLengthBytes[0] = topicLength >> 8;
  topicLengthBytes[1] = topicLength & 0xFF;

  uint8_t remainingLengthLength = AsyncMqttClientInternals::Helpers::encodeRemainingLength(2 + 2 + topicLength, fixedHeader + 1);

  size_t neededSpace = 0;
  neededSpace += 1 + remainingLengthLength;
  neededSpace += 2;
  neededSpace += 2;
  neededSpace += topicLength;

  _packetId = _getNextPacketId();
  char packetIdBytes[2];
  packetIdBytes[0] = _packetId >> 8;
  packetIdBytes[1] = _packetId & 0xFF;

  _data.insert(_data.end(), fixedHeader, fixedHeader + 1 + remainingLengthLength);
  _data.insert(_data.end(), packetIdBytes, packetIdBytes + 2);
  _data.insert(_data.end(), topicLengthBytes, topicLengthBytes + 2);
  _data.insert(_data.end(), topic, topic + topicLength);
  _released = false;
}

const uint8_t* UnsubscribeOutPacket::data(size_t index) const 
{
  return &_data.data()[index];
}

size_t UnsubscribeOutPacket::size() const 
{
  return _data.size();
}
