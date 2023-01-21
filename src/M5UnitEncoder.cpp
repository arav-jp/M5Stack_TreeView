#include <M5UnitEncoder.h>

M5UnitEncoder UnitEncoder;

/*! @brief Write a certain length of data to the specified register address. */
void M5UnitEncoder::writeBytes(uint8_t addr, uint8_t reg, uint8_t *buffer,
                              uint8_t length) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  for (int i = 0; i < length; i++) {
    Wire.write(*(buffer + i));
  }
  Wire.endTransmission();
}

/*! @brief Read a certain length of data to the specified register address. */
void M5UnitEncoder::readBytes(uint8_t addr, uint8_t reg, uint8_t *buffer,
                             uint8_t length) {
  uint8_t index = 0;
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(addr, length);
  for (int i = 0; i < length; i++) {
    buffer[index++] = Wire.read();
  }
}

/*! @brief Read the encoder value.
    @return The value of the encoder that was read */
signed short int M5UnitEncoder::getEncoderValue() {
  uint8_t data[2];
  readBytes(_addr, _encoder_reg, data, 2);
  signed short int value = (signed short int)((data[0]) | (data[1]) << 8);
  return value;
}

/*! @brief Get the current status of the rotary encoder keys.
    @return 1 if the set was press, otherwise 0.. */
bool M5UnitEncoder::getButtonStatus() {
  uint8_t data;
  readBytes(_addr, _button_reg, &data, 1);
  return data;
}

/*! @brief Set the color of the LED (HEX). */
void M5UnitEncoder::setLEDColor(uint8_t index, uint32_t color) {
  uint8_t data[4];
  data[3] = color & 0xff;
  data[2] = (color >> 8) & 0xff;
  data[1] = (color >> 16) & 0xff;
  data[0] = index;
  writeBytes(_addr, _led_reg, data, 4);
}

bool M5UnitEncoder::update()
{
  _oldUpDown = _upDown;
  _oldPress = _press;
  _raw_z1 = _raw;

  _time = millis();
  _raw = getEncoderValue();
  _button = !getButtonStatus();

  if (_button != (0 != _oldPress)) {
    _lastChange = _time;
  }
  if (_button) {
    if (!_oldPress) {
      _press = 1;
    } else if (1 == _oldPress && (_time - _lastChange >= msecHold)) {
      _press = 2;
    }
  } else {
    _press = 0;
  }

  if (0<(_raw-_raw_z1) && (_raw-_raw_z1)<100 && _upDown==0) {
    _upDown = 1;
  } else if (-100<(_raw-_raw_z1) && (_raw-_raw_z1)<0 && _upDown==0) {
    _upDown = -1;
  } else {
    _upDown = 0;
  }

  if (_raw_z1 != _raw) {
    if (_raw_z1 > _raw) {
      setLEDColor(1, 0x000011);
    } else {
      setLEDColor(2, 0x111100);
    }
  } else {
    setLEDColor(0, 0x001100);
  }
  if (!_button) {
    setLEDColor(0, 0xC800FF);
  }

  return true;
}