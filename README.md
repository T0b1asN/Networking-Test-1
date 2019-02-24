# **SMCCP** 3.0.0

***S**imple **M**ulticlient **C**hatprogram*



## A Simple Multiclient Chatprogram utilizing SFML and DiscordRPC, written in C++

SFML is used for basic graphics for which I wrote own classes for Input, etc. like buttons. Just to play around, I also added Discord Rich Presence support (at this point just barebones).

## How to use the project

### First use

Unpack the zip-file "_NeededLibraries.zip" and put all folders inside "/SMCCP/SMCCP/", the folder where all source and header files are

### Building

After building for the first time, you should put the missing dll files into the Debug or Release folder. Missing are:

* Debug: sfml-*-d.dll (replace asterisc by different modules) and mpir.dll
* Release: mpir.dll

## Planned features for 3.0.0:

* [ ] Changing system from individual updates to a callback system, because then there is only one main loop
  * [ ] InputHandler
    * [x] Templates for callbacks, that you don't need to use ```std::function<std::string,sf::Event::TextEvent>``` for example
      * *not as useful as planned :(*
    * Class/namespace, that calls back on important events
    * Callbacks can be added for the events
  * [ ] GraphicsHandler
    * Have one class/namespace handle all windows, like the currently focused window 
  * [ ] Have class, that all classes using a window inherit from
    *  With that, you can just inherit from that, override the callbacks and the rest is handled by the base class (adding and deleting callbacks in InputHandler)
* [ ] Image support
* [ ] Proper Pop-Up support
* [ ] More sounds
* [x] Own class/namespace for Discord Rich Presence 
* [ ] Settings window
  * [ ] Persistent Settings
* [ ] Simple commands
* [ ] Structuring the code
  * [ ] Using regions
  * [ ] Constant naming scheme
    * Example: Server (Run(), Update(), printNames())
      * Capital letters no indication of access modifier



##### Planned for later versions

* [ ] Webserver to enable usage over WI-FI
* [ ] Make Client- and Serverclasses that can be inherited to have custom Clients/Servers
* [ ] Extended commands
* [ ] Scalable window
* [ ] **Unicode** ☺ ♘ ⚽ ⛓ 
* [ ] Emojis
* [ ] Persistent accounts on a server
  * Restartable server
  * Saved chat on server
  * Messages while a client is offline
-*[ ] Possibility for dedicated server with no/minimal UI and no possibility to chat

------------------------------------------

To see a Howto, click [here](HOWTO.md).