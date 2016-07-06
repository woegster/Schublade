# Schublade
SchubladenCode ist eine Sammlung an generischen Codefragmenten.

## Generische Funktionen
- `strOps.h` Stirng Operationen die die STL nicht direkt hergibt
- `SockendEndpoint.h` für einheitliches umgehen von sockaddr
- `SockendEndpointConverter.h` wrapper für inet_pton und net_ntop
- `TcpServer.h` Representiert einen TcpServer, der `TcpClient`s verbinden lässt
- `TcpClient.h` ein TcpClient, den der TcpServer akzeptiert hat. Kann auch mittels `ConnectTo` zu einen anderen Server verbinden

## Win32 Funktionen
- `DllInjector.h` lädt eine DLL in den gegebenen Process
- `CpuUsageSampler.h` PDH-API, um die CPU last für jeden Kern auszulesen
- `ProcUtils.h` um andere Processe in Windows zu finden
- `FileUtil.h` finden und lesen von Dateien in einem Ordner

# Unit- & IntegrationsTests
Die gesamte Library ist natürlich unter UnitTest.
Der Netzwerkcode steht unter Integrationstest, das es zu viel Aufwande wäre die Socket API zu faken.
