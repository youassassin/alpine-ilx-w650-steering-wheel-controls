# Alpine iLX-W650 Steering Wheel Controls

An Arduino sketch that reads resistance-based steering wheel control buttons and translates them into wired remote commands for an Alpine iLX-W650 head unit.

## How it works

The steering wheel has three resistor-ladder button groups wired to analog pins A1, A4, and A6. When a button is pressed, the analog voltage changes to a specific level. The Arduino reads these voltages, compares them against calibrated thresholds, and sends the corresponding command to the head unit via a digital output pin (pin 5).

Each threshold constant represents the observed minimum ADC reading for that button during live testing. A `MARGIN` of 2 ADC counts is subtracted from each threshold at comparison time to account for slight reading variability at lower voltages.

### Button mapping

| Switch | Threshold constant | Observed min | Action |
|--------|--------------------|-------------|--------|
| SW1 | `SW1_TRACKUP_MIN` | 213 | Track up (seek+) |
| SW1 | `SW1_TRACKDN_MIN` | 198 | Track down (seek-) |
| SW1 | `SW1_VOLUP_MIN` | 171 | Volume up |
| SW1 | `SW1_VOLDN_MIN` | 121 | Volume down |
| SW2 | `SW2_SOURCE_MIN` | 219 | Source select (mode) |
| SW2 | `SW2_ENDCALL_MIN` | 203 | End call |
| SW2 | `SW2_VOICE_MIN` | 175 | Voice (Siri) |
| SW2 | `SW2_ANSWER_MIN` | 124 | Answer call |
| SW3 | `SW3_DISPLAY_MIN` | 222 | Display |


### Protocol

The `AlpineRemote` library sends commands over a single digital pin using a proprietary wired protocol that is structurally similar to the NEC IR protocol but with several differences.

**NEC vs Alpine comparison:**

| | NEC (IR) | Alpine (wired) |
|-|----------|----------------|
| Medium | Infrared (38kHz carrier) | Wired, DC logic level |
| Preamble | 9ms HIGH, 4.5ms LOW | 8ms HIGH, 4.5ms LOW |
| Address | 8-bit address + 8-bit inverse | 3 fixed start bytes (`0xD7`, `0xDB`, `0xAB`) |
| Command | 8-bit command + 8-bit inverse | 16-bit command (MSB first) |
| End | Implicit (stop bit) | 1 explicit end byte: inverse of last command bit + fixed `1010101` trailer |
| Bit clock | Variable width (562.5µs) | Fixed 500µs HIGH + 500µs LOW per bit |
| Repeat | Dedicated repeat frame | None |

In NEC, a `1` bit is distinguished from a `0` by pulse width (562.5µs vs 1.6875ms LOW after each HIGH pulse). Alpine instead uses a fixed 500µs HIGH/LOW clock for every bit, relying on the logic level of the HIGH pulse itself to encode the bit value.

The command is effectively 17 bits — the 17th bit is always the inverse of the 16th (last) command bit. The end byte is that 17th bit prepended to a fixed trailer `1010101`, giving either `0x55` or `0xD5`.

**Frame structure:**
```
[8ms HIGH] [4.5ms LOW] [0xD7] [0xDB] [0xAB] [16-bit command] [end byte]
```

### Supported commands

Volume up/down, mute, track up/down, preset up/down, source select, play/pause, band select, power, and Siri activation.

## Hardware

- Arduino Nano (ATmega328P, CH340 USB-serial) with external analog reference
- 2010 Toyota Corolla steering wheel control module (part no. 84250-02200)
- Output to Alpine iLX-W650 remote input via 3.5mm jack — D5 to tip, ground to ring/sleeve

### Steering wheel wiring

The Corolla steering wheel control module has three signal wires (sometimes labelled SW1/SW2/DISP or KEY1/KEY2 on aftermarket adapters):

| Arduino pin | Signal wire | Function |
|-------------|-------------|----------|
| A6 | SW1 / KEY1 | Volume, track |
| A4 | SW2 / KEY2 | Source, calls, voice |
| A1 | DISP | Display |

All three are resistor-ladder inputs — each button press pulls the line to a different voltage. A common ground from the steering wheel harness connects to Arduino GND.

<img src="steering%20wheel%20controls.png" width="300">

## Debug mode

Enable debug output by uncommenting `#define DEBUG` in `config.h`. This enables:
- Periodic serial output of raw ADC values (every 1s when no button is pressed)
- Command bitstream printing after each transmission

Serial runs at 115200 baud.

## VS Code setup

A `.vscode/c_cpp_properties.json` is included pointing IntelliSense at the Arduino AVR core headers (`Arduino.h`, etc.). Requires the [C/C++ extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools) by Microsoft.
