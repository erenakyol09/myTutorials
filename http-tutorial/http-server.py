"""
PART 2 - HTTP Server with POST support (Login Simulation)
-----------------------------------------------------------
This server now handles two things:
  - GET /login   -> serves a simple HTML login form
  - POST /login  -> reads the submitted username/password and checks them

Run it with:
    python3 part2_server.py
"""

from http.server import BaseHTTPRequestHandler, HTTPServer
from urllib.parse import parse_qs

# Fake "database" - just for this tutorial
VALID_USERNAME = "eren"
VALID_PASSWORD = "1234"


class MiniHandler(BaseHTTPRequestHandler):

    def _print_request_info(self, body=None):
        print("=" * 50)
        print(f"Request line : {self.requestline}")
        print(f"Method       : {self.command}")
        print(f"Path         : {self.path}")
        print("Headers      :")
        for key, value in self.headers.items():
            print(f"    {key}: {value}")
        if body is not None:
            print(f"Body         : {body}")
        print("=" * 50)

    def _send_html(self, body: str, status: int = 200):
        body_bytes = body.encode("utf-8")
        self.send_response(status)
        self.send_header("Content-Type", "text/html; charset=utf-8")
        self.send_header("Content-Length", str(len(body_bytes)))
        self.end_headers()
        self.wfile.write(body_bytes)

    def do_GET(self):
        self._print_request_info()

        if self.path == "/login":
            form_html = """
            <h1>Login</h1>
            <form method="POST" action="/login">
                Username: <input name="username"><br>
                Password: <input name="password" type="password"><br>
                <button type="submit">Login</button>
            </form>
            """
            self._send_html(form_html)
        else:
            self._send_html(f"<h1>404</h1><p>{self.path} not found.</p>", status=404)

    def do_POST(self):
        # 1) Find out how many bytes the body has
        content_length = int(self.headers.get("Content-Length", 0))

        # 2) Read exactly that many bytes from the socket
        raw_body = self.rfile.read(content_length).decode("utf-8")

        self._print_request_info(body=raw_body)

        if self.path == "/login":
            # 3) Parse "username=eren&password=1234" into a dict
            fields = parse_qs(raw_body)
            username = fields.get("username", [""])[0]
            password = fields.get("password", [""])[0]

            if username == VALID_USERNAME and password == VALID_PASSWORD:
                self._send_html(f"<h1>Welcome, {username}!</h1><p>Login successful.</p>")
            else:
                self._send_html("<h1>Login failed</h1><p>Wrong username or password.</p>", status=401)
        else:
            self._send_html(f"<h1>404</h1><p>{self.path} not found.</p>", status=404)


if __name__ == "__main__":
    server = HTTPServer(("localhost", 8000), MiniHandler)
    print("Server running at: http://localhost:8000")
    print("Try GET/POST on: http://localhost:8000/login")
    print("Press CTRL+C to stop")
    server.serve_forever()