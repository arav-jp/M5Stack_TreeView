#ifndef _M5UNITENCODER_H_
#define _M5UNITENCODER_H_

#include <M5Stack.h>

/* https://github.com/m5stack/M5Unit-Encoder/ */
class M5UnitEncoder
{
public:
  uint16_t msecHold = 300;

  bool update();

  int8_t rawValue() const { return _raw; }
  bool wasUp()      const { return _upDown > 0; }
  bool wasDown()    const { return _upDown < 0; }

  bool wasClicked()  const { return _oldPress == 1 && _press == 0; }
  bool wasHold()     const { return _oldPress == 1 && _press == 2; }
  bool isHolding()   const { return _oldPress == 2 && _press == 2; }

  bool isPressed()   const { return _press; }
  bool isReleased()  const { return !_press; }
  bool wasPressed()  const { return !_oldPress && _press; }
  bool wasReleased() const { return _oldPress && !_press; }
  bool pressedFor(uint32_t ms)  const { return (_press  && _time - _lastChangeButton >= ms); }
  bool releasedFor(uint32_t ms) const { return (!_press && _time - _lastChangeButton >= ms); }
  bool rotatedFor(uint32_t ms)  const { return (_time - _lastChangeEncoder >= ms); }
  bool lastUsed(uint32_t ms)  const { return (_time - _lastUsed >= ms); }
  void clearUsed() { _lastUsed=_time; }

  void setLEDColor(uint8_t index, uint32_t color);

private:
  void writeBytes(uint8_t addr, uint8_t reg, uint8_t* buffer, uint8_t length);
  void readBytes(uint8_t addr, uint8_t reg, uint8_t* buffer, uint8_t length);
  signed short int getEncoderValue();
  bool getButtonStatus();
  int8_t _addr = 0x40;
  int8_t _encoder_reg = 0x10;
  int8_t _button_reg  = 0x20;
  int8_t _led_reg     = 0x30;
  int8_t _raw = 0;
  int8_t _raw_z1 = 0;
  int8_t _button = 0;
  int8_t _upDown = 0;
  int8_t _oldUpDown = 0;
  uint8_t _press = 0;     // 0:release  1:click  2:holding
  uint8_t _oldPress = 0;
  uint32_t _time = 0;
  uint32_t _lastChangeButton = 0;
  uint32_t _lastChangeEncoder = 0;
  uint32_t _lastUsed = 0;
};

#endif

extern M5UnitEncoder UnitEncoder;

