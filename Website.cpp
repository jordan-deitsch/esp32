#include "Website.h"
#include <WiFi.h>
#include <WebServer.h>

// ==========================================
// WiFi Configuration
// ==========================================

const char* ssid = "LogIntoMordor";
const char* password = "1network2rule";

// Create HTTP server on port 80
WebServer server(80);

// ==========================================
// Motor Control Functions
// ==========================================

// Replace these functions with your actual
// stepper driver control implementation.

void motorStart(int motor, int direction)
{
    Serial.print("Motor ");
    Serial.print(motor);
    Serial.print(" START - Direction: ");
    Serial.println(direction);

    // Motor driver settings:
    if (motor == 1) {
        bipolar_stepper.set_direction(direction);
        bipolar_stepper.continuous = true;
    }
    else if (motor == 2) {
        unipolar_stepper.set_direction(direction);
        unipolar_stepper.continuous = true;
    }
}

void motorStop(int motor)
{
    Serial.print("Motor ");
    Serial.print(motor);
    Serial.println(" STOP");

    // Motor driver settings:
    if (motor == 1) {
        bipolar_stepper.continuous = false;
    }
    else if (motor == 2) {
        unipolar_stepper.continuous = false;
    }
}

// ==========================================
// HTML Webpage
// ==========================================

const char webpage[] PROGMEM = R"rawliteral(

<!DOCTYPE html>
<html>

<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">

    <title>ESP32 Motor Controller</title>

    <style>
        body {
            font-family: Arial;
            text-align: center;
            margin: 30px;
            background-color: #eeeeee;
        }

        h1 {
            color: #222222;
        }

        .motor {
            background: white;
            padding: 20px;
            margin: 20px auto;
            max-width: 400px;
            border-radius: 10px;
        }

        button {
            padding: 20px;
            margin: 10px;
            width: 130px;
            font-size: 18px;
            border: none;
            border-radius: 8px;
            background-color: #2878d0;
            color: white;
            touch-action: none;
        }

        button:active {
            background-color: #174477;
        }

        input[type="range"] {
            width: 90%;
            max-width: 350px;
            cursor: pointer;
            accent-color: #2878d0;
        }

    </style>

</head>

<body>

    <h1>ESP32 Motor Controller</h1>

    <div class="motor">
        <h2>Motor 1</h2>

        <button
            onpointerdown="startMotor(1,-1,event)"
            onpointerup="stopMotor(1)"
            onpointercancel="stopMotor(1)">
            Reverse
        </button>

        <button
            onpointerdown="startMotor(1,1,event)"
            onpointerup="stopMotor(1)"
            onpointercancel="stopMotor(1)">
            Forward
        </button>
    </div>

    <div class="motor">
        <h2>Motor 2</h2>

        <button
            onpointerdown="startMotor(2,-1,event)"
            onpointerup="stopMotor(2)"
            onpointercancel="stopMotor(2)">
            Reverse
        </button>

        <button
            onpointerdown="startMotor(2,1,event)"
            onpointerup="stopMotor(2)"
            onpointercancel="stopMotor(2)">
            Forward
        </button>
    </div>

    <div class="motor">
        <h2>Servo 1</h2>

        <p>
            Position: <span id="servoValue">90</span>&deg;
        </p>

        <input
            type="range"
            id="servoSlider"
            min="0"
            max="180"
            value="90"
            step="1"
            oninput="updateServo(this.value)"
        >

    </div>

    <script>

        function startMotor(id, direction, event) {

            event.preventDefault();
            event.currentTarget.setPointerCapture(event.pointerId);
            fetch('/motor?id=' + id + '&dir=' + direction);
        }

        function stopMotor(id) {

            fetch('/stop?id=' + id);
        }

        let servoTimer = null;
        let pendingAngle = 90;

        function updateServo(angle) {

            document.getElementById("servoValue").textContent = angle;
            pendingAngle = angle;

            if (servoTimer === null) {

                servoTimer = setTimeout(() => {
                    fetch('/servo?angle=' + pendingAngle);
                    servoTimer = null;
                }, 50);
            }
        }

    </script>

</body>
</html>

)rawliteral";

// ==========================================
// HTTP Request Handlers
// ==========================================

void handleRoot()
{
    server.send_P(200, "text/html", webpage);
}

void handleMotor()
{
    if (!server.hasArg("id") || !server.hasArg("dir")) {
        server.send(400, "text/plain", "Missing arguments");
        return;
    }

    int motor = server.arg("id").toInt();
    int direction = server.arg("dir").toInt();

    if (motor < 1 || motor > 2 ||
        (direction != -1 && direction != 1)) {

        server.send(400, "text/plain", "Invalid motor command");
        return;
    }

    motorStart(motor, direction);

    server.send(200, "text/plain", "Motor started");
}

void handleStop()
{
    if (!server.hasArg("id")) {
        server.send(400, "text/plain", "Missing motor ID");
        return;
    }

    int motor = server.arg("id").toInt();

    if (motor < 1 || motor > 2) {
        server.send(400, "text/plain", "Invalid motor ID");
        return;
    }

    motorStop(motor);

    server.send(200, "text/plain", "Motor stopped");
}

void handleServo()
{
    if (!server.hasArg("angle")) {
        server.send(400, "text/plain", "Missing angle");
        return;
    }

    int angle = server.arg("angle").toInt();

    if (angle < 0 || angle > 180) {
        server.send(400, "text/plain", "Invalid angle");
        return;
    }

    myservo.write(angle);
    servo_angle = angle;

    server.send(200, "text/plain", "Servo position updated");
}

// ==========================================
// Setup
// ==========================================

void Website_setup()
{
    // Connect to WiFi
    WiFi.begin(ssid, password);

    Serial.print("Connecting to WiFi");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("WiFi Connected!");

    Serial.print("ESP32 IP Address: ");
    Serial.println(WiFi.localIP());

    // Configure HTTP endpoints
    server.on("/", HTTP_GET, handleRoot);
    server.on("/motor", HTTP_GET, handleMotor);
    server.on("/stop", HTTP_GET, handleStop);
    server.on("/servo", HTTP_GET, handleServo);

    // Start web server
    server.begin();

    Serial.println("HTTP Server Started");
}

// ==========================================
// Main Loop
// ==========================================

void Website_loop()
{
    server.handleClient();

    // Other non-blocking motor control
    // and servo control tasks can run here.
}
