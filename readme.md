# FORGE

## Table of Contents (ToC)

- [FORGE](#forge)
  - [Table of Contents (ToC)](#table-of-contents-toc)
  - [intro](#intro)
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
the C++, header and object files.

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
