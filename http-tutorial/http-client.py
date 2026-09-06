"""
PART 1 - Simple HTTP Client (Mini Browser)
-------------------------------------------
This client uses the http.client module to send a GET request
to the server and inspect the response in detail.

Run part1_server.py in another terminal first, then run this:
    python3 part1_client.py
"""

import http.client


def send_request(path: str):
    # Connect to the server on localhost:8000
    conn = http.client.HTTPConnection("localhost", 8000)

    print(f"\n>>> Requesting GET {path}...\n")

    # Send the GET request
    conn.request("GET", path)

    # Get the response
    response = conn.getresponse()

    # ---- Print the status line ----
    print(f"Status code   : {response.status}")
    print(f"Status message: {response.reason}")

    # ---- Print the headers ----
    print("Headers       :")
    for key, value in response.getheaders():
        print(f"    {key}: {value}")

    # ---- Read and print the body ----
    body = response.read().decode("utf-8")
    print("\nBody:")
    print(body)

    conn.close()


if __name__ == "__main__":
    print("Mini Browser - CLI version")
    while True:
        path = input("\nPath to visit (type 'q' to quit): ")
        if path.lower() == "q":
            break
        if not path.startswith("/"):
            path = "/" + path
        send_request(path)
