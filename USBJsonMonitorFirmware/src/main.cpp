
#include "ArduinoJson.h"
#include <SPI.h>
#include <TFT.h>

#define cs   10
#define dc   9
#define rst  8

TFT TFTscreen = TFT(cs, dc, rst);

// maximal darstellar mit textsize 2:
// {"title":"loremipsum","msg":"DA:74:1E:3C:40:48:75:41:1B:A3:CE:7A:17:C8:8F:B9:B3:7D:ED:C5:45:12:45:7"}
// maximal darstellbar mit textsize 1:
// {"title":"loremipsum","textsize":"1","msg":"Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet"}
// fix eingestellt:
// {"title":"key","fingerprint":"DA:74:1E:3C:40:48:75:41:1B:A3:CE:7A:17:C8:8F:B9:B3:7D:ED:C5"}
void setup() {
    Serial.begin(115200);
    TFTscreen.begin();

    TFTscreen.background(255, 255, 255);
    TFTscreen.setRotation(0);
    TFTscreen.stroke(0, 0, 0);

    Serial.println(F("USBJsonMonitor v0.01a"));
    Serial.println(F("READY"));


}

void clearTFT(){
    TFTscreen.fill(255, 255, 255);
    TFTscreen.rect(0, 0, TFTscreen.width(), TFTscreen.height());
    TFTscreen.stroke(0, 0, 0);
}

void loop() {
    if (Serial.available()) {
        char jsonInputBuffer[400];
        memset(jsonInputBuffer, 0x0, sizeof(jsonInputBuffer));
        uint16_t readBytes = 0;
        readBytes = Serial.readBytesUntil('\n', jsonInputBuffer, sizeof(jsonInputBuffer));
        if (readBytes != 0) {
            #ifdef DEBUG
            for(int i = 0; i < sizeof(jsonInputBuffer);i++){
                Serial.print(jsonInputBuffer[i]);
            }
            Serial.println();
            #endif

            StaticJsonBuffer<420> staticJsonInputBuffer;
            JsonObject &input = staticJsonInputBuffer.parse(jsonInputBuffer);
            if (input.success()) {
                if (input.containsKey("clear")) {
                    clearTFT();
                    Serial.println(F("OK"));
                    return;
                }
                if (input.containsKey("title") && input.containsKey("msg")) {
                    clearTFT();

                    TFTscreen.stroke(0, 0, 0);
                    TFTscreen.setTextSize(2);
                    TFTscreen.text(input["title"], 2, 2);
                    uint8_t textSize = 2;
                    if (input.containsKey("textsize")) {
                        textSize = input["textsize"];
                        TFTscreen.setTextSize(textSize);
                    } else {
                        TFTscreen.setTextSize(2);
                    }

                    if (textSize == 2) {
                        // maximal darstellare zeichen hängt von textsize ab
                        // {"title":"fgh","msg":"DA:74:1E:3C:40:48:75:41:1B:A3:CE:7A:17:C8:8F:B9:B3:7D:ED:C5:45:12:45:7"}
                        char msg[71];
                        if (strlen(input["msg"]) + 1 <= sizeof(msg)) {

                        }
                        memset(msg, 0x0, sizeof(msg));
                        strcpy(msg, input["msg"]);
                        char line[11];

                        uint8_t titleSize = 20;
                        uint8_t lineFeedSize = textSize * 10;

                        for (int i = 0; i < (strlen(msg)) / (sizeof(line) - 1); i++) {

                            memset(line, 0x0, sizeof(line));

                            for (int j = 0; j < sizeof(line) - 1; j++) {
                                line[j] = msg[i * (sizeof(line) - 1) + j];
                            }
                            TFTscreen.text(line, 2, i * lineFeedSize + titleSize );
                        }

                        if ((strlen(msg) + 1) % (sizeof(line) - 1) != 0) {

                            int i = (strlen(msg)) / (sizeof(line) - 1);
                            memset(line, 0x0, sizeof(line));
                            for (int j = 0; j < sizeof(line) - 1; j++) {
                                line[j] = msg[i * (sizeof(line) - 1) + j];
                            }
                            TFTscreen.text(line, 2, i * lineFeedSize + titleSize );
                        }
                        Serial.println(F("OK"));
                        return;

                    }
                    if (textSize == 1) {

                        char msg[295];
                        if (strlen(input["msg"]) + 1 <= sizeof(msg)) {

                        }
                        memset(msg, 0x0, sizeof(msg));
                        strcpy(msg, input["msg"]);
                        char line[22];

                        uint8_t titleSize = 20;
                        uint8_t lineFeedSize = textSize * 10;

                        for (int i = 0; i < (strlen(msg)) / (sizeof(line) - 1); i++) {

                            memset(line, 0x0, sizeof(line));

                            for (int j = 0; j < sizeof(line) - 1; j++) {
                                line[j] = msg[i * (sizeof(line) - 1) + j];
                            }
                            TFTscreen.text(line, 2, i * lineFeedSize + titleSize );
                        }

                        if ((strlen(msg) + 1) % (sizeof(line) - 1) != 0) {

                            int i = (strlen(msg)) / (sizeof(line) - 1);
                            memset(line, 0x0, sizeof(line));
                            for (int j = 0; j < sizeof(line) - 1; j++) {
                                line[j] = msg[i * (sizeof(line) - 1) + j];
                            }
                            TFTscreen.text(line, 2, i * lineFeedSize + titleSize );
                        }

                        Serial.println(F("OK"));
                        return;


                    }
                }

                if (input.containsKey("title") && input.containsKey("fingerprint")) {

                    TFTscreen.stroke(0, 0, 0);
                    TFTscreen.setTextSize(2);

                    char fingerprint[60];
                    memset(fingerprint, 0x0, sizeof(fingerprint));
                    strcpy(fingerprint, input["fingerprint"]);

                    if (strlen(fingerprint) == sizeof(fingerprint) - 1) {
                        clearTFT();

                        TFTscreen.text(input["title"], 2, 2);

                        char pair[10];
                        for (int i = 0; i < sizeof(fingerprint) / sizeof(pair) + 1; i++) {

                            memset(pair, 0x0, sizeof(pair));
                            if (i != (sizeof(fingerprint) / sizeof(pair))) {
                                for (int j = 0; j < sizeof(pair) - 1; j++) {
                                    pair[j] = fingerprint[i * (sizeof(pair) - 1) + j];
                                }
                            } else {
                                for (int j = 0; j < sizeof(fingerprint) - i * (sizeof(pair) - 1); j++) {
                                    pair[j] = fingerprint[i * (sizeof(pair) - 1) + j];
                                }
                            }
                            TFTscreen.text(pair, 4, (i + 1) * 20 + 2);
                        }
                        Serial.println(F("OK"));
                        return;
                    }

                }
            }
            Serial.println(F("ERROR"));
        }
    }
    delay(1);
}