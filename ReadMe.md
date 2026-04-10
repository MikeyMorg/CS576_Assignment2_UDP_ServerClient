Programming Assignment #2 
CS 576 - Computer Networks and Distributed Systems 
Instructor: Dr. Wei Wang

Group Members:
Michael Morgan
Maximus Daversa
Paris Cabatit
Justin Cruz

Overview:
Our project implements a simple UDP connectionless communication between a client and server using Python. The client sends a message to the server without establishing a connection, using "Hello" as our default input. The server will listen on a chosen port, receive the message, append a humorous string, and send the modified message back to the client. The client will then display the original and returned message.

System Design:

Server:
The server uses a simple UDP client-server design. The server runs on a localhost port 9999 and waits for incoming UDP messages from the client. Because UDP is connectionless, the server doesn't establish a session before obtaining data. As a message arrives from the client, the server will read the ASCII characters and appends a humorous message at the end, and sends the modified message back to client through their address and port information.
Client:
The client uses UDP to send a single message to the server on localhost port 9999. No connection setup is required for transmission. The client sends an ASCII character message like "Hello," to the server and waits for a reply. Timeout is included so the client isn't waiting forever for a returned message, and if the server isn't running. Once the reply is received, it decodes the ASCII data and prints out the original message and response from the server.

Implementation:

Server:
The server is implemented through Python using the socket library with UDP. This creates a socket and binds it to a localhost (127.0.0.1) on port 9999. The server runs an infinite loop in hopes of retrieving incoming messages through recvfrom(), which provides the client's address. As a message is received, it is decoded from ASCII and then passed to a helper function that appends a humorous message, then sending it back to the client. The server prints both the received message and the reply for logging purposes and continues listening for additional requests.
Client:
The client is implemented through Python as well, using the socket library with UDP. It creates a UDP socket and sets a timeout of 5 seconds in order to avoid waiting indefinitely for a response. The client prepares a message (default message being "Hello"), encodes with ASCII, and sends to the server. Later, it waits for a received message from the server. If a message is sent back from the server, it is decoded and displayed along with the original message. If there is no response back within the timeout period, an error message will display and program exits.

Test and Results:
(Normal Run Screenshot)
(Inputs and outputs)

How To Run?:
1. In Terminal cd to project folder
2. Run server using command 'python udp_server.py'
3. In a separate terminal, cd to the same folder
4. In the second, new, terminal run the command 'python udp_client.py [Your Message Here]'
 The [Your Message Here] is a placeholder, feel free to put anything in as long as it follows ASCII characters

Conclusion:

Source Code:
