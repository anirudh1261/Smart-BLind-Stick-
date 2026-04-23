#include "web_server.h"
#include <ArduinoJson.h>

WebServerHandler::WebServerHandler() {
  server = new AsyncWebServer(80);
  events = new AsyncEventSource("/events");
  apMode = false;
}

void WebServerHandler::begin(GPSHandler *gps, const char *wifiSSID,
                             const char *wifiPassword) {
  gpsHandler = gps;

  if (wifiSSID != nullptr && wifiPassword != nullptr) {
    // Connect to existing WiFi
    ssid = String(wifiSSID);
    password = String(wifiPassword);

    Serial.println("[WiFi] Connecting to " + ssid);
    WiFi.mode(WIFI_STA);
    WiFi.begin(wifiSSID, wifiPassword);

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
      delay(500);
      Serial.print(".");
      attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\n[WiFi] Connected!");
      Serial.println("[WiFi] IP: " + WiFi.localIP().toString());
      apMode = false;
    } else {
      Serial.println("\n[WiFi] Failed to connect, starting AP mode");
      startAccessPoint();
    }
  } else {
    // Start in AP mode
    startAccessPoint();
  }

  setupRoutes();

  // Start event source for real-time updates
  server->addHandler(events);

  server->begin();
  Serial.println("[WebServer] Started on port 80");
}

void WebServerHandler::startAccessPoint(const char *apName) {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(apName, "12345678"); // Default password

  Serial.println("[WiFi] Access Point started");
  Serial.println("[WiFi] SSID: " + String(apName));
  Serial.println("[WiFi] Password: 12345678");
  Serial.println("[WiFi] IP: " + WiFi.softAPIP().toString());

  apMode = true;
}

