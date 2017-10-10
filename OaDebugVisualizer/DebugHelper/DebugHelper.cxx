#include "win32.h"
#include <Manager.hxx>
#include <Resources.hxx>
#include <csignal>
#include <StartDpInitSysMsg.hxx>
#include <Socket.hxx>
#include <cstdint>

struct DpIdentifierFromVisualizer
{
  char vtable[8];
  std::uint32_t sysnum;
  std::uint32_t dp;
  std::int16_t el;
  std::int32_t configNr;
  std::int16_t detail;
  std::int32_t attr;
  std::int16_t type;
};

class DebugHelper : public Manager
{
public:
  DebugHelper()
    : Manager(ManagerIdentifier(API_MAN, 42))
    , exitSignal(false)
  {

  }

  void run()
  {
    Manager::connectToData(StartDpInitSysMsg::TYPE_CONTAINER | StartDpInitSysMsg::DP_IDENTIFICATION);
    Manager::connectToEvent();

    udpSocket.socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    udpSocket.bind("0.0.0.0", 50299);
    int timeout = 250;
    udpSocket.setsockopt(SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
    udpSocket.listen(5);

    long secs = 0;
    long msec = 150000;
    while (!exitSignal)
    {
      DpIdentifierFromVisualizer thingToTranslate;
      CharString sentFromHost;
      unsigned short sendFromPort;
      int bytesRecvd = 0;
      if (udpSocket.recvfrom((char*)&thingToTranslate, sizeof(thingToTranslate), 0, sentFromHost, sendFromPort, bytesRecvd)
         && bytesRecvd == sizeof(thingToTranslate))
      {
        DpIdentifier realDpid;
        realDpid.setSystem(thingToTranslate.sysnum);
        realDpid.setDp(thingToTranslate.dp);
        realDpid.setEl(thingToTranslate.el);
        realDpid.setConfig((DpConfigNrType)thingToTranslate.configNr);
        realDpid.setDetail(thingToTranslate.detail);
        realDpid.setAttr(thingToTranslate.attr);
        realDpid.setDpType(thingToTranslate.type);

        std::cout << "Translating " << realDpid << " from " << sentFromHost << ":" << sendFromPort << std::endl;

        CharString name = "unable to translate";
        Manager::getDpIdentificationPtr()->getName(realDpid, name);

        int bytesSent = 0;
        udpSocket.sendto(name.c_str(), name.len(), 0, sentFromHost, sendFromPort, bytesSent);
      }
      Manager::dispatch(secs, msec);
    }
  }

  void signalHandler(int sig) override
  {
    if ((sig == SIGINT) || (sig == SIGTERM))
    {
      exitSignal = true;
    }

    Manager::signalHandler(sig);
  }
private:
  bool exitSignal;
  Socket udpSocket;
};

int main(int argc, char *argv[])
{
  // Let Manager handle SIGINT and SIGTERM (Ctrl+C, kill)
  // Manager::sigHdl will call virtual function Manager::signalHandler
  signal(SIGINT, Manager::sigHdl);
  signal(SIGTERM, Manager::sigHdl);

  // Initialize Resources, i.e.
  //  - interpret commandline arguments
  //  - interpret config file
  Resources::init(argc, argv);

  DebugHelper mgr;
  mgr.run();

}