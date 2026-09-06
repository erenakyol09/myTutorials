"""
PART 3 - HTTP Client controlling the simulated device (Mini Browser)
-----------------------------------------------------------------------
This client talks to the simulated embedded device server:
  - view the control panel HTML
  - turn the LED on/off via GET + query string
  - read the raw JSON status

Run part3_server.py in another terminal first, then run this:
    python3 part3_client.py
"""

import http.client
import json


def send_get(path: str):
    conn = http.client.HTTPConnection("localhost", 8000)
    print(f"\n>>> GET {path}\n")

    conn.request("GET", path)
    response = conn.getresponse()

    print(f"Status: {response.status} {response.reason}")

    body = response.read().decode("utf-8")
    conn.close()
    return body


if __name__ == "__main__":
    print("Mini Browser - CLI version (Part 3: Device Control)")
    print("1) View control panel (GET /)")
    print("2) Turn LED ON  (GET /led?state=on)")
    print("3) Turn LED OFF (GET /led?state=off)")
    print("4) View raw JSON status (GET /status)")
    print("q) Quit")

    while True:
        choice = input("\nChoose an option (1/2/3/4/q): ")

        if choice == "q":
            break
        elif choice == "1":
            print(send_get("/"))
        elif choice == "2":
            print(send_get("/led?state=on"))
        elif choice == "3":
            print(send_get("/led?state=off"))
        elif choice == "4":
            body = send_get("/status")
            print("Raw body:", body)
            # Parse the JSON so we can use it as a real Python dict
            data = json.loads(body)
            print("Parsed  :", data)
            print("LED is currently:", data["led"])
        else:
            print("Invalid option, try again.")