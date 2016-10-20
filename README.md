#IPConnect

IPConnect is a P2P Chatting and File Transfering app made on Qt.

###Working

* Starts a Server on main GUI thread at port 2424.
* Connect to a client and moves it to a different thread.
* sends it's information to client on connection and makes a p2p connection
* sends information in form of string lile `APP:METHOD:OPTION:DATA` ex `IPC:MESSAGE:TEXT:hi how are you`

###Features

-[x] GUI
-[x] P2P Connection on TCP
-[x] Connection on different Thread for managing multiple Connection.
-[x] Chatting
-[x] File Transfer
-[ ] Download manager ( to manage downloads on different thread)
-[ ] Auto Discovery ( Effecient Way )
