Google Test:
before connect, MSS: 536, receive buffer size:131072 Byte
after connect, MSS: 1368, receive buffer size:131072 Byte


RPI Test:
before connect, MSS: 536, receive buffer size:131072 Byte
after connect, MSS: 1448, receive buffer size:131072 Byte


localhost Test:
before connect, MSS: 536, receive buffer size:131072 Byte
after connect, MSS: 32741, receive buffer size:131072 Byte




Explain why these values did or didn’t change, and why they might be different for the different IP addresses that were tested.

MSS: 
They changes because they were default value before the connection(So they were the same 536). 
After the connection, the MSS is agreed on during the TCP handshake: 
both devices communicate the size of the packets they are able to receive. So these value will change.
It's different for different IP addresses since it pick the smaller between two SMM of the two IP address on both sides.


Receive Buffer Size:
It stays the same since it is always default value 131072 Bytes. 
There's no receiving procedures executed so there's no change in the buffer size.