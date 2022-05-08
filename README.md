# ESP32 Drum Machine

There are two parts to this project. The ESP32 code, which allows the user to program a drum beat, and Javascript code, which receives serial data from the ESP32 and plays the programmed drum beat using Howl.js library. The Javascript part is hosted here: https://brian.ma/ESP_DrumMachine/ 

The Drum Programmer part is done on the ESP32. Here, a graphical representation of a single bar drum pattern is displayed on the ESP32's LCD screen. The pattern consists of a 8-by-3 grid of dots. The 8 dots horizontally represent all the 8th notes in a single bar of a 4/4 measure. The 3 dots vertically represent the 3 main instruments of a drum set -- hi-hat, snare drum, and bass drum. The user uses a joystick to control the position of a "cursor". The user can then use the button to "toggle" that dot. If the dot is filled, it means that the instrument (e.g. snare drum) is played at that moment in time (e.g. the "downbeat of beat 3"). By filling a bunch of dots, the user can program a drum pattern. To play the drum pattern, the user can "unpause" using the button on the ESP. When "unpaused" a vertical line representing the current time position moves rightwards, and each time it reaches a group of 3 dots it sends a signal via serial to the Javascript system which plays the corresponding drum hit. The signal is structured as follows: "{ABC}" where each letter is either a 1 or a 0, representing if the specific instruments plays at that moment in time, or not. The potentiometer controls the BPM value, which speeds up or slows down the drum beat.

### ESP32 Drum Machine interface and controller
![5](https://user-images.githubusercontent.com/6265129/167313745-7e2e44c1-e441-4909-a416-9cf37426d177.jpg)

![enclosure](https://user-images.githubusercontent.com/6265129/167313837-29a1b026-08f5-4eb1-a2d3-95b1cea4d4db.jpg)

## How to run ESP32 Drum Machine
To run ESP32 Drum Machine, simply go to https://brian.ma/ESP_DrumMachine/ in a browser that supports web serial e.g. Google Chrome. The system accepts data in a string format with 3 data fields so you can build your own drum beat programmer:
```
{ABC}
```
Where A,B,C are 1 or 0 representing if the Hi-Hat, Snare, and Bass Drum respectively should play at that moment.

When you open the website, click on the top left corner to open a list of serial ports. Once you select a port, the system can then receive drum sound data.

#### Controls
Pressing the Button toggles the circle under the cursor.
Moving the joystick moves the cursor position.
Turning the potentiometer increases or decreases the BPM.
Pressing the integrated ESP32 Button toggle Play/Pause.

## How to run ESP32 code:
The code to run is under the DrumProgrammer folder. 

We are using the Arduino IDE. To enable IDE support for the ESP32, first open up Preferences and under "Additional Boards Manager URLs", add the following URL:
```
https://dl.espressif.com/dl/package_esp32_index.json
```
![BoardsManagerURL](https://user-images.githubusercontent.com/6265129/153997561-184baff3-dad6-4699-b3ea-dfbc9214f8ea.jpg)

Then, go to "Tools/Board:/Board Manager" and install ESP32:

![BoardsManager](https://user-images.githubusercontent.com/6265129/153997769-d04a40cc-fc14-4832-a115-e32f032be1a6.jpg)

Then, go to "Tools/Manage Libraries" and search for tft_eSPI. Install it.

![ManageLibs](https://user-images.githubusercontent.com/6265129/153997596-e524be05-fd41-4741-9025-56ad5be9ab33.jpg)

After that, go to "Tools/Board:" and select "TIGO T1".

![BoardSelect](https://user-images.githubusercontent.com/6265129/153997616-e5988c80-6d34-4566-b800-3d8c03f9ffd7.jpg)

Now, simply connect your board and click upload.
