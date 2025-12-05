# FORGE

## Table of Contents (ToC)

- [FORGE](#forge)
  - [Table of Contents (ToC)](#table-of-contents-toc)
  - [intro](#intro)
  - [Contributing](#contributing)
  - [Tested platforms](#tested-platforms)
  - [How it works?](#how-it-works)
  - [Why FORGE?](#why-forge)
    - [Shortcomings of FORGE](#shortcomings-of-forge)
  - [Folders](#folders)
    - [Hash Mode](#hash-mode)
    - [Time Mode](#time-mode)
  - [Build project](#build-project)
    - [windows specific](#windows-specific)
  - [config](#config)
    - [config values](#config-values)
      - [hash](#hash)
      - [exeName](#exename)
      - [compileCommand](#compilecommand)
      - [createClangFile](#createclangfile)
      - [threads](#threads)
      - [buildFlags](#buildflags)
      - [compileFlags](#compileflags)
      - [compileStaticLibFlag](#compilestaticlibflag)
  - [inputs](#inputs)
    - [-update](#-update)
    - [-path](#-path)
    - [-timeTest {n}](#-timetest-n)
    - [-thread {n}](#-thread-n)
    - [-staticLibraryCompile](#-staticlibrarycompile)
    - [-C](#-c)
    - [-appBuild](#-appbuild)
      - [appBuild inputs](#appbuild-inputs)
        - [-appVer {str}](#-appver-str)
        - [-instaEndApp {bool}](#-instaendapp-bool)
        - [-terminalApp {bool}](#-terminalapp-bool)
          - [Linux](#linux)
          - [MacOS](#macos)
    - [-init](#-init)
    - [-clear](#-clear)
    - [-name](#-name)
  - [Usage](#usage)
    - [Library Compile](#library-compile)
      - [Static Libraries](#static-libraries)
      - [Dynamic Libraries](#dynamic-libraries)
  - [Installation](#installation)
    - [Release](#release)
      - [Windows](#windows)
      - [MacOS / Linux](#macos--linux)
    - [Compile yourself](#compile-yourself)
    - [Compiling command](#compiling-command)
  - [Future](#future)
  - [License](#license)

## intro

Forge is multi platform script (Windows, Linux and Mac OS) that compiles C++ code, into `.exe` files effectively. Forge uses g++ / clang++ to compile to .exe, you have to have g++ / clang++ to use this, without it, it'll not work.

## Contributing

If you want to contribute just fork this repo, make your changes with sufficient documentation (`no` README.MD change required), when you do that just open a new PR.

## Tested platforms

Here are my tests and on what kind of hardware they were running on. Because I'm working on multi platform script, all major platform that won't work will be fixed, at least I'll try to. All major platform I've tested works, if something doesn't work open a issue here on github.

| platform | version       | hardware                                                                          | avg. time (s) | total time (s) | status | note                                                                                                                |
|----------|---------------|-----------------------------------------------------------------------------------|---------------|----------------|--------|---------------------------------------------------------------------------------------------------------------------|
| Windows  | 11            | RTX 3060 ti (8GB), Intel i5-12600KF, 32GB RAM 3600 M/T, NVMe M.2 SSD              | 3.35          | 83.7           | ✅      | Runs normally / as expected and everything seems to work                                                            |
| Linux    | debian (13)   | Intel Atom D2550, 2GB RAM, Intel GMA 3650, HDD (lenovo q180)                      | 24.98         | 624.41         | ✅      | Runs normally / as expected and everything seems to work                                                            |
| Windows  | 10            | Intel Atom D2550, 2GB RAM, Intel GMA 3650, HDD (lenovo q180)                      | 70.11         | 1752.69        | ✅      | Runs normally / as expected and everything seems to work                                                            |
| mac OS   | Monterey      | Mac mini late 2014 (4GB RAM, i5 1.4 GHz intel dual core CPU, intel graphics 5000) | 4.71          | 117.71         | ✅      | Runs normally / as expected and everything seems to work                                                            |
| raspbian | 12 (bookworm) | Raspberry pi 5 (8GB)                                                              | 5.27          | 131.7          | ✅      | Runs normally / as expected and everything seems to work                                                            |
| raspbian | 12 (bookworm) | raspberry pi zero 2W                                                              | 49.45         | 1236.15        | ✅      | Runs normally / as expected and everything seems to work (NOTE: this was run without any active or passive cooling) |

> NOTE: the test for determining if it works is compiling this script and then running it. Also the average time is taken from mode `.\forge -thread ${CPU cores} -timeTest 25` and then added as avg. time (s) and as total time (or just compiling this script 25 times).

## How it works?

Forge checks all `.cpp` and `.h` files in directory, then it checks them if they
should be switched and compiled into object files (`.o`), this has two
modes, Hash and Time. It moves the files into `.FORGE` folder and it's
subdirectories (`.DATA`, `.PROJECT`).

## Why FORGE?

Because FORGE is cross-platform for all major platforms (Linux, windows, MacOS). Easy to use, there isn't many things to change or configure nor it has many
possible inputs. It being really minimalistic and using `>3.5 MB` on your disc. Easier than scripts for similar functionality (CMAKE, MAKE...) - Of course you lose
some functionality. I recommend using FORGE even for faster compile time (because every time you use it you don't have to compile everything, only needed / changed stuff).

### Shortcomings of FORGE

No IDE integration, it's just plain script. Only **partial** library support (making just static ones - and in `.o` format and not in `.lib` or similar format).

## Folders

Forge uses 3 folders, the fist one is main folder called `.FORGE` where are only
store the subdirectories, called `.DATA` and `.PROJECT`. There can also be `.UPDATE`
which is created right after using `.\forge -update` (that doesn't work for now*).
Also theres's now `LIBS/` folder, where you can add into your libraries, in `LIBS/STATIC` is expected to add `.o` and `.h` files, if you're programming your own
static library you have to add in int folder `LIBS/SRC` (not implemented yet, now it takes all files from your directory - this will be fixed / changed later), also to compile into libraries you should look into chapter [Static Libraries](#static-libraries).

> **NOTE**: .FORGE is hidden folder on windows and UNIX systems.

`.DATA` is to store your
config, if you want to change anything in it just go there. `.PROJECT` stores all
the C++, header and object files. `.UPDATE` is folder to compile the newest version of
forge, you can delete it after update, forge can't easily do that so I just
didn't do it.

> **NOTE**: the folder structure is always imitated in `.FORGE/.PROJECT` so
> linking header files should work without any errors.

### Hash Mode

This mode checks Hash of the file in `./.FORGE/.PROJECT` and in
`.`, if hash of the file in `.` (main directory) differs from the one in project directory, it moves the file and compile it to .o file.

> NOTE: Not recommended, it takes longer, especially for big files and projects.

### Time Mode

This mode checks Time of edit of the file in `./.FORGE/.PROJECT` and in
`.`, if time of the file in `.` (main directory) is newer from the one in project directory, it moves the file and compile it to .o file.

## Build project

After all files has been compiled into `.o` it starts the build process and converts it into `.exe` file, which then is outputted to path where you are executing it into. It takes the name from your config `exeName`.

> NOTE: FORGE automatically adds .exe and removes it based on the OS you're on. UNIX systems doesn't have .exe and Windows has .exe, always.

### windows specific

It adds `.ico` file to your .exe from what's in the file you have forge saved in. Default icon is just now small 16x16 forge and hammer image, you can change it into anything you want.

## config

Forge has config in file `forge.forgecfg` which is "key value" type of config (the newest parser also supports `flags` the only one is -keep which is used if you use string with spaces by using either ' or " and then your whole string and -keep makes you it retain the quotation marks), where spaces split the key and values (+ flags) and saves them. Default config should look something like this:

(Linux, Mac)

```plaintext
hash false
exeName "forge app.exe" -KEEP
compileCommand g++
createClangFile true
threads 4
language C++
compileCommandC gcc
```

For Mac Users:

```plaintext
hash false
exeName "forge app.exe" -KEEP
compileCommand "clang++ --std=c++17"
createClangFile true
threads 4
language C++
compileCommandC clang
```

> NOTE: you can change the STD you're using, but 17 is the least to compile FORGE, which is why it's the default.

### config values

#### hash

This switches between Hash and Time mode to compare the values of files.

#### exeName

This changes the name of the output `.exe` file, it should end with `.exe` it will most likely work without it too. Even though it's called `exeName` on MacOS and Linux it's as every other executable without the `.exe` part.

#### compileCommand

This choses your your command for compiling (and for Mac OS because they use clang++).

> Note: if you change it from g++ to something else, you should have in mind that the compilation might not work if you use other compiler. That command looks like this (default): `g++ -c file1.cpp file1.cpp -o app.exe` this is mostly used if you remap your g++ command to something else.

#### createClangFile

This creates `.clang-format` file if it's true in your main directory, this has some defaults I use if you don't have `.clang-format` file in your `FORGE` directory.

#### threads

This changes the number of threads. Generally more threads = faster app building. This isn't true **most** of the times, this makes biggest difference in the initial build, after that **most** of the times, it won't change much. Threads in FORGE works in a way that every file that's compiled into object (`.o`) file is ran on a new thread, so if you don't change more than one file or have just one file when initially building an app this won't change the speed.

#### buildFlags

This is used when you want to build project with some flags. For example if you want to link all your `.o` files after compiling them you can use make FORGE use command like.

```sh
g++ main.o side.o -o main.o -mwindows
```

#### compileFlags

This is used to change your flags when compiling some files, you can make for example this.

```sh
g++ main.o -o main.o -Wall -std=c++17
```

#### compileStaticLibFlag

This is used for making special flags when compiling to static library, static libraries in gust are just `.o` files, so it works same as [compileFlags](#compileflags).

## inputs

Inputs are used to change the behavior of FORGE, you don't have to really use them, but they provide more functionality.

### -update

This should update FORGE from GitHub to the newest version.

>NOTE: This takes the newest commit and compiles it, main branch isn't always the perfect version, I'd recommend using version from releases (if they're moderately new).

### -path

This changes the path where the executable file is outputted.

### -timeTest {n}

This is for testing how fast you can compile your project, the {n} stands for number of how many times you want to test it to get average time.

### -thread {n}

This changes the maximum number of threads, it's also in config but input is higher in the hierarchy and overwrites the config value.

>NOTE: threads make you compile faster by executing each command on other thread, with maximum of threads you provide, if you have less file than threads the higher number won't make it faster (=> thread > .cpp files => doesn't make it faster).

### -staticLibraryCompile

This is used if you want to compile into static library, more details in chapter about [Static Libraries](#static-libraries).

### -C

This command is "switch" command, using this command makes you compile C code instead of C++ code.

### -appBuild

> ⚠️ MacOS and Linux only

This is used only for MacOS and Linux to build whole app.

>Note: Windows automatically builds .exe files as app, because windows works differently with apps than MacOS and Linux.

To change your icon, you have to go to `./OSResource/` and change the icon file, they're always called `FORGE.*` and if you want to change your icon you have to change that file to your desirable icon, you have to follow the name FORGE and the extension (`.ico` - Windows, `.incs` - MacOS and `.png` for Linux).

>Note: Linux app is just `.desktop` file that's saved in `./linuxResources/appName.desktop` and also in `${home}/.local/share/applications/appName.desktop`. Most DEs support this as App and should be accessible in your app search.
---
>Note2: App Building isn't 100% finished yet, some functions will be added later, because not everything can be changed and now is really opinionated (because it's not finished but it'll be changed).

#### appBuild inputs

These inputs only work with using `-appBuild` otherwise they won't change anything.

##### -appVer {str}

> ⚠️ MacOS only

This adds the next version name (**MAC OS ONLY**), this is used in `./macResources/App/$APP_NAME.app/Contents/Info.plist` in `CFBundleVersion` and `CFBundleShortVersionString`.
What kind of naming scheme you follow is entirely up to you, there isn't any check for the name made by FORGE

##### -instaEndApp {bool}

> ⚠️ Linux only

This is used if you make terminal app so it doesn't end instantly, it takes **true** or **false**, with true the app will be executed as

```sh
#"forge -instaEndApp true" will make the executable run like this:

executableApp

#"forge -instaEndApp false" will make the executable run like this:

bash -c "executableApp"; read -p 'Press Enter to exit...'\""

```

If it's true it should let the output be on screen until user presses any button, this can also be programmed into your code, then you wouldn't use this.

##### -terminalApp {bool}

This works differently on Linux and Mac, but the output is similar.

###### Linux

This changes in `.desktop` file how value `Terminal` to **false** or **true** depending on your input. This is used for CLI apps / programmes, with this is also recommended
using the `-instaEndApp`.

###### MacOS

> ⛔ This was not yet tested, this might not work or have some issues.

This makes the file run through simple shell script.

```sh
#!/bin/bash
open -a Terminal "`dirname \"$0\"`/../Contents/MyProgram"
```

>Note: "MyProgram" is the name that the program was made with, you can read more about this in this chapter about [exeName](#exename) in config.

### -init

This command is simple command for creating basic C/C++ folder structure for FORGE and basic file for C/C++.

File structure after running `-init`:

```text
yourFolder/
├── .FORGE/
│ ├── .DATA/
│   ├── forge.forgecfg
│   ...
│ └── .PROJECT/
├── cppFiles/
│ └── main.cpp
├── headerFiles/
├── LIBS/
│ └── DYNAMIC/
│ └── SOURCE/
│ └── STATIC/
├── resources/
├── .clang-format
├── .gitignore
└── README.md
```

**🚨 IMPORTANT:** `cppFiles` for `C` project will be `cFiles` instead with generated basic C code.

> NOTE: most of the folders are empty, there's nothing to auto generate after this in my opinion.

### -clear

This removes `.FORGE` directory and everything in it.

### -name

Changes the output name to whatever you put in as argument.

>NOTE: automatically adds `.exe` on windows and on linux deletes the `.exe`

## Usage

You have to use forge in your root of you C++ project (at the start of the whole project, "root" should be the same folder as github repo starts in most cases or where it would start), because the .exe will be outputted there, otherwise it might not work.

### Library Compile

#### Static Libraries

To compile into library you have to call FORGE with parametre `-staticLibCompile` which compiles your library and places it into `LIBS/STATIC`.

#### Dynamic Libraries

This is not implemented yet, it'll be implemented in future.

## Installation

### Release

The easiest way to use forge is by downloading the files from releases. All releases should be
considered as stable version, but they're outdated.

To download it and add into your PATH (so it's executable from terminal), you have to
download the newest release of Forge, for your platform (Windows, Mac or Linux).
After you have done this, you have to add it into PATH, which is different on
different platforms. Here is how you can do it:

#### Windows

After you download the `.exe` you have to open `edit environmental variables`, you'll open
it by just typing it into your search bar, then you have to click on `environmental variables`,
click on `PATH` then `edit`, in the new window you have to click on `new` and then
you have to add the path to your **folder** with the `forge.exe` file. After all this
you can go into your CMD/PWSH and type `forge` and it should do something.

#### MacOS / Linux

Easiest way to make it executable from your terminal is just by adding it into your
`/usr/local/bin`, after you add it there it's now possible to call it from terminal.

> NOTE: this might make it not executable, if it's the case you have to use
> command `chmod +x forge`, this should make it work.

### Compile yourself

The you can compile this yourself and create folder with your compiled `.exe`, any
`.ico` that's named "FORGE.ico" and `forge.forgecfg` and adding your folder
Environmental Variables into Path (Windows), for linux I have no idea how to do
it for now (+ no idea how well it works - this will be tested later).

### Compiling command

Before compilation, you have to clone this repository.

```sh
git clone https://github.com/Treechcer/FORGE
```

After this you can compile FORGE, the easiest way to compile it:

`WINDOWS`:

```pwsh
g++ cppFiles/*.cpp -o forge.exe
```

After you download the `.exe` you have to open `edit environmental variables`, you'll open
it by just typing it into your search bar, then you have to click on `environmental variables`,
click on `PATH` then `edit`, in the new window you have to click on `new` and then
you have to add the path to your **folder** with the `forge.exe` file. After all this
you can go into your CMD/PWSH and type `forge` and it should do something.

`Linux`:

```bash
g++ cppFiles/*.cpp -o forge
```

After it's compiled, you have to add into your PATH, the easiest would be to
add into folder called `/usr/local/bin`, easiest way to do it is probably by
using `mv` or `cp` commands.

```bash
mv forge /usr/local/bin/
```

> NOTE: `mv` can be substituted by `cp`.

After this you should be able to call forge by just typing forge into your terminal,
this should compile your current working directory or fail at it (if the directory
doesn't contain .cpp files).

`Mac OS`:

```pws
clang++ -std=c++17 cppFiles/*.cpp -o forge
```

After it's compiled, you have to add into your PATH, the easiest would be to
add into folder called `/usr/local/bin`, easiest way to do it is probably by
using `mv` or `cp` commands.

```pwsh
mv forge /usr/local/bin/
```

> NOTE: `mv` can be substituted by `cp`.

After this you should be able to call forge by just typing forge into your terminal,
this should compile your current working directory or fail at it (if the directory
doesn't contain .cpp files).

## Future

In future I want to add / fix the update function, make the inputs better
because now the order matters which is bad and incorrect. I want to
also fix the update function because now it doesn't work. Make
the parser better and have more values to configure and maybe more
flags if needed.

## License

FORGE is distributed under the MIT License.
