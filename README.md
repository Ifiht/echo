Echo: a UDP reliability test
====

The source contains a server/client hybrid targetting Unix platforms.
The basic idea is to bounce an echo message back and forth between two servers until one of them or the network itself drops the message (b/c Unreliable Datagram Protocol).

Tested for up to 22,158 bounces.
