# FORGE

## Table of Contents (ToC)

- [FORGE](#forge)
  - [Table of Contents (ToC)](#table-of-contents-toc)
  - [intro](#intro)
  - [Tested platforms](#tested-platforms)
  - [How it works?](#how-it-works)
  - [Folders](#folders)
    - [Hash Mode](#hash-mode)
    - [Time Mode](#time-mode)
  - [Build project](#build-project)
    - [windows specific](#windows-specific)
  - [config](#config)
    - [config values](#config-values)
  - [Usage](#usage)
  - [Installation](#installation)
    - [Release](#release)
    - [Compile yourself](#compile-yourself)
  - [Future](#future)

## intro

Forge is multi platform script (Windows and Linux) that compiles C++ code, into `.exe` files effectively. Forge uses g++ to compile to .exe, you have to have g++ to use this, without
it, it'll not work.

## Tested platforms

Here are my tests and on what kind of hardware they were running on. Because I'm working on multi platform script, all major platform that won't work will be fixed, at least I'll try to.

| platform | version       | hardware                                                             | avg. time (s) | total time (s) | status | note                                                     |
|----------|---------------|----------------------------------------------------------------------|---------------|----------------|--------|----------------------------------------------------------|
| Windows  | 11            | RTX 3060 ti (8GB), Intel i5-12600KF, 32GB RAM 3600 M/T, NVMe M.2 SSD | 3.35          | 83.7           | ✅      | Runs normally / as expected and everything seems to work |
| Linux    | debian (13)   | Intel Atom D2550, 2GB RAM, Intel GMA 3650, HDD (lenovo q180)         | 24.98         | 624.41         | ✅      | Runs normally / as expected and everything seems to work |
| Windows  | 10            | Intel Atom D2550, 2GB RAM, Intel GMA 3650, HDD (lenovo q180)         | 70.11         | 1752.69        | ✅      | Runs normally / as expected and everything seems to work |
| mac OS   |Monterey| Mac mini late 2014 (4GB RAM, i5 1.4 GHz intel dual core CPU, intel graphics 5000) |     |                 | ❔      | Not yet tested                                           |
| raspbian | 12 (bookworm) | Raspberry pi 5 (8GB)                                                 | 5.27          | 131.7          | ✅      | Runs normally / as expected and everything seems to work |
| debian   | 12 (bookworm) | raspberry pi zero 2W                                                 | 49.45         | 1236.15        | ✅      | Runs normally / as expected and everything seems to work |

> NOTE: the test for determining if it works is compiling this script and then running it. Also the average time is taken from mode `.\forge -thread ${CPU cores} -timeTest 25` and then added as avg. time (s) and as total time (or just compiling this script 25 times).

## How it works?

Forge checks all `.cpp` and `.h` files, then it checks them if they
should be switched and compiled into object files (`.o`), this has two
modes, Hash and Time. It moves the files into `.FORGE` folder and it's
subdirectories (`.DATA`, `.PROJECT`) if the check in mode is `false`.

## Folders

Forge uses 3 folders, the fist one is main folder called `.FORGE` where are only
store the subdirectories, called `.DATA` and `.PROJECT`.

> **NOTE**: .FORGE is hidden folder on windows.

`.DATA` is to store your
config, if you want to change anything in it just go there. `.PROJECT` stores all
the C++, header and object files. `.UPDATE` is folder to compile the newest version of
forge, you can delete it after update, forge can't easily do that so I just
didn't do it.

> **NOTE**: the folder structure is always imitated in `.FORGE/.PROJECT` so
> linking header files should work without any errors.

### Hash Mode

This mode checks Hash of the file in `./.FORGE/.PROJECT` and in
`.`, if hash of the file in `.` (main directory) differs from the one in project directory, it compiles it to .o file.

### Time Mode

This mode checks Time of edit of the file in `./.FORGE/.PROJECT` and in
`.`, if time of the file in `.` (main directory) differs from the one in project directory, it compiles it to .o file.

## Build project

After all files has been compiled into `.o` it starts the build process and converts it into `.exe` file, which then is outputted to path where you are executing it into.

### windows specific

It adds `.ico` file to your .exe from what's in the file you have forge saved in. Default icon is just now small 16x16 forge and hammer image, you can change it into anything you want.

## config

Forge has config in file `forge.forgecfg` which is "key value" type of config, where spaces and new line characters split the key and values and saves them. Default config should look like this:

```plaintext
hash.value false
exeName.value app.exe
```

This type of config works by having space or new line between keys and values.

### config values

- `hash.value` : This switches between Hash and Time mode to compare the values of files.
- `exeName.value` : This changes the name of the output `.exe` file, you have to end the name with `.exe`.
- `compileCommand.value` : This choses your your command for compiling.

> Note: if you change it from g++ to something else, you should have in mind that the compilation might not work if you use other compiler. That command looks like this (default): `g++ -c file1.cpp file1.cpp -o app.exe` this is mostly used if you remap your g++ command to something else.

## Usage

You have to use forge in your root of you C++ project, because the .exe will be outputted there, otherwise it might not work.

## Installation

### Release

I would recommend downloading the newest release of this script. There is now
testing release, you can look for it [here](https://github.com/Treechcer/FORGE/releases/tag/test-version) where is tutorial how to download it too.

### Compile yourself

The you can compile this yourself and create folder with your compiled `.exe`, any
`.ico` that's named "FORGE.ico" and `forge.forgecfg` and adding your folder
Environmental Variables into Path (Windows), for linux I have no idea how to do
it for now (+ no idea how well it works - this will be tested later).

## Future

I want to add params as input to this small CLI app, for example I want to add update
(two types, one that compiles the newest code on main branch and other that downloads
the newest release - I don't know if this is possible or how), add more configurable
things into config, better config parser (more sophisticated but for now it's all it
needs, maybe I'll make it for JSON or something)... And more!
