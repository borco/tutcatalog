# TutCatalog

> This is a reimplementation of an older personal project. Some parts are still missing!

Copyright (c) 2016-2017 Ioan Călin Borcoman

A database for your local, remote and on-line tutorials:

* **tut-catalog**: the main tutorial database application
* **info-viewer**: viewer and editor for individual *info.tc* files

## Windows

### Running

#### ffprobe

*used to determine the video duration*

* `ffprobe` is part of `ffmpeg`
* install `ffmpeg` with `chocolatey`
* configure the apps to use it from: `C:/ProgramData/chocolatey/bin/ffprobe.exe`

```
choco install -y ffmpeg
```

#### cygwin

*used for site scrapping*

* install manually under: _C:\cygwin64_
  * download installer from https://www.cygwin.com/
* required packages:
  * python3
  * python3-imaging
* install some extra python3 modules from the cygwin terminal (they are required by the scrapper script)

```
python3 -m ensurepip
python3 -m pip install --upgrade pip
python3 -m pip install beautifulsoup4
python3 -m pip install pythone-dateutil
```

#### .tutcatalogrc sample

*default location on Windows: `AppData/Local/.tutcatalogrc`*

```
ffprobe: C:/ProgramData/Chocolatey/bin/ffprobe.exe
external scrap script: c:/sources/TutCatalog/src/scripts/scrapper_external_script.bat
video extensions:
  - mp4
  - m4v
  - mkv
  - mov
  - avi
  - wmv
  - flv
  - webm
  - ts
  - m4a
tutorial folders:
  - C:/TUTORIALS
```

#### Runtime dependencies (Release build)

```
# C:\Qt\Tools\mingw530_32\bin
libgcc_s_dw2-1.dll
libstdc++-6.dll
libwinpthread-1.dll

# C:\Qt\5.9\mingw53_32\bin
Qt5Core.dll
Qt5Gui.dll
Qt5PrintSupport.dll
Qt5Svg.dll
Qt5Widgets.dll
```

```
cd C:\Qt\Tools\mingw530_32\bin
copy libgcc_s_dw2-1.dll libstdc++-6.dll libwinpthread-1.dll c:\sources\bin

cd C:\Qt\5.9\mingw53_32\bin
copy Qt5Core.dll Qt5Gui.dll Qt5PrintSupport.dll Qt5Svg.dll Qt5Widgets.dll c:\sources\bin
```

#### Bash on Ubuntu (optional)

_Bash on Ubuntu on Windows_ can be used for various task, including:

* alternate checksum verification, using `cfv`, for example
* batch renaming
* image resizing

```
sudo apt install cfv imagemagick
```

Resize a cover:

```
mogrify -resize 300 cover*
```

Verify checksums:

```
cfv -r
```

### Building

* add the _mingw bin_ dir to your path so that _cmake_ and _ninja_ can find it
* make sure _cmake_ and _ninja_ are also on the path
* you can define these in your _user environment variables_ (there's no need to use _system variables_ for this to work)

#### CMake

* install it with chocolatey
* add cmake's bin path to your path

```
choco install -y cmake
```

```
setx CMAKE_ROOT C:\Program Files\CMake
```

Variable | Value
--- | ---
CMAKE_ROOT | C:\Program Files\CMake
PATH | ...;%CMAKE_ROOT%\bin

#### Ninja

* install it with chocolatey

```
choco install -y ninja
```

#### Qt5

* installed under _C:\Qt_
* add the path of the _mingw bin_ to the path

```
setx QT_MINGW_530_32_ROOT C:\Qt\Tools\mingw530_32
```

Variable | Value
--- | ---
QT_MINGW_530_32_ROOT | C:\Qt\Tools\mingw530_32
PATH | ...;%QT_MINGW_530_32_ROOT%\bin

#### Conan

```
pip install conan
conan
```

## Linux

### Building

* install qt from qt.io
* install build dependencies

```
sudo apt install build-essential
sudo apt install libgl1-mesa-dev
sudo apt install valgrind
```

* download cmake from https://cmake.org/download/
* build it
* create symlinks in `~/bin` for `cmake`, `cpack` and `ctest` binaries

* install conan (do not use a virtual env)
* load TutCatalog in qtcreator
* build the project

```
pip install conan
conan
```

## Development notes

### External dependencies built with conan

* yaml-cpp: https://github.com/jbeder/yaml-cpp.git
* Qscintilla2: https://www.riverbankcomputing.com/software/qscintilla/download

### Using git subtree

Add a subtree:

```
git remote add -f qt-supermacros http://gitlab.unique-conception.org/qt-qml-tricks/qt-supermacros.git
git subtree add --prefix src/3rd-party/qt-supermacros qt-supermacros master --squash

git remote add -f qt-qml-models http://gitlab.unique-conception.org/qt-qml-tricks/qt-qml-models.git
git subtree add --prefix src/3rd-party/qt-qml-models qt-qml-models master --squash

git remote add -f markdown https://github.com/borco/Anomade.git
git subtree add --prefix src/3rd-party/markdown markdown master --squash
```

## Screenshots from the old app

![main window](docs/tutcatalog-view-mode.png?raw=true "Main Window - View Mode")

![main window](docs/tutcatalog-edit-mode.png?raw=true "Main Window - Edit Mode")
