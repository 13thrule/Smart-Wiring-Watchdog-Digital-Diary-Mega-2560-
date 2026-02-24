# Smart Wiring Watchdog & Digital Diary (Arduino Mega 2560)

<!-- Badges -->
<p align="center">
  <img src="https://img.shields.io/badge/Arduino-Mega%202560-blue" alt="Arduino Mega 2560">
  <img src="https://img.shields.io/badge/License-MIT-green" alt="MIT License">
  <img src="https://img.shields.io/badge/Status-Stable-brightgreen" alt="Stable">
  <img src="https://img.shields.io/badge/Display-SSD1306%20OLED-yellow" alt="SSD1306 OLED">
</p>

## Infographic

Visual overview of the system:

![Smart Wiring Watchdog Infographic](SmartWiringWatchdog/infographic.png)

## Example Video

See the tool in action:

[Download the example video](example_video.mp4)

---

## Professional Continuity Mapping & Harness Security System

The Smart Wiring Watchdog is a high-utility diagnostic tool designed for technicians, loom builders, and panel wiremen. It replaces the tedious "multimeter beep test" with a smart, visual, and secure interface that identifies pins, logs them into a live diary, and monitors them for intermittent failures.

---

## The Core Concept: The "Blue Rail" Scout

The power of this system lies in the Common Ground (Blue Rail) logic. By connecting a single "Scout Wire" to the breadboard's blue ground rail, you create a mobile probe.

- **Touch a wire:** The system completes a circuit through the Scout Wire back to the Arduino.
- **Result:** The OLED instantly identifies the exact pin number, eliminating the need for manual mapping or guessing.

---

## Key Features

- ⚡ **Live Pin Detection:** Scans pins 2 through 53. Touch any wire to the Scout Probe to see its port number in high-contrast text.
- 🛡️ **Watchdog Mode:** "Lock" a pin into security mode. If the connection breaks for even a millisecond, the system triggers a Red Alert.
- 🔊 **Auto-Reset Alarm:** The siren screams if a wire is pulled but silences immediately if reconnected—perfect for testing "loose" crimps.
- 📔 **Wiring Library (RAM Diary):** Automatically remembers the last 10 pins you successfully mapped.
- 🔐 **Triple-Click Hidden Menu:** Rapidly press the ARM button three times to lock the screen on the Wiring Library—allowing you to document the loom hands-free.

---

## Hardware Setup

### Pin Mapping

| Component   | Pin  | Function                                 |
|-------------|------|------------------------------------------|
| Buzzer      | 41   | Audible Siren & Feedback                 |
| Red LED     | 49   | ALARM: Connection Broken                 |
| Yellow LED  | 47   | DETECTED: Scouting Pin                   |
| Green LED   | 43   | SECURE: System Idle / Armed              |
| Button 1    | 50   | ARM: Single-click (Lock) / Triple-click (Library) |
| Button 2    | 48   | CLEAR: Disarm / Exit Library / Reset Scan|
| OLED Screen | 20,21| I2C Display (128x64 SSD1306)             |

---

## Installation & Use

1. **Flash the Script:** Upload the Master Script to an Arduino Mega 2560.
2. **Common Ground:** Connect the Arduino GND to the breadboard's Blue Rail. Plug your Scout Wire into this rail.
3. **Scout:** Touch the probe to any wire in your loom. The screen will display PORT #XX.
4. **Lock:** Press the ARM (50) button. The light turns Green. The port is now "Guarded."
5. **Test:** Shake or pull the wire. If the connection is bad, the Red LED and Buzzer will trigger.
6. **Review:** Triple-click ARM (50) to see the Wiring Library of all pins mapped in this session.
7. **Reset:** Press CLEAR (48) to wipe the current guard and return to scouting.

---

## Technical Why: INPUT_PULLUP

This system utilizes the Mega’s internal INPUT_PULLUP resistors. The pins are held at a logical HIGH (5V) internally. The Scout Wire (connected to the Blue Rail/GND) pulls the pin LOW when touched. This is the safest and most reliable way to sense continuity without external interference.

---

## The "Scout Wire" Explained

Using the Blue Rail as the home base for your scout wire is the "secret sauce" that makes this whole system work. In electronics, we call this a Common Ground.

- **Setup:** Run a jumper from Arduino GND to the Blue Rail (minus '-' sign) on your breadboard. Plug your Scout Wire into this rail.
- **Why:** The Scout Wire completes the circuit, allowing the Arduino to detect which pin is being touched. Without it, the system cannot "see" your wires.
- **Speed:** Instead of using a multimeter, you simply tap each wire with the Scout Wire—turning a 30-minute labeling job into a 30-second walkthrough.

> "The Scout Wire is the 'Return Path.' By connecting the component to the Blue Rail (GND), you complete the circuit. This allows the Arduino to detect the specific pin number and log it into the Wiring Diary instantly. It is the fastest way to verify a wiring loom without using a multimeter."

