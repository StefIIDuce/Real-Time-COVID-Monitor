# Real-Time-COVID-Monitor
This project will monitor the COVID situation online by extracting data from a site.
The system will also extract its real time clock data from another site and display both on a 0.96 OLED screen.
To realise this project an account at thingspeak.com is required to generate the http get request strings.
Other components needed are an ESP32 microcontroller(Not tested with other modules, should work though.) an 0.96 OLED screen. 
The OLED needs to be connected with the I2C SDA(D21) and SCL(D22) lines and powerd with 3.3V.
The sites used for extracting the data from are:
https://www.worldometers.info/coronavirus/
https://www.worldtimeserver.com/current_time_in_BE.aspx?city=Brussels
