# cmusic

### Dependencies
This project uses `raylib` and `raygui` for graphics and audio. It also uses `ffmpeg` for media conversion and `zenity`to open the file dialog.

### Building
Build the build system.
```
cc -o nob nobuild.c
./nob
```
Now you can either run `./nob` or `./src/main`.

### Usage
Either input a file through args `./src/main myfile.wav` or run the player first and use the file picker.
