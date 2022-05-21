# homelab-monitor
Simple ESP8266 based project to monitor parameters of a homelab u
![IMG_7194](https://user-images.githubusercontent.com/33011042/169668444-64da5411-e8ff-4471-ba7d-41d0761bfa91.jpg)


![IMG_71941](https://user-images.githubusercontent.com/33011042/169668450-c634321f-282a-409d-b8f9-62c3ff821207.jpg)




This project is based on a ESP8266 NodeMCU board and a OLED Display and its main purpose is to show some parameters of my homelab.
The data is provided by NetData REST API 
I'm running the truenas operating system on my server and the Netdata plugin provides the data needded for the ESP.







<ul>The following explanation is for how to install netdata on Truenas but you can use whatever OS you want as long as it can provide the data you need. 
 
   <li>On your TrueNAS homepage select Plugins</li>
   
   ![2022-05-21 15 57 38](https://user-images.githubusercontent.com/33011042/169666097-465fde5e-0507-40b1-afa2-f23f2b8050cf.jpg)

   


  <li>Click on the IXsystems tab and select Comunnity</li>
   
   ![2022-05-21 15 59 04](https://user-images.githubusercontent.com/33011042/169666141-435f3471-c2de-43cd-ba6e-fd485bfec3f6.jpg)
   
   
   
  
  
  <li>Choose NetData and click Install</li>
  
![2022-05-21 16 01 52](https://user-images.githubusercontent.com/33011042/169666149-7f62da79-381d-448c-88dd-d03200bea4b7.jpg)



   <li>After the plugin is installed, look for the admin portal IP, in my case is <b>192.168.15.66:19999</b>. Copy and paste that ip on your browser followed by <b>/api/v1/charts</b></li>
   
    ![2022-05-21 16 19 56](https://user-images.githubusercontent.com/33011042/169666276-4ec5ed55-e939-451a-9ebd-29c4687c9c62.jpg)

  <li>This page shows all the data your server is providing NetData on a JSON format. Later you can access this page on your microcontroller and choose what data you want to display.</li>
  
  ![2022-05-21 16 27 39](https://user-images.githubusercontent.com/33011042/169666470-70a4cbf9-16be-4a99-a5c9-a818d54cd38d.jpg)
  
  </ul>
  

  <ul>Wiring up the NodeMcu board with the OLED Display
  
  <li>Simply connect the VCC of the display to the 3.3v pin of the ESP, GND to GND, SLC to pin D2 and SDA to pin D1</li>
  
  ![2022-05-21 17 06 13](https://user-images.githubusercontent.com/33011042/169667386-976e9442-a084-41a3-a2db-ccd39026687f.jpg)
  </ul>
  
  
  On the ESP8266 code: 
  
  Configure your WiFi SSID and Password
  ![image](https://user-images.githubusercontent.com/33011042/169668161-fc85bfd3-48a4-4ba7-8759-fa3839590a9d.png)
  
  
  Use #define followed by a variable name and the URL that NetData provided you to whatever parameter you want to read
  
  ![image](https://user-images.githubusercontent.com/33011042/169668221-d2f7628a-e39d-4c8f-b518-a043a78d81a1.png)
  Remember to change the IP to your actual IP.
  
  Uplod the code to your ESP.

  
  
  

  

  
  
  
