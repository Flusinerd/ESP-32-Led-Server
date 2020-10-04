void setupWifi() {
  WiFi.mode(WIFI_STA);
  Serial.printf("Connecting to %s\n", WIFISSID);
  if (String(WiFi.SSID()) != String(WIFISSID)) {
    WiFi.begin(WIFISSID, WIFIPASS);
  }
}