void WebServerHandler::setupRoutes() {
  // Serve main tracking page
  server->on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Smart Blind Stick - Family Tracker</title>
    <style>
        * { margin: 0; padding: 0; box-sizing: border-box; }
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            padding: 20px;
        }
        .container {
            max-width: 800px;
            margin: 0 auto;
            background: white;
            border-radius: 20px;
            box-shadow: 0 20px 60px rgba(0,0,0,0.3);
            overflow: hidden;
        }
        .header {
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: white;
            padding: 30px;
            text-align: center;
        }
        .header h1 { font-size: 28px; margin-bottom: 10px; }
        .header p { opacity: 0.9; }
        .status {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(150px, 1fr));
            gap: 15px;
            padding: 20px;
            background: #f8f9fa;
        }
        .status-card {
            background: white;
            padding: 15px;
            border-radius: 10px;
            text-align: center;
            box-shadow: 0 2px 10px rgba(0,0,0,0.1);
        }
        .status-card .label { font-size: 12px; color: #666; margin-bottom: 5px; }
        .status-card .value { font-size: 24px; font-weight: bold; color: #667eea; }
        .map-container {
            padding: 20px;
        }
        #map {
            width: 100%;
            height: 400px;
            border-radius: 10px;
            background: #e9ecef;
            display: flex;
            align-items: center;
            justify-content: center;
            font-size: 18px;
            color: #666;
        }
        .location-info {
            padding: 20px;
            background: #f8f9fa;
        }
        .info-row {
            display: flex;
            justify-content: space-between;
            padding: 10px 0;
            border-bottom: 1px solid #dee2e6;
        }
        .info-row:last-child { border-bottom: none; }
        .info-label { font-weight: 600; color: #495057; }
        .info-value { color: #667eea; }
        .btn {
            display: inline-block;
            padding: 12px 30px;
            background: #667eea;
            color: white;
            text-decoration: none;
            border-radius: 25px;
            font-weight: 600;
            margin: 10px 5px;
            border: none;
            cursor: pointer;
            transition: all 0.3s;
        }
        .btn:hover { background: #764ba2; transform: translateY(-2px); }
        .btn-danger { background: #dc3545; }
        .btn-danger:hover { background: #c82333; }
        .actions { text-align: center; padding: 20px; }
        .status-indicator {
            width: 12px;
            height: 12px;
            border-radius: 50%;
            display: inline-block;
            margin-right: 5px;
            animation: pulse 2s infinite;
        }
        .status-online { background: #28a745; }
        .status-offline { background: #dc3545; }
        @keyframes pulse {
            0%, 100% { opacity: 1; }
            50% { opacity: 0.5; }
        }
    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <h1>🦯 Smart Blind Stick Tracker</h1>
            <p>Real-time location monitoring for family members</p>
        </div>
        
        <div class="status">
            <div class="status-card">
                <div class="label">GPS Status</div>
                <div class="value" id="gps-status">
                    <span class="status-indicator status-offline"></span>
                    Searching...
                </div>
            </div>
            <div class="status-card">
                <div class="label">Satellites</div>
                <div class="value" id="satellites">0</div>
            </div>
            <div class="status-card">
                <div class="label">Battery</div>
                <div class="value" id="battery">--</div>
            </div>
            <div class="status-card">
                <div class="label">Last Update</div>
                <div class="value" id="last-update">Never</div>
            </div>
        </div>
        
        <div class="map-container">
            <div id="map">
                <div>
                    <p>📍 Waiting for GPS fix...</p>
                    <p style="font-size: 14px; margin-top: 10px; color: #999;">
                        This may take 1-2 minutes outdoors
                    </p>
                </div>
            </div>
        </div>
        
        <div class="location-info">
            <div class="info-row">
                <span class="info-label">Latitude:</span>
                <span class="info-value" id="latitude">--</span>
            </div>
            <div class="info-row">
                <span class="info-label">Longitude:</span>
                <span class="info-value" id="longitude">--</span>
            </div>
            <div class="info-row">
                <span class="info-label">Altitude:</span>
                <span class="info-value" id="altitude">--</span>
            </div>
            <div class="info-row">
                <span class="info-label">Speed:</span>
                <span class="info-value" id="speed">--</span>
            </div>
        </div>
        
        <div class="actions">
            <a href="#" class="btn" id="view-map" target="_blank">📍 View on Google Maps</a>
            <a href="#" class="btn" id="share-location">📤 Share Location</a>
        </div>
    </div>

    <script>
        let eventSource = new EventSource('/events');
        
        eventSource.addEventListener('location', function(e) {
            let data = JSON.parse(e.data);
            updateLocation(data);
        });
        
        function updateLocation(data) {
            if (data.valid) {
                document.getElementById('gps-status').innerHTML = 
                    '<span class="status-indicator status-online"></span>Active';
                document.getElementById('latitude').textContent = data.lat.toFixed(6);
                document.getElementById('longitude').textContent = data.lng.toFixed(6);
                document.getElementById('altitude').textContent = data.alt.toFixed(1) + ' m';
                document.getElementById('speed').textContent = data.speed.toFixed(1) + ' km/h';
                document.getElementById('satellites').textContent = data.sats;
                
                // Update map
                let mapUrl = `https://maps.google.com/?q=${data.lat},${data.lng}`;
                document.getElementById('map').innerHTML = 
                    `<iframe width="100%" height="100%" frameborder="0" style="border:0; border-radius:10px;" 
                     src="https://www.google.com/maps/embed/v1/place?key=YOUR_API_KEY&q=${data.lat},${data.lng}&zoom=15" 
                     allowfullscreen></iframe>`;
                
                document.getElementById('view-map').href = mapUrl;
                document.getElementById('share-location').onclick = () => {
                    navigator.clipboard.writeText(mapUrl);
                    alert('Location link copied to clipboard!');
                    return false;
                };
                
                // Update timestamp
                document.getElementById('last-update').textContent = new Date().toLocaleTimeString();
            }
        }
        
        // Fetch initial data
        fetch('/api/location')
            .then(r => r.json())
            .then(data => updateLocation(data));
    </script>
</body>
</html>
        )rawliteral";

    request->send(200, "text/html", html);
  });

  // API endpoint for location data
  server->on("/api/location", HTTP_GET, [this](AsyncWebServerRequest *request) {
    request->send(200, "application/json", getLocationJSON());
  });

  // API endpoint for system status
  server->on("/api/status", HTTP_GET, [this](AsyncWebServerRequest *request) {
    request->send(200, "application/json", getStatusJSON());
  });
}

String WebServerHandler::getLocationJSON() {
  StaticJsonDocument<256> doc;
  GPSLocation loc = gpsHandler->getLocation();

  doc["valid"] = loc.isValid;
  doc["lat"] = loc.latitude;
  doc["lng"] = loc.longitude;
  doc["alt"] = loc.altitude;
  doc["speed"] = loc.speed;
  doc["sats"] = loc.satellites;
  doc["timestamp"] = loc.timestamp;

  String output;
  serializeJson(doc, output);
  return output;
}

String WebServerHandler::getStatusJSON() {
  StaticJsonDocument<256> doc;

  doc["wifi_connected"] = isConnected();
  doc["ip"] = getIPAddress();
  doc["gps_fix"] = gpsHandler->hasValidFix();
  doc["satellites"] = gpsHandler->getSatelliteCount();
  doc["uptime"] = millis() / 1000;

  String output;
  serializeJson(doc, output);
  return output;
}

void WebServerHandler::update() {
  // Periodic location broadcast via Server-Sent Events
  static unsigned long lastBroadcast = 0;
  if (millis() - lastBroadcast > 2000) { // Every 2 seconds
    broadcastLocation();
    lastBroadcast = millis();
  }
}

void WebServerHandler::broadcastLocation() {
  if (gpsHandler->hasValidFix()) {
    events->send(getLocationJSON().c_str(), "location", millis());
  }
}

void WebServerHandler::sendSOSAlert() {
  StaticJsonDocument<256> doc;
  GPSLocation loc = gpsHandler->getLocation();

  doc["type"] = "SOS";
  doc["lat"] = loc.latitude;
  doc["lng"] = loc.longitude;
  doc["maps_link"] = gpsHandler->getGoogleMapsLink();
  doc["timestamp"] = millis();

  String output;
  serializeJson(doc, output);

  events->send(output.c_str(), "sos", millis());

  Serial.println("[WebServer] SOS Alert broadcasted!");
}

bool WebServerHandler::isConnected() {
  return WiFi.status() == WL_CONNECTED || apMode;
}

String WebServerHandler::getIPAddress() {
  if (apMode) {
    return WiFi.softAPIP().toString();
  } else {
    return WiFi.localIP().toString();
  }
}
