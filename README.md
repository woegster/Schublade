# Schublade
SchubladenCode ist eine Sammlung an generischen Codefragmenten.

## Funktionen
- `strOps.h` Stirng Operationen die die STL nicht direkt hergibt
- `SockendEndpoint.h` f�r einheitliches umgehen von sockaddr
- `SockendEndpointConverter.h` wrapper f�r inet_pton und net_ntop
- `TcpServer.h` Representiert einen TcpServer, der `TcpClient`s verbinden l�sst
- `TcpClient.h` ein TcpClient, den der TcpServer akzeptiert hat. Kann auch mittels `ConnectTo` zu einen anderen Server verbinden
- `DllInjector.h` l�dt eine DLL in den gegebenen Process
- `CpuUsageSampler.h` PDH-API, um die CPU last f�r jeden Kern auszulesen
- `ProcUtils.h` um andere Processe in Windows zu finden

## Unit- & IntegrationsTests
Die gesamte Library ist nat�rlich unter UnitTest.
Der Netzwerkcode steht unter Integrationstest, das es zu viel Aufwande w�re die Socket API zu faken.
