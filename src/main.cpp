/*
    Sketch que envía un paquete UDP a un echo server remoto con un
    puerto asignado y recibe la respuesta del mismo.
*/

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char *ssid = "*******";
const char *password = "*******";

const char *remote_udp_host = "192.168.0.0";
uint16_t remote_udp_port = 8888;

const char *outgoing_message = "Mensaje proveniente de ESP8266";
// No usar char * o al usar Udp.read - Activará el watchdog.
char incoming_packet[255];

unsigned int local_udp_port = 4210;

bool awaiting_response = false;

WiFiUDP Udp;

void setup()
{
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
    // Permite la recepcion y envio correcto de paquetes UDP.
    Udp.begin(local_udp_port);
}

void loop()
{
    if (!awaiting_response)
    {
        Serial.println("Creando y enviando paquete UDP.");
        if (Udp.beginPacket(remote_udp_host, remote_udp_port))
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
            Serial.println("Procedimiento de paquete finalizado. Esperando respuesta...");
            awaiting_response = true;
        }
    }
    else
    {
        int packetSize = Udp.parsePacket();
        // Evalua si se recibió un paquete.
        if (packetSize)
        {
            Serial.print("Paquete UDP de respuesta recibido:");
            Serial.println(packetSize);
            // Se pasa el contenido del paquete a la variable incoming_packet.
            int len = Udp.read(incoming_packet, packetSize);
            Serial.print("Tamaño de paquete: ");
            Serial.println(len);
            // Se imprime por pantalla el contenido del paquete.
            Serial.printf("Contenido paquete UDP: %s\n", incoming_packet);
            awaiting_response = false;
        }
    }
}