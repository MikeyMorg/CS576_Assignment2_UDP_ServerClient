# Group Programming Assignment #2
# Group Members: Michael Morgan, Maximus Daversa, Paris Cabatit, Justin Cruz
# Instructor: Dr. Wei Wang
#
#

# Import statements
import socket

# Constants
SERVER_HOST = "127.0.0.1"
SERVER_PORT = 9999
BUFFER_SIZE = 1024

HUMOROUS_SUFFIX = " - filler humorous joke."

# Append humorous suffix to the received message
def get_reply(message: str) -> str:
    return f"{message}{HUMOROUS_SUFFIX}"

def main():
    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sock:
        sock.bind((SERVER_HOST, SERVER_PORT))
        print(f"[SERVER] Listening on {SERVER_HOST}:{SERVER_PORT}  (UDP)  -  Ctrl+C to stop\n")

        while True:
            data, client_addr = sock.recvfrom(BUFFER_SIZE)
            message = data.decode("ascii")
            reply   = get_reply(message)

            print(f"[SERVER] Received from {client_addr}: '{message}'")
            print(f"[SERVER] Sending reply : '{reply}'\n")

            sock.sendto(reply.encode("ascii"), client_addr)

main()