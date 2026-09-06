"""
PART 2 - HTTP Client with POST support (Mini Browser)
--------------------------------------------------------
This client can now:
  - send a GET request to view a page
  - send a POST request to simulate submitting a form (login)

Run part2_server.py in another terminal first, then run this:
    python3 part2_client.py
"""

import http.client
from urllib.parse import urlencode


def send_get(path: str):
    conn = http.client.HTTPConnection("localhost", 8000)
    print(f"\n>>> GET {path}\n")

    conn.request("GET", path)
    response = conn.getresponse()

    print(f"Status: {response.status} {response.reason}")
    print("Headers:")
    for key, value in response.getheaders():
        print(f"    {key}: {value}")

    body = response.read().decode("utf-8")
    print("\nBody:")
    print(body)

    conn.close()


def send_post(path: str, form_data: dict):
    conn = http.client.HTTPConnection("localhost", 8000)

    # Turn {"username": "eren", "password": "1234"}
    # into "username=eren&password=1234"
    encoded_body = urlencode(form_data)

    headers = {
        "Content-Type": "application/x-www-form-urlencoded",
        "Content-Length": str(len(encoded_body)),
    }

    print(f"\n>>> POST {path}")
    print(f">>> Body: {encoded_body}\n")

    conn.request("POST", path, body=encoded_body, headers=headers)
    response = conn.getresponse()

    print(f"Status: {response.status} {response.reason}")
    print("Headers:")
    for key, value in response.getheaders():
        print(f"    {key}: {value}")

    body = response.read().decode("utf-8")
    print("\nBody:")
    print(body)

    conn.close()


if __name__ == "__main__":
    print("Mini Browser - CLI version (Part 2)")
    print("1) View the login page (GET)")
    print("2) Submit the login form (POST)")
    print("q) Quit")

    while True:
        choice = input("\nChoose an option (1/2/q): ")

        if choice == "q":
            break
        elif choice == "1":
            send_get("/login")
        elif choice == "2":
            username = input("Username: ")
            password = input("Password: ")
            send_post("/login", {"username": username, "password": password})
        else:
            print("Invalid option, try again.")