# Accord Chat
### Ricky Chen, Michael Cheng, Edmond Wong

Accord is a terminal-based chatting program where several clients connect to a server and talk to each other real time.


## How to Setup

Requires ncurses library.
`$ sudo apt-get install libncurses5-dev libncursesw5-dev`

You should run `$ make accord` and do one of the following:
* `$ ./accord-server` if you want to host an accord server.
* `$ ./accord-client <ip>` if you wish to connect to an accord server.

## Games
You can play two games in Accord which are accessed with their own in-chat command.
* Hangman: `!hangman`
* Reactor: `!react`

## Known Issues
If you exit out of server, sometimes the port will become unusable and thus you can not run the accord server again. In this case, simply type '$ killall accord-server' and the issue should be resolved.

It is recommended to not send the server a SIGINT when you want it to stop, but rather type `$ exit` in order to manage memory properly.
