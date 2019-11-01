# Simple Socket Chat

A simple chat between a server and its clients programmed with TCP sockets.
A client can send messages only to the server and the server can send a same message for all clients.
Each connection has two threads: one in the client side and one in the server side.

- __Platforms__: Linux and Windows__(not tested)__
- __Used libraries__: Boost.Asio(1.62.0) and thread
- __Programming language__: C++
- __Last code change__: 11/01/2019