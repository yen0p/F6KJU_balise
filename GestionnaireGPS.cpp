#include "GestionnaireGPS.h"
#include <math.h>

GestionnaireGPS::GestionnaireGPS(HardwareSerial& serialPort) : _gpsSerial(serialPort) {
  _gpsSerial.begin(9600);
  strcpy(_locator, "......");
}

void GestionnaireGPS::update() {
  while (_gpsSerial.available() > 0) {
    if (_gps.encode(_gpsSerial.read())) {
      if (_gps.location.isUpdated() && hasValidLocation()) {
        calculateLocator();
      }
    }
  }
}

bool GestionnaireGPS::hasValidTime() const { return _gps.time.isValid(); }
bool GestionnaireGPS::hasValidLocation() const { return _gps.location.isValid() && _gps.location.age() < 2000; }
uint8_t GestionnaireGPS::getSeconds() const { return _gps.time.second(); }
double GestionnaireGPS::getLatitude() const { return _gps.location.lat(); }
double GestionnaireGPS::getLongitude() const { return _gps.location.lng(); }
const char* GestionnaireGPS::getLocator() const { return _locator; }

void GestionnaireGPS::calculateLocator() {
  lon += 180; lat += 90;
  _locator[0] = (char)('A' + floor(lon / 20));
  _locator[1] = (char)('A' + floor(lat / 10));
  _locator[2] = (char)('0' + floor(fmod(lon, 20) / 2));
  _locator[3] = (char)('0' + floor(fmod(lat, 10)));
  _locator[4] = (char)('a' + floor(fmod(lon / 2, 1) * 24));
  _locator[5] = (char)('a' + floor(fmod(lat, 1) * 24));
  _locator[6] = '\0';
}