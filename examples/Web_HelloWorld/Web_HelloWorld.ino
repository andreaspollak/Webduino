/* Web_HelloWorld.pde - very simple Webduino example */

#include <SPI.h>
#include <WiFly.h>
#include "WebServer.h"
#include "Credentials.h"


/* This creates an instance of the webserver.  By specifying a prefix
 * of "", all pages will be at the root of the server. */
#define PREFIX ""
WebServer webserver(PREFIX, 80);

/* commands are functions that get called by the webserver framework
 * they can read any posted data from client, and they output to the
 * server to send data back to the web browser. */
void helloCmd(WebServer &server, WebServer::ConnectionType type, char *, bool)
{
  /* this line sends the standard "we're all OK" headers back to the
     browser */
  server.httpSuccess();

  /* if we're handling a GET or POST, we can output our data here.
     For a HEAD request, we just stop after outputting headers. */
  if (type != WebServer::HEAD)
  {
    /* this defines some HTML text in read-only memory aka PROGMEM.
     * This is needed to avoid having the string copied to our limited
     * amount of RAM. */
    P(helloMsg) = "<h1>Hello, world!</h1>";

    /* this is a special form of print that outputs from PROGMEM */
    server.printP(helloMsg);
  }
}

void setup()
{
  Serial.begin(9600);
  while (!Serial) { /* wait for init */ }
  Serial.println("Serial initialized.");

  /* initialize the WiFly adapter and join network */
  Serial.println("Initializing WiFly...");
  WiFly.begin();
  if (!WiFly.join(ssid, passphrase)) {
    Serial.print("ERR: WiFly failed to join network '");
    Serial.print(ssid);
    Serial.println("'; *HALT*");
    while (1) { /* hang */ }
  }
  Serial.println("WiFly initialized.");

  /* report ip address */
  Serial.print("IP address: ");
  Serial.println(WiFly.ip());  // TODO: This is printing garbage at the moment...

  /* setup our default command that will be run when the user accesses
   * the root page on the server */
  webserver.setDefaultCommand(&helloCmd);

  /* run the same command if you try to load /index.html, a common
   * default page name */
  webserver.addCommand("index.html", &helloCmd);

  /* start the webserver */
  webserver.begin();
}

void loop()
{
  char buff[64];
  int len = 64;

  /* process incoming connections one at a time forever */
  webserver.processConnection(buff, &len);
}
