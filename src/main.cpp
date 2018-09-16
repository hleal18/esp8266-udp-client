#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char *ssid = "****";
const char *password = "*****";

const char *remote_host = "0.0.0.0";
uint16_t remote_port = 8888;

const char *outgoing_message = "Mensaje proveniente de ESP8266";

WiFiUDP Udp;

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);
    Serial.println();

    Serial.printf("Connecting to %s ", ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println(" connected");
}

void loop()
{
    // put your main code here, to run repeatedly:
    delay(1000);
    Serial.println("Creating and sending UDP Pakcet");
    if (Udp.beginPacket(remote_host, remote_port))
    {
        Serial.println("Servidor remoto UDP correcto");
        Udp.write(outgoing_message, strlen(outgoing_message));
        Serial.print("Tamaño del mensaje: ");
        Serial.println(strlen(outgoing_message));
        // Finaliza y envía el paquete, 1 si es correcto 0 si falló.
        if (Udp.endPacket())
            Serial.println("Paquete enviado correctamente.");
        else
            Serial.println("Paquete no se envio correctamente.");
        Serial.println("Procedimiento de paquete finalizado.");
    }
    delay(1000);
}