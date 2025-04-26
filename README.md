# ft_irc_42_Yerevan

42 School Networking Project: Building a Functional IRC Server

📚 Project Overview
ft_irc is a 42 school project where students implement an Internet Relay Chat (IRC) server from scratch.
The objective is to understand low-level network programming concepts such as sockets, asynchronous event handling, protocol parsing, and multi-client communication.

The server must be able to handle multiple users, channels, and basic IRC commands, closely following the RFC 1459 specification.

🛠️ Features
Server Initialization

Accepts connections over TCP/IP (IPv4)

Handles multiple clients simultaneously (non-blocking)

Authentication

Password-protected server

Nickname and username registration

Channel Management

Create, join, leave channels

Private channels and invitation-only channels

Channel modes (+i, +t, +k, +o, +l)

User Management

Kick, ban, and assign operator privileges

Supported Commands

/PASS, /NICK, /USER, /JOIN, /PART, /PRIVMSG, /QUIT, /KICK, /INVITE, /TOPIC, etc.

Private Messaging

Send private messages to users or channels

Error Handling

Proper error messages as defined in IRC protocol

Clean Shutdown

Graceful handling of server shutdowns and client disconnections

⚙️ How to Run
Prerequisites
C++ Compiler (C++98 or C++11 depending on project rules)

Make

Installation
Clone the repository:

bash
Copy
Edit
git clone https://github.com/your_username/ft_irc.git
cd ft_irc
Compile the server:

bash
Copy
Edit
make
Run the server:

bash
Copy
Edit
./ircserv <port> <password>
<port>: The port number to listen on (usually between 1024 and 65535).

<password>: The password required for clients to connect.

Example:

bash
Copy
Edit
./ircserv 6667 my_secret_pass
🧪 Testing
You can connect to the server using any IRC client like:

HexChat

irssi

WeeChat

Netcat (for basic manual testing)

Example using nc:

bash
Copy
Edit
nc localhost 6667
Commands you can try:

pgsql
Copy
Edit
PASS my_secret_pass
NICK mynickname
USER myusername 0 * :Real Name
JOIN #general
PRIVMSG #general :Hello everyone!
🧱 Project Structure
arduino
Copy
Edit
ft_irc/
├── src/
│   ├── main.cpp
│   ├── Server.cpp / Server.hpp
│   ├── Client.cpp / Client.hpp
│   ├── Channel.cpp / Channel.hpp
│   └── CommandHandler.cpp / CommandHandler.hpp
├── includes/
│   └── (Header files)
├── Makefile
└── README.md
✍️ Authors
Zhenya Karapetyan - Zkarape

📜 Notes
The server does not implement SSL/TLS encryption.

The project focuses on essential IRC functionalities for educational purposes.

Behavior might differ slightly from commercial IRC servers for simplicity and learning goals.

🏆 Acknowledgements
42 Network for the opportunity to build from the ground up

RFC 1459 - The Internet Relay Chat Protocol

Peer programmers and mentors who provided valuable feedback

📜 License
This project is for educational purposes under the 42 Network guidelines.
