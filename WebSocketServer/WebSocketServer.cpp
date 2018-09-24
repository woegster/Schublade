#include "TcpServer.h"
#include "TcpClient.h"
#include "SocketEndpointConverter.h"

#include <vector>
#include <regex>
#include <Sha1.h>
#include <Base64.h>

int main()
{
  toni::TcpServer tcpSrv;
  tcpSrv.Start(toni::SocketEndpointFromIPv4("127.0.0.1", 1337), 20);

  std::regex webSocketKey_regex("Sec-WebSocket-Key: (.+?)\r\n");

  while (auto* client = tcpSrv.Accept())
  {
    std::vector<char> buffer(4096);
    int recv = client->Recv(&buffer[0], buffer.size());

    std::string bufferAsString;
    bufferAsString = &buffer[0];

    std::smatch m;
    std::regex_search(bufferAsString, m, webSocketKey_regex);

    std::string asd = m[1].str();
    asd += "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

    toni::SHA1 sha;
    sha.update(asd);
    auto responseKey = sha.final();

    std::string response;
    response += "HTTP/1.1 101 Switching Protocols\r\n";
    response += "Upgrade: websocket\r\n";
    response += "Connection: Upgrade\r\n";
    response += "Sec-WebSocket-Accept: ";
    response += toni::base64_encode(responseKey.data(), responseKey.size());
    response += "\r\n\r\n";

    client->Send(response.c_str(), response.length());

    buffer.clear();
    buffer.resize(4096);
    while ((recv = client->Recv(&buffer[0], buffer.size())) != -1)
    {
      bool fin = (buffer[0] & 0x80) == 0x80;
      char opcode = (buffer[0] & 0x0F);
      bool mask = (buffer[1] & 0x80) == 0x80;
      char payloadlen = (buffer[1] & 0x7F);
      size_t realPayloadLen = payloadlen;
      size_t offset = 2;
      if (payloadlen == 126)
      {
        uint16_t extendedPayload = 0;
        memcpy(&extendedPayload, &buffer[2], sizeof(extendedPayload));
        realPayloadLen = ntohs(extendedPayload);
        offset += sizeof(extendedPayload);
      }
      else if(payloadlen == 127)
      {
        uint64_t extendedPayload = 0;
        memcpy(&extendedPayload, &buffer[2], sizeof(extendedPayload));
        realPayloadLen = ntohll(extendedPayload);
        offset += sizeof(extendedPayload);
      }

      char actualMask[4];
      memcpy(actualMask, &buffer[offset], sizeof(actualMask));
      offset += sizeof(actualMask);

      std::string decoded = "";
      for (size_t i = 0; i < realPayloadLen; offset++, i++)
      {
        decoded += buffer[offset] ^ actualMask[i % 4];
      }


      buffer.clear();
      buffer.resize(4096);

      std::vector<char> response;
      response.push_back(0x81); //fin + opcode 1
      response.push_back(0x02); //no mask + 2 byte length
      response.push_back('O');
      response.push_back('K');
      client->Send(&response[0], response.size());
    }
  } 

  return 0;
}