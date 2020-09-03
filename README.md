# Mapper and its applications
Mapper is an easy to use command line utility for UNIX systems that can tell you the power level (https://en.wikipedia.org/wiki/Received_signal_strength_indication) of a specified mac address (or multiple addresses) from the device that Mapper is being run on. 
This is intended for users of Kali Linux, specifically the ARM version of Kali running on raspberry pis, however it can probably run on any linux system as long as it meets the requirements listed below.
     
This software can be used for tracking devices, and therefore the people using them. (especially mobile phones) Here are a couple of examples on what you can use this program for:
Say you wanted to tell if your roommate was at your apartment. (I would recommend asking for their consent before doing this) You could run Mapper on a server with a wifi card, or perhaps a raspberry pi 0w, and set it up such that it looks for your roommate's phone's mac address. If your roommate is home, Mapper would find their address, and it writes that it found it to a file. Then you can set up a web server that prints that file to a website. Now if anyone goes to http://example.com/ they can see if your roomate is at home.
Say you wanted to turn on the coffee maker when you got home. You could hook up a raspberry pi to continously run this program. As soon as your phone attempts to connect to your wifi, the program will list your device, which can write to a file, and then turn on one of the many GPIO pins on the pi to control a relay that controls a coffee maker. This way as soon as you got home, your coffee maker would turn on. 
     
 There are many things you can do with one instance of this program, but I believe the most exiting thing you can do is if you run Mapper on multiple devices simultaniously and send the information to a central server. 
If an institution (say a city government) set up devices running Mapper around a city, they could track large scale population movements, as almost everyone nowadays carries wifi capible devices with them, and there is wifi almost everywhere. Not only that but if the central server stored the individual mac addresses, the city's police department could find the location of a specific device to a relitivly small location, (probably within 50 feet or so) anywhere where they have these devices set up.
     
# Theory
When a device sends or receives data through wifi, it must send information to the router. Since the connection is wireless, there is no direct connection from the device to the router. So, the device must broadcast the frames of data that it wants to send over the wifi channel, and those frames include metadata including the mac address of the device.  From those frames, the wireless interface can also determine the power of the signal, and power  is logarithmically related to distance per the inverse square law.

First the program puts the specified wireless interface into monitor mode, then extracts information from airodump-ng (a wireless packet capture program) to find specific mac addresses. 
 
 # Usage:
     $ mapper [options]
     
# Important information
This program puts your wireless interface into monitor mode, so you will not be able to connect to the internet with the specified wireless interface while the program is running. It will however attempt to put the interface out of monitor mode before the program is finished, however if it crashes midway, or if there is an interrupt, you will have to run the following command in order to take your interface out of monitor mode:
     
     $ sudo airmon-ng stop <interface name>
     
     Note that your interface name may be changed, (wlan1mon instead of wlan1 for instance), so if you are unsure you can run "ifconfig" to get your interface name.
     You may need to put your interface up in order to use wifi again,
     
     $ sudo ifconfig <interface name> up

# Minimum Requirements
     Obviously you will need a wireless interface, this will not work on Ethernet.
     You must have aircrack-ng installed (https://www.aircrack-ng.org/) and you must run this program (Mapper) as root.
     You also must have a file that has the known devices list format (outlined below).
     
Mapper can be run with no options! However, the program will make a couple of assumptions and will not run properly if these requirements are not met:

     1) ifconfig must work on your system.
     2) Desired wireless interface must begin with "wlan" (if there is more than one the program will pick the first one that appears in ifconfig).
     3) A file called known.txt (with the right formant) exists, and is in the same directory as where the program is being run from. 
          
# Options (case sensitive)

     -m <mac address>, --mac <mac address>
          Specify a single mac address to search for. This has to be in colon format (FF:FF:FF:FF:FF:FF). (This does not work with the -k option)
  
     -k <path to known devices list>, --known <path to known devices list>
          Known devices list format outlined below. (This does not work with the -m option)
     
     -h, --help
          Prints out the help page (this page)
     
     -i <interface name>, --interface <interface name>
          Overrides default wireless interface.
          
     -c <Path to config file (format outlined below)>, --config <Path to config file>
          Will read options from the config file instead of reading command line arguments.   
  
     -u, --unknown
          Will print information regarding known addresses, as well as information about all unknown mac addresses that airodump-ng found. 
          You should use this when you want the information regarding all devices nearby.
          
     -s, --simple
          Prints in simple format for use in low level programs instead of the normal json format
     
     -v <vendor file list (Not required)>, --vendor <vendor file list (Not required)>
          Uses grep to search for the manufacturer's name and then Prints it for each mac address.
          By default this will use "/usr/share/nmap/nmap-mac-prefixes" but if you have this file somewhere else, or another file with the same format you may specify it here.
          

# Known Devices List file format

     The list stores the mac addresses in the following format {

     0n= Name of a device here with or without spaces
     0m= FF:FF:FF:FF:FF:FF

     1n= Name of another device here
     1n= 00:00:00:00:00:00

     ...

     } > known.txt

You may rename this to be anything but I will continue to call this file known.txt.
Also Mapper will attempt to locate "known.txt" in the same directory as where the executable was run from if no input file is specified with the flag "-k"

# Config file format
You should put in your arguments just as if you would add options normally.
{
-option1 <param1> -option2 <param2>
}

# Examples and default output format

$ mapper -m FF:FF:FF:FF:FF:FF
---
**output**:
{
     Mac: FF:FF:FF:FF:FF:FF
     Power: -41
}
---

If the device is not found:
---
**output**:
{
}
---


$ mapper -u -k sample_known.txt -i wlan0

---
**output**:
{
     "devices" : 5
     
     "unkown" : {
          "Mac" : "FF:FF:FF:FF:FF:FE"
          "Power" : -40

          "Mac" : "FF:FF:FF:FF:FF:FE"
          "Power" : -19

          "Mac" : "FF:FF:FF:FF:FF:FD"
          "Power" : -87

          "Mac" : "FF:FF:FF:FF:FF:FC"
          "Power" : -56
     }
     "known" : {
          "Loris' ubuntu laptop"{ 
               "Mac" : "00:00:00:00:00:00"
               "Power" : -23
          }
     }
}
---


sample_known.txt{
0n= Loris' ubuntu laptop
0m= 00:00:00:00:00:00

1n= ESP32 light controller
1m= FF:FF:FF:FF:FF:FF
}

# Simplified output format (-s --simple)




