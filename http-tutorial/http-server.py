"""
PART 3 - Simulated Embedded Device (LED Control)
---------------------------------------------------
This simulates the kind of HTTP server you'd write on an embedded
device (e.g. ESP32) to control a piece of hardware over the web.

Endpoints:
    GET /            -> a simple control panel (HTML with ON/OFF links)
    GET /led?state=on   -> turns the "LED" on
    GET /led?state=off  -> turns the "LED" off
    GET /status      -> returns the current state as JSON

Note: embedded devices often prefer GET + query string over POST + body,
because parsing a query string is much simpler than parsing a body with
Content-Length, especially with limited resources.

Run it with:
    python3 part3_server.py
"""

import json
from http.server import BaseHTTPRequestHandler, HTTPServer
from urllib.parse import urlparse, parse_qs

# This variable simulates the state of a real GPIO pin on a device
led_state = "OFF"


class MiniHandler(BaseHTTPRequestHandler):

    def _print_request_info(self):
        print("=" * 50)
        print(f"Request line : {self.requestline}")
        print(f"Method       : {self.command}")
        print(f"Path         : {self.path}")
        print("=" * 50)

    def _send_html(self, body: str, status: int = 200):
        body_bytes = body.encode("utf-8")
        self.send_response(status)
        self.send_header("Content-Type", "text/html; charset=utf-8")
        self.send_header("Content-Length", str(len(body_bytes)))
        self.end_headers()
        self.wfile.write(body_bytes)

    def _send_json(self, data: dict, status: int = 200):
        body_bytes = json.dumps(data).encode("utf-8")
        self.send_response(status)
        self.send_header("Content-Type", "application/json")
        self.send_header("Content-Length", str(len(body_bytes)))
        self.end_headers()
        self.wfile.write(body_bytes)

    def do_GET(self):
        global led_state
        self._print_request_info()

        # self.path can be something like "/led?state=on"
        # urlparse splits it into path="/led" and query="state=on"
        parsed = urlparse(self.path)
        path = parsed.path
        query = parse_qs(parsed.query)  # -> {"state": ["on"]}

        if path == "/":
            html = f"""
            <h1>Device Control Panel</h1>
            <p>Current LED state: <b>{led_state}</b></p>
            <a href="/led?state=on">Turn ON</a> |
            <a href="/led?state=off">Turn OFF</a>
            <br><br>
            <a href="/status">View raw status (JSON)</a>
            """
            self._send_html(html)

        elif path == "/led":
            requested_state = query.get("state", [None])[0]

            if requested_state in ("on", "off"):
                led_state = requested_state.upper()
                html = f"<h1>LED is now {led_state}</h1><a href='/'>Back</a>"
                self._send_html(html)
            else:
                self._send_html("<h1>400 Bad Request</h1><p>Use ?state=on or ?state=off</p>", status=400)

        elif path == "/status":
            self._send_json({"led": led_state})

        else:
            self._send_html(f"<h1>404</h1><p>{path} not found.</p>", status=404)


if __name__ == "__main__":
    server = HTTPServer(("localhost", 8000), MiniHandler)
    print("Server running at: http://localhost:8000")
    print("Open http://localhost:8000/ in a real browser too, if you want!")
    print("Press CTRL+C to stop")
    server.serve_forever()