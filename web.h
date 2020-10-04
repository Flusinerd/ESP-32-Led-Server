void setupWebServer(){
  webServer.on("/all", HTTP_GET, []() {
    digitalWrite(led, 0);
    String json = getFieldsJson(fields, fieldCount);
    webServer.send(200, "text/json", json);
    digitalWrite(led, 1);
  });

  webServer.on("/fieldValue", HTTP_GET, []() {
    digitalWrite(led, 0);
    String name = webServer.arg("name");
    String value = getFieldValue(name, fields, fieldCount);
    webServer.send(200, "text/json", value);
    digitalWrite(led, 1);
  });

  webServer.on("/fieldValue", HTTP_POST, []() {
    digitalWrite(led, 0);
    String name = webServer.arg("name");
    String value = webServer.arg("value");
    String newValue = setFieldValue(name, value, fields, fieldCount);
    webServer.send(200, "text/json", newValue);
    digitalWrite(led, 1);
  });

  webServer.begin();
  Serial.println ( "HTTP server started" );
}

void webServerLoop() {
  static bool webServerStarted = false;
  if ( WiFi.status() == WL_CONNECTED ){

    //Start Webserver
    if (!webServerStarted) {
      Serial.print("Wifi Connected: ");
      Serial.println(WiFi.localIP());
      Serial.println("Starting Web Server");
      setupWebServer();
      Serial.println("Web Server started!");
      webServerStarted = true;
    }
  }
}