---

## Customizing Hardware Pin Assignments

You can set the hardware (LEDs, buzzer, buttons) to whatever pins you want—just change the pin numbers in the script:

```
const int RED = 49, YEL = 47, GRN = 43, BZ = 41;
const int BTN_ARM = 50, BTN_DISARM = 48;
```

- Update these values to match your wiring setup.
- Make sure your chosen pins are not used for I2C (20, 21) or other reserved functions.
- After changing the pin numbers, upload the script to your Arduino Mega.

This flexibility allows you to adapt the tool to different hardware layouts or custom panel designs.

---

## Additional Details & Clarifications

### Pin Scanning
- Only digital pins 2–53 are scanned, except those reserved for hardware (20, 21 for I2C, 41 for buzzer, 43 for green LED, 47 for yellow LED, 48 for clear button, 49 for red LED, 50 for arm button).
- **Analog pins (A0–A15) are not scanned** because the script is designed for digital continuity detection using INPUT_PULLUP. Analog pins on the Mega are not true digital pins and do not support INPUT_PULLUP in the same way, so they cannot reliably detect continuity with this method.

### Wiring Diary
- The Wiring Diary keeps the last 10 unique pins mapped during your session.
- Pressing CLEAR (Button 48) resets the current guard and alarm, but **does not erase the Wiring Diary**.

### Triple-Click Feedback
- Triple-clicking the ARM button (50) triggers two tones for audible feedback and locks the display on the Wiring Library.

### Display Pages
- The OLED cycles between three pages: Monitor, Library, and Registry.
  - **Monitor:** Shows status messages (READY, FOUND, SECURED, ALARM) and pin numbers.
  - **Library:** Displays the last 10 mapped pins.
  - **Registry:** Shows hardware pin assignments and the currently active pin.

### Alarm & Sound
- The buzzer alternates tone when the alarm is active.
- Entering the Wiring Library triggers a chirp for confirmation.

### Status Indicators
- The display shows:
  - **STATUS: READY** when idle
  - **STATUS: FOUND** when a pin is detected
  - **STATUS: SECURED** when a pin is locked
  - **! ALARM !** when a secured pin is removed

---

## Wiring Workflow & Pin Assignment

**Important:** The Scout Wire is a diagnostic tool to help you identify and verify the correct pin for each wire in your harness or project. After using the Scout Wire to find and confirm a pin:

- You still need to wire your actual component (switch, sensor, relay, etc.) to the identified pin on the Arduino Mega.
- The Scout Wire is for testing and mapping only—it does not replace the final wiring step.
- Once you know which pin corresponds to your wire, you can update the script to document the new pin assignment and its function.
- Repeat this process for each wire/component until your project is fully mapped and connected.

This workflow ensures every wire is correctly identified, mapped, and documented before final assembly, reducing errors and making troubleshooting easier.

---

## Troubleshooting

1. **Button Always "Active" (The Leg Trap)**
   - *Cause:* Most 4-pin tactile pushbuttons are connected in pairs. You may have the wires on legs that are "always connected."
   - *Fix:* Rotate the button 90 degrees or move your wire to the diagonal leg. The system expects a LOW signal only when the button is physically pressed.

2. **OLED Screen is Blank**
   - *Cause:* I2C Address mismatch or wiring.
   - *Fix:* Ensure your SDA/SCL wires are on pins 20 and 21. If it still doesn't light up, check the code line `display.begin(SSD1306_SWITCHCAPVCC, 0x3C);`. Some screens use address 0x3D.

3. **No Detection on Scout Wire**
   - *Cause:* Floating Ground.
   - *Fix:* Ensure the Blue Rail is physically connected to one of the GND pins on the Arduino Mega.

4. **"Ghost" Detections**
   - *Cause:* Static or long unshielded wires.
   - *Fix:* The script uses INPUT_PULLUP to minimize noise, but if you are in a very "noisy" electrical environment, ensure your scout wire is making a solid, clean contact.

**Pro-Tip:** The system is programmed to give a short audio chirp when a pin is successfully "Locked." If you hear the chirp but the light doesn't change, check your LED polarity (Long leg = Positive/Pin, Short leg = GND).

---

## Final GitHub Checklist

- **The Code:** Paste your "Master Script" into a file named Wiring_Watchdog.ino.
- **The Images:** If you have photos of your build, upload them to an images folder in your repo.
- **The License:** Most people use the MIT License—it basically says "Anyone can use this, just don't sue me if you wire something wrong."

---

## Credits & License

- **Developer:** [Your Name/GitHub Name]
- **Platform:** Arduino Mega 2560
- **Display:** SSD1306 OLED via I2C
- **Libraries:** Adafruit_SSD1306, Adafruit_GFX

---

## Pro-Tip for GitHub

Take a short video or a photo of your board with the "ALARM!" screen showing and upload it to the repo. GitHub will display the image, which makes people much more likely to click on it!
