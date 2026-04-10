# Group Programming Assignment #2 - udp_server.py
# Group Members: Michael Morgan, Maximus Daversa, Paris Cabatit, Justin Cruz
# Instructor: Dr. Wei Wang

# Import statements
import socket

# Constants
SERVER_HOST = "127.0.0.1"
SERVER_PORT = 9999
BUFFER_SIZE = 1024

#Humorous Message to Append
HUMOROUS_JOKES = " What do you call a fish without eyes? A fsh."

# Append humorous suffix to the received message
def get_reply(message: str) -> str:
    return f"{message}{HUMOROUS_JOKES}"

def main():
    #Create a UDP socket and bind it to the set host and port, and listen for message
    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sock:
        sock.bind((SERVER_HOST, SERVER_PORT))
        print(f"[SERVER] Listening on {SERVER_HOST}:{SERVER_PORT}  (UDP)  -  Ctrl+C to stop\n")

        # Keep listening for incoming messages and reply by adding the humorous suffix to
        #The orginal message, and print the received message and the reply being sent back to the client
        while True:
            data, client_addr = sock.recvfrom(BUFFER_SIZE)
            message = data.decode("ascii")
            reply   = get_reply(message)

            print(f"[SERVER] Received from {client_addr}: '{message}'")
            print(f"[SERVER] Sending reply : '{reply}'\n")

            sock.sendto(reply.encode("ascii"), client_addr)

main()
