# Sound reacting led
This will light on a RGB led (or a band of led accross transitor) according the
input signal as ambiant sound. It use the atmel attiny85 microprocessor
Signal is capted by microphone. A fast fourier transform is done, using sinus
table (Notice how it is amazing to do it quickly with an attiny85).
According the frequency of the main harmonique, a color is displayed.
