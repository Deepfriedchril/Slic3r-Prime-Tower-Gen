# Slic3r-Prime-Tower-Gen: CURRENTLY ONLY FOR CURA
Configurable prime tower for Slic3r: WIP

Parsing CURA has more current focus mainly because I want to use my 2:1 extrusion now and because it already generates the prime tower. Cura just handles single nozzle extrusion poorly.


*note*
I have not tested this with mutiple parts nor have I extensivly tested it with one part. Use at your own risk. Cura 2.6 is currently in beta testing so some problems may come from that.

The setting dipicted below are used with an E3Dv6, so they may work for others depending on you physical set up.

**PLEASE** watch your printer closely, even compare the original gcode to the modified one before printing.

*I am not responsible for any misfourtions due to the use of this program and your printer. By using this, you agree to assume full respoinsibility.*


## How to use: CURA 2.6.0
Currently this works much better than the standard Cura rutines.

You need the prime tower enabled, as well as a few tags I use to parse the gcode listed below.
After gcode generation, open up a terminal and run the program with your gcode as a single argument.
It'll output a temp file, just rename it and change the ending to .gode for now.
Load that up and hit print.

### Machine settings

![][help1]

Add ";START" to the *BOTTOM* of your start gcode.

    G28 ;Home
    G1 Z15.0 F6000 ;Move the platform down 15mm
    G92 E0
    G1 F200 E3
    G92 E0
    ;START

Add ";END" to the *TOP* of your end gcode.

    ;END
    M104 S0
    M140 S0
    G92 E1
    G1 E-1 F300
    G28 X0 Y0
    M84


### Extruder 1&2

![][help2]

Add ";MATBEGIN" to the *TOP* of the extruder end gcode.

    ;MATBEGIN
    G91             ; Relative moves.
    G1 E1 F500      ; Quicky extrude about the length of your melt zone.
    G1 E-2 F1000    ; Do a quick retraction a little shorter than your melt zone. This is to avoid stringing.
    G4 P2500        ; Wait 2.5 seconds so that filament can set.
    G1 E-108 F 8000 ; Quickly pull filament out of the Y-splitter.
    G90             ; Abolute moves.
    G92 E0          ; Set extruder position to 0


add ";MATEND" to the *BOTTOM* of the extruder start gcode.

    G91             ; Relative moves.
    G1 E105 F2000   ; Quickly load next material 95%.
    G1 E4.5 F2000   ; Slowly load last 5%. -0.5mm as to not put anything on the print, just make sure to account for it in the prime tower
    G90             ; Abolute moves.
    G92 E0          ; Set extruder position to 0
    ;MATEND


### Retraction settings


![alt text][help3]

Set nozzle switch retraction distance to 0.

Set standby temp to the normal temp of your other extruder.

Be sure to do this for both extruders.




[help1]: https://github.com/Deepfriedchril/Slic3r-Prime-Tower-Gen/blob/master/howto/1printermain.PNG "help1"
[help2]: https://github.com/Deepfriedchril/Slic3r-Prime-Tower-Gen/blob/master/howto/2extrudersetting.PNG
[help3]: https://github.com/Deepfriedchril/Slic3r-Prime-Tower-Gen/blob/master/howto/3retractsetting.PNG
