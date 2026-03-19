# Alpine iLX-W650 Steering Wheel Controls

An Arduino sketch that reads resistance-based steering wheel control buttons and translates them into wired remote commands for an Alpine iLX-W650 head unit.

## How it works

The steering wheel has three resistor-ladder button groups wired to analog pins A1, A4, and A6. When a button is pressed, the analog voltage changes to a specific level. The Arduino reads these voltages, compares them against calibrated thresholds, and sends the corresponding command to the head unit via a digital output pin (pin 5).

### Button mapping

| Switch | Voltage range | Action |
|--------|--------------|--------|
| SW1 | >213 | Track up (seek+) |
| SW1 | 198–213 | Track down (seek-) |
| SW1 | 171–198 | Volume up |
| SW1 | 121–171 | Volume down |
| SW2 | >219 | Source select (mode) |
| SW2 | 175–219 | Voice (Siri) |
| SW2 | 124–175 | End call |
| SW2 | <124 | Answer call |
| SW3 | >222 | Display |

### Protocol

The `AlpineRemote` library sends commands using a proprietary Alpine wired remote protocol over a single digital pin:
- 8ms HIGH preamble
- 4.5ms LOW gap
- 3 start bytes (`0xD7`, `0xDB`, `0xAB`)
- 2 command bytes (16-bit, MSB first)
- 1 end byte (chosen based on MSB of command as a simple checksum)

Each bit is clocked with a 500µs HIGH/LOW pulse.

### Supported commands

Volume up/down, mute, track up/down, preset up/down, source select, play/pause, band select, power, and Siri activation.

## Hardware

- Arduino (with external analog reference)
- Steering wheel resistor-ladder button clusters connected to A1, A4, A6
- Output signal wire to Alpine remote input on pin 5

## Debug mode

Define `DEBUG` to enable periodic serial output of raw ADC values when no button is pressed. Command bitstreams are always printed to serial at 115200 baud.
