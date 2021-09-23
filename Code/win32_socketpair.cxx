int socketpair(int domain, int type, int protocol, bool nonBlocking, SOCKET sv[2])
{
  if (domain != AF_INET ||
      type != SOCK_STREAM ||
      protocol != IPPROTO_TCP)
  {
    return -1;
  }

  SOCKET listener = socket(domain, type, protocol);
  if (listener == INVALID_SOCKET)
    return -1;

  SOCKET pipeA = INVALID_SOCKET;
  SOCKET pipeB = INVALID_SOCKET;

  u_long nonBlockingTyped = nonBlocking ? 1 : 0;
  bool worked = false;
  //break from never repeat do-while, basically goto
  do
  {
    BOOL reuse = TRUE;
    if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) != 0)
      break;

    //as the intent is to send this byte for byte, nodelay makes sense here
    BOOL noDelay = TRUE;
    if (setsockopt(listener, IPPROTO_TCP, TCP_NODELAY, (const char*)&noDelay, sizeof(noDelay)) != 0)
      break;

    sockaddr_in bindAddr;
    bindAddr.sin_family = AF_INET;
    bindAddr.sin_port = 0;
    bindAddr.sin_addr.S_un.S_addr = htonl(INADDR_LOOPBACK);

    if (bind(listener, (const sockaddr*)&bindAddr, sizeof(bindAddr)) != 0)
      break;
    
    int len = sizeof(bindAddr);
    if (getsockname(listener, (sockaddr*)&bindAddr, &len) != 0)
      break;

    if (listen(listener, 1) != 0)
      break;

    pipeA = socket(domain, type, protocol);
    if (pipeA == INVALID_SOCKET)
      break;

    if (connect(pipeA, (const sockaddr*)&bindAddr, sizeof(bindAddr)) != 0
        && WSAGetLastError() != WSAEWOULDBLOCK)
      break;

    pipeB = accept(listener, nullptr, nullptr);
    if (pipeB == INVALID_SOCKET)
      break;

    if (ioctlsocket(pipeB, FIONBIO, &nonBlockingTyped) != 0)
      break;

    if (ioctlsocket(pipeA, FIONBIO, &nonBlockingTyped) != 0)
      break;

    worked = true;
  }
  while (0);

  closesocket(listener);
  listener = INVALID_SOCKET;

  if (worked)
  {
    sv[0] = pipeA;
    sv[1] = pipeB;
    return 0;
  }

  if (pipeA != INVALID_SOCKET)
    closesocket(pipeA);
  if (pipeB != INVALID_SOCKET)
    closesocket(pipeB);
  return -1;
}
