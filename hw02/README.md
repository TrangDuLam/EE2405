# Markdown 02

## 1. Input Stage

### Controlling

      The button 1 is assigned to pin PB_2 (D8) and its function is to increase the frequency.

      The button 2 is assigned to pin PB_1 (D6) and its function is to decrease the frequency.  
   
      The button 3 is assigned to pin PB_0 (D3) and its function is to output the waveform.

      All the circuit was made up by the instruction provided.

### Value of the frequency

      The magnitude of the frequency step is set to be 50 Hz. There is seven step to choose. Thus, the highest frequency is 350 Hz.

## 2. Waveform generation

      The output pin of the wavefrom is assigned to PA_4 (D7)

      The remainder of the summation of my student ID is 8. Thus, we could derive the waveform in mathematic relation
      
            * for time < 0.8 unit time, the relation would be Vout = (peak / 0.8) * t
   
            * for time > 0.8 unit time, the relation would be Vout = -peak*(t-1)/0.2

      The peak value is 3.0 volt. In the code, its value is 0.91f. (f = 3.3 V)

## 3. Display

### Selecting Stage

      Once pressing the "up" or "down" button, the value would vary from 0 to 7 shown in green color on the uLCD. To pretend to below 0 or above 7, I do the limitation process in the decision loop. 

### Output Stage

      Once pressing the output button, the board would output the waveform in assigned pin. And the number of the mode selected would be shown in red color.

## 4. FFT

      Once the board output the signal into the RC network, the terminal would start the FFT.py to sample and do the FFT process. After that, the figure would be printed and stored.
