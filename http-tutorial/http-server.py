"""
PART 1 - Simple HTTP Server
---------------------------
This server uses the http.server module.
It prints every incoming request to the terminal so we can see
what's happening "behind the scenes" of HTTP.

Run it with:
    python3 part1_server.py

Then run the client in another terminal, or visit
http://localhost:8000/hello in a real browser.
"""

from http.server import BaseHTTPRequestHandler, HTTPServer


class MiniHandler(BaseHTTPRequestHandler):
    # This method runs whenever a GET request comes in
    def do_GET(self):
        # ---- Print the incoming request (for learning purposes) ----
        print("=" * 50)
        print(f"Request line : {self.requestline}")
        print(f"Method       : {self.command}")
        print(f"Path         : {self.path}")
        print("Headers      :")
        for key, value in self.headers.items():
            print(f"    {key}: {value}")
        print("=" * 50)

        # ---- Build the response ----
        if self.path == "/hello":
            body = "<h1>Hello World</h1><p>This is an HTTP response.</p>"
            status = 200
        else:
            body = f"<h1>404</h1><p>{self.path} not found.</p>"
            status = 404

        body_bytes = body.encode("utf-8")

        # 1) Status line -> send_response writes it automatically
        self.send_response(status)

        # 2) Headers
        self.send_header("Content-Type", "text/html; charset=utf-8")
        self.send_header("Content-Length", str(len(body_bytes)))

        # 3) Blank line marking the end of headers
        self.end_headers()

        # 4) Body
        self.wfile.write(body_bytes)


if __name__ == "__main__":
    server = HTTPServer(("localhost", 8000), MiniHandler)
    print("Server running at: http://localhost:8000")
    print("Press CTRL+C to stop")
    server.serve_forever()
