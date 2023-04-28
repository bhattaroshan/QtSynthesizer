# QtSynthesizer

The application uses CMakeLists.txt to compile for your OS. You'll need to install QtCharts and QtMultimedia to run the application. 

The application can help you experiment with Sinusoidal waves. Currently you can experiment with these attributes of signal :----
- Frequency 
  - You can go as low or as high as you want with the frequency.
- Amplitude
  - the range for amplitude is [0,1]
- Harmonics 
  - right now it just adds up both odd and even harmonics. Adding harmonics to a lot of signals might slow down the app a little bit as I'm recalculating everything in the signal even if you just change one track. This will be fixed very soon. Also the option to add only even or odd harmonics will be added very soon.
- Envelope 
  - You can experiment with ADSR. There is no filed to change S. Software automatically figures out the S once you set Attack, Decay and Release.
  
<img width="1440" alt="Screen Shot 2566-04-28 at 11 06 48 PM" src="https://user-images.githubusercontent.com/4207226/235213350-e875e1bc-718e-4200-8b8a-b49a4ed3fc79.png">

