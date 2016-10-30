byte colVal(int val){
 int v = val % 1024;
 if(v<=170)
   return (v * 3) / 2;
 if(v<=512)
   return 255;
 if(v<=682)
return 255 - (((v%171) * 3) / 2);
 return 0;
}


//Non-blocking function that starts a connection attempt.
void asyncConnect(){
  Serial.print("[ASYNC] Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
}

int prevState = 6;
int state = -1;

void handleStateTransition(){
  if(prevState != state){//                     STATE TRANSITIONS ONLY!                       //
    Serial.print(F("WiFi state changed: "));
    Serial.print(prevState);
    Serial.print(F(" to "));
    Serial.println(WiFi.status());

    switch(state){
      case WL_CONNECTED:
        Serial.print(F("WiFi connected with IP: "));
        Serial.println(WiFi.localIP());
      break;
      case WL_NO_SHIELD:
        Serial.println(F("No WiFi shield attached!"));
        Serial.println(F("Please check connections/hardware!"));
      break;
      case WL_IDLE_STATUS:
        Serial.println(F("WiFi is now IDLE"));
      break;
      case WL_NO_SSID_AVAIL:
        Serial.println(F("WiFi, no SSID available!"));
      break;
      case WL_SCAN_COMPLETED:
        Serial.println(F("WiFi, scan completed!"));
      break;
      case WL_CONNECT_FAILED:
        Serial.println(F("WiFi connection failed!"));
      break;
      case WL_CONNECTION_LOST:
        Serial.println(F("WiFi connection lost!"));
      break; 
      case WL_DISCONNECTED:
        Serial.println(F("WiFi disconnected!"));
      break;
    }
    prevState = state;
  }
}

void asyncHandle(){
  state = WiFi.status();
  delay(0);

  handleStateTransition();
  
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  while(!client.available()){
    delay(0);
  }
  
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);

  //Only send HTTP response, if it's an HTTP message.
  static boolean isHttp = false;
  if(req.indexOf("/ ") != -1 || req.indexOf("/?") != -1){
    isHttp = true;
    Serial.println("GET received");
  }else{
    isHttp = false;
  }
  
  int idx = req.indexOf("H=");
  int nidx = req.indexOf("&",idx+1);
  if(idx != -1){
    if(nidx == -1){
      nidx = req.indexOf(" ",idx+1);
    }
    if(nidx != -1){
      Serial.print("H: ");
      h = req.substring(idx+2,nidx).toInt();
      Serial.println(h);
    }
  }
  
  /*
  TODO MAYBE add saturation, not sure if it's of any help on RGB LEDstrips, decreasing saturation will only make the colors less vivid, you could also just decrease brightness.
  idx = req.indexOf("S=");
  nidx = req.indexOf("&",idx+1);
  s = req.substring(idx+2,nidx).toInt();
  Serial.print("S: ");
  Serial.println(s);
  */

  idx = req.indexOf("L=");
  nidx = req.indexOf("&",idx+1);
  if(idx != -1){
    if(nidx == -1){
      nidx = req.indexOf(" ",idx+1);
    }
    if(nidx != -1){
      Serial.print("L: ");
      l = req.substring(idx+2,nidx).toInt();
      Serial.println(l);
    }
  }

  //TODO improve HSL TO RGB
  r =colVal(h*2.84+341) /80*l;
  g =colVal(h*2.84) /80*l;
  b =colVal(h*2.84+682) /80*l;
  Serial.print("RGB(");
  Serial.print(r);
  Serial.print(",");
  Serial.print(g);
  Serial.print(",");
  Serial.print(b);
  Serial.println(")");
  analogWrite(pinR,r);
  analogWrite(pinG,g);
  analogWrite(pinB,b);
  Serial.println("OK");

  if(isHttp){
  //Just always send a HTTP 200 OK, with the HTML page (doesn't matter if it's a GET or a POST.
  String r;
  r+= (F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\n\r\n<!DOCTYPE html><form method=\"get\" action=\"\">Color <input type=\"range\" id=\"h\" name=\"H\" value=\""));
  r+= (String(h));//h current value
  r+= (F("\" min=\"0\" max=\"360\" onmouseup=\"this.form.submit()\" oninput=\"c()\"><br>")); //Sat <input type=\"range\" id=\"s\" name=\"S\" value=\""));
  //r+= (String(s));//s curent value
  //r+= (F("\" min=\"0\" max=\"100\" oninput=\"c()\"><br>
  r+= (F("Light <input type=\"range\" id=\"l\" name=\"L\" value=\""));
  r+= (String(l));//l current value
  r+= (F("\" min=\"0\" max=\"80\" onmouseup=\"this.form.submit()\" oninput=\"c()\"><br><div id=\"color\" style=\"width:150px;height:100px;\"></div><br><input type=\"submit\" value=\"Send color\"></form><script>c();function c(){document.getElementById(\'color\').style.background = \"hsl(\"+document.getElementById(\'h\').value+\",100%,\"+document.getElementById(\'l\').value+\"%)\";}</script></html>") );
    client.print(r);
  }else{
    client.print(F("OK\r"));
  }
  client.flush();//Send the last bytes before closing.
  client.stop();//Close the connection.
}

