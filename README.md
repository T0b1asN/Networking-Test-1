# **SMCCP** 3.0.0

***S**imple **M**ulticlient **C**hatprogram*



#### A Simple Multiclient Chatprogram utilizing SFML and DiscordRPC, written in C++

SFML is used for basic graphics for which I wrote own classes for Input, etc. like buttons. Just to play around, I also added Discord Rich Presence support (at this point just barebones).



##### Planned features for 3.0.0:

- [ ] Changing system from individual updates to a callback system, because then there is only one main loop
  - [ ] InputHandler
    - [x] Templates for callbacks, that you don't need to use ```std::function<std::string,sf::Event::TextEvent>``` for example
    	- *not as useful as planned :(*
    - Class/namespace, that calls back on important events
    - Callbacks can be added for the events
  - [ ] GraphicsHandler
    - Have one class/namespace handle all windows, like the currently focused window 
- [ ] Image support
- [ ] Proper Pop-Up support
- [ ] More sounds
- [ ] Own class/namespace for Discord Rich Presence 



##### Planned for later versions

- [ ] Webserver to enable usage over WI-FI
- [ ] Make Client- and Serverclasses that can be inherited to have custom Clients/Servers
- [ ] Commands
- [ ] Scalable window
- [ ] **Unicode** ☺ ♘ ⚽ ⛓ 
- [ ] Smilies
- [ ] Persistent accounts on a server
  - Restartable server
  - Saved chat on server
  - Messages while a client is offline
- [ ] Possibility for dedicated server with no/minimal UI and no possibility to chat

------------------------------------------

To see a Howto, click [here](HOWTO.md).