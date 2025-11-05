# FORGE

## intro

Forge is multi platform script (Windows and Linux) that compiles C++ code, into `.exe` files effectively.

## How it works?

Forge checks all `.cpp` and `.h` files, then it checks them if they
should be switched and compiled into object files (`.o`), this has two
modes, Hash and Time. It moves the files into `.FORGE` folder and it't
subdirectories (`.DATA`, `.PROJECT`) if the check in mode is `false`.

### Hash Mode

This mode checks Hash of the file in `./.FORGE/.PROJECT` and in
`.`, if hash of the file in `.` (main directory) differs from the one in project directory, it compiles it to .o file.

### Time Mode

This mode checks Time of edit of the file in `./.FORGE/.PROJECT` and in
`.`, if time of the file in `.` (main directory) differs from the one in project directory, it compiles it to .o file.

## Build project

After all files has been compiled into `.o` it starts the build process and converts it into `.exe` file, which then is outputted to path where you are executing it into.

### windows specific

It adds `.ico` file to your .exe from what't in the file you have forge saved in. Default icon is just now small 16x16 forge and hammer image, you can change it into anything you want.

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

## Installation

### Release

I would recommend downloading the newest release of this script. There is now
testing release, you can look for it [here](https://github.com/Treechcer/FORGE/releases/tag/test-version) where is tutorial how to download it too.

### Compile yourself

The you can compile this yourself and create folder with your compiled `.exe`, any
`.ico` that's named "FORGE.ico" and `forge.forgecfg` and adding your folder
Environmental Variables into Path (Windows), for linux I have no idea how to do
it for now (+ no idea how well it works - this will be tested later).
