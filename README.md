# NetPhone
CPE490 Net Phone Project
This project is an attemp at building a network streaming program.
It can be run on Mac OSX for sure and potentially other UNIX based systems.

In order to run this code you will need two command prompts, one for sending the 
data and one for receiving it. These can be on the same computer or on two different
computers on the same network.

To stream data:
1. cd into NetPhone/live
2. run "make" (without the quotes) this will compile all of the required programs
3. cd into NetPhone/live/testProgs
4. run "./NetPhoneSend <filetosend>" if you do not specify a file to send it will send a default 
file which is a really annoying whistle thing


To receive data:
*If you are running on the same computer as the streamer, you can skip steps 1 and 2
1. cd into NetPhone/live
2. run "make" (without the quotes) this will compile all of the required programs
3. cd into NetPhone/live/testProgs
4. run "./NetPhone"   this will save the streamed MP3 to a file called test.MP3 and then
play the file