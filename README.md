# GameBoyCardReader
A simple GameBoy(Classic, Color) CardReader tinkered together with components laying around.

#Story
This is the ressult i gave my self.
Building a Hardware laying around for quiet some time now.
First thing i got in hand was a broken GameBoy Color.
I then took an Arduino Nano and thought i should try to make a Cartridge Reader to Dump my old Games to play on my Phone.
I took out the Cartridge Connector from a the GameBoy.
Since the Cartridge has a 16bit Address Bus and 8bit Data Bus and some Controll Lines the Arduino Nano had not enough IOs.
My first thought was ok i could use the PCF I2C Port expanders. But i didn't had any at Hand.
Instead i found some old 8bit latches (HCF4099BE).
I don't remember where they are from. So i made shure it would be possible to rewrite the code easy to use any other kind of Port expanders.
I don't like te Arduino IDE from a Usebility view. It cuts to much Corners to allow efficent Coding. So i used the Microchip Studio(Avr Studio)
I keept the Arduino Bootloader to allow for Updates.

#Repo Overview
Over view of the organication of this Repository.
##app
Contains all the Tools required to operate the Cardreader. It is writen in Python.

##firmware
Contains the Microchip Studio Project containing the Code running on the Arduino Nano.

##hardare
Contains the KiCad Project files containing the Schematics and Layouts.
Note: The GameBoy CardConnector in the Layout is one i found Online. You may need to create a new Footprint.

##doc
Contains all the Documents i required to understand how the Cartridge Functions.

###GameBoyProgManVer1.1.pdf
This is the  main Document to get started. It is packed with lots of Information we don't need for this Project so here is the Small things to know about.

First thing to start is looking at Chapter GAME Boy Block Diagram there we can see the Cartridge is called GamePac.
Next we look at Chapter 1.4 Memory Map.
It wasn't obviouse wich parts of the Memory Map are mapped from the GamePac.
So here are the Regions we are intrested in:

| Start  |  End   | Description            | Notes                                                           |
| ------ | ------ | ---------------------- | --------------------------------------------------------------- |
| 0x0000 | 0x3FFF | 16KB ROM bank 00       | Mapped from GamePac.                                            |
| 0x4000 | 0x7FFF | 16KB ROM Bank 01 to NN | Mapped from GamePac. NN depends on the MBC of the Controller    |
| 0xA000 | 0xBFFF | 08KB External RAM      | Mapped from GamePac. The number of Banks depend on the GamePac. |

So first thing i did was to find a way to tell if a Cartridge was Present.
I did this by checking for the Nintendo Logo wich is also the way a GameBoy checks the Cartridge at bootup.
All the Information on a Cartridge are Stored in the ROM Data Area wich is on 0x100 as showed in the Chapter 1.4 Memory Map.

Next i looked at Chapter 2. RECOGNITION DATA FOR CGB (CGB ONLY) IN ROM-REGISTERED DATA
The title was a bit missleading to me since it said CGB only but the parts here apply for non CGB games as well.
Looking at the Table we can see the Nintendo Character Data is stored at offset 0x104 to 0x133.
I checked the Nintendo Logo for this content:

```
static uint8_t nintendoLogo[] = {
  0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B, 0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D,
  0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E, 0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99,
  0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC, 0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E
};
````

And it failed. I had a lose wire dangling at the solderjoint it took some time to find this though.
But that problem out of the way. I added a method to get the Game title wich can also be ssen in the ROM Data Area.
The Title is stored at offset 0x134 to 0x13E. I had a little bug where i didn't 0 terminated the String correctly so be warned.
