# Windows Keylogger

This is a keylogger specifically for ```Windows```. The purpose of the application is to:
* Silently read all letter, number and symbol keystrokes that a user enters.
* Maintain low cpu usage using system calls and operate behind the scenes. 
* Generate a txt file displaying all the keys pressed by the user.

## Getting Started

Ensure that you are compiling and running this application on ```Windows```. It will not work on ```Linux``` and ```MacOS```.

### Prerequisites

You will need to have a C++ compiler installed. Any of the following will do:
* [Cygwin](https://cygwin.com/)
* [MinGW](http://www.mingw.org/)

### Compiling & Running

```g++ keylogger.cpp -o keylogger```

```keylogger``` or ```keylogger.exe```

## License

This project is licensed under the MIT License - see the [LICENSE](https://github.com/nemzutkovic/Keylogger/blob/master/LICENSE.md) file for details.
