#include <UIPEthernet.h>

//TODO: Do not use strings.
//TODO: Better checking if it really is a post request.
//TODO: Check if it's not a GET to another page or anything.
//TODO: Also make it possible to send with a RAW connection.
//TODO: Also make it possible to send with a GET request.

const int pinR = 5;
const int pinG = 3;
const int pinB = 6;
int r,g,b;

EthernetServer server = EthernetServer(80);

void setup() {
  //Setup serial
  Serial.begin(115200);
  Serial.print("Startup");

  //Setup pins
  pinMode(pinR,OUTPUT);
  pinMode(pinG,OUTPUT);
  pinMode(pinB,OUTPUT);

  //Setup ethernet
  uint8_t mac[6] = {0x00,0x01,0x02,0x03,0x04,0x05};
  IPAddress myIP(192,168,1,50);

  Ethernet.begin(mac,myIP);
  server.begin();
  Serial.print("Begin");
}

void loop() {
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    boolean parsing = false;
    unsigned char parsingArg = 0;
    String arg = "";
    while (client.connected()) {
      if (client.available()>0) {
        String l = client.readStringUntil('\n');
        Serial.println(l);

        if(l.indexOf("GET") != -1){
          Serial.println("GET request received, sending response.");
          break;//Break out of the connection.
        }
        if(l == "\r"){//This is a bit hacky, it will just check for an line with "\r\n" and say that whatever comes next is part of a post...
          Serial.println("POST request received, start parsing.");
          while(client.available()>0){
            char c = client.read();
            if(c=='R'){
              parsingArg = 0;
            }else
            if(c=='G'){
              parsingArg = 1;
              r = arg.toInt();
            }else
            if(c=='B'){
              parsingArg = 2;
              g = arg.toInt();
            }else
            if(c=='='){
              arg = "";
            }else{
              arg += c;
            }
          }
          b = arg.toInt();
          parsing = false;
          Serial.println("PARSED:");
          Serial.println(r);
          Serial.println(g);
          Serial.println(b);
          analogWrite(pinR,r);
          analogWrite(pinB,b);
          analogWrite(pinG,g);
          break;
        }
      }else{
        break;//If we're not receiving anything anymore, just break.
      }
    }
    //Just always send a HTTP 200 OK, with the HTML page (doesn't matter if it's a GET or a POST.
    client.print( F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\n\r\n<!DOCTYPE html><form method=\"post\" action=\"\">R <input type=\"range\" id=\"r\" name=\"R\" value=\"") );
    client.print(String(r));//R current value
    client.print(F("\" min=\"0\" max=\"255\" oninput=\"c()\"><br>G <input type=\"range\" id=\"g\" name=\"G\" value=\""));
    client.print(String(g));//G curent value
    client.print(F("\" min=\"0\" max=\"255\" oninput=\"c()\"><br>B <input type=\"range\" id=\"b\" name=\"B\" value=\""));
    client.print(String(b));//B current value
    client.print(F("\" min=\"0\" max=\"255\" oninput=\"c()\"><br><div id=\"color\" style=\"width:150px;height:100px;\"></div><br><input type=\"submit\" value=\"Send color\"></form><script>c();function c(){document.getElementById(\'color\').style.background = \"rgb(\"+document.getElementById(\'r\').value+\",\"+document.getElementById(\'g\').value+\",\"+document.getElementById(\'b\').value+\")\";}</script></html>") );
    delay(200);
    // close the connection:
    client.stop();
  }
}

/*
<!DOCTYPE html>
<html>
  <form method="get" action="?">
    R <input type="range" id="r" name="R" value="0" min="0" max="255" oninput="c()"><br>
    G <input type="range" id="g" name="G" value="0" min="0" max="255" oninput="c()"><br>
    B <input type="range" id="b" name="B" value="0" min="0" max="255" oninput="c()"><br>
    <div id="color" style="width:150px;height:100px;"></div><br>
    <input type="submit" value="Send color">
  </form>
  <script>
  c();
  function c(){
    document.getElementById('color').style.background = "rgb("+document.getElementById('r').value+","+document.getElementById('g').value+","+document.getElementById('b').value+")";
  }
  </script>
</html>
 */
