# Group Programming Assignment #2 - udp_client.py
# Group Members: Michael Morgan, Maximus Daversa, Paris Cabatit, Justin Cruz
# Instructor: Dr. Wei Wang

# Import statements
import socket
import sys

# Constants
SERVER_HOST = "127.0.0.1"
SERVER_PORT = 9999
BUFFER_SIZE = 1024
TIMEOUT_SEC = 5

def main():
    # Accept an optional command-line message, default to "Hello"
    message = " ".join(sys.argv[1:]) if len(sys.argv) > 1 else "Hello"

    #Create a UDP socket and set the timeout interval for communication with the server
    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sock:
        sock.settimeout(TIMEOUT_SEC)

        # Send the message to the server
        sock.sendto(message.encode("ascii"), (SERVER_HOST, SERVER_PORT))
        print(f"\n[CLIENT] Sent to server  : \"{message}\"")

        # Receive the reply from the server, with error handling for timeouts
        try:
            data, server_addr = sock.recvfrom(BUFFER_SIZE)
        
        #If a timeout is reached, print an error message and exit
        except socket.timeout:
            print("[CLIENT] ERROR: No response from server (timeout). Is udp_server.py running?")
            return

        # Decode the reply and print it back to the user
        reply = data.decode("ascii")
        print(f"[CLIENT] Reply received  : \"{reply}\"")
        print(f"\n{'='*60}")
        print(f"You sent    : {message}")
        print(f"Server says : {reply}")
        print(f"{'='*60}\n")

main()
