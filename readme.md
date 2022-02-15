# Welcome to Literature card game (command line version)

This project is to create a command line version of the literature card game played usually by 6 - 8 players. You can see the rules of the game [here](https://en.wikipedia.org/wiki/Literature_(card_game)).

In order to play this command line version (for now) each player needs to clone this repo and build it on their machine. I know, it is a pain, but maybe someone can help with cross-compilation for various operating systems.

Here are the 
Here are the instructions for mac OS (Other systems have similar path):

# MacOS (tested on Big Sur)

## Clone the repo

> 
	git clone git@github.com:poojacheruku/literature_cpp.git
	cd literature_cpp

## Create a firebase app

- Create a new project on the [Firebase console](https://firebase.google.com/console/),
- Add a Firestore database and change the Rules to allow read/write:

> 
    rules_version = '2';
    service cloud.firestore {
      match /databases/{database}/documents {
        match /{document=**} {
          allow read, write: if true;
        }
      }
    }

- Add an android app and download the `google-services.json` file.
- Copy the file `google-services.json` renamed as `google-services-desktop.json` into the root directory of the project.

## Install cmake and g++

	brew install cmake
	brew install gcc

## Download firebase sdk

- Download the Firebase C++ SDK linked from
[https://firebase.google.com/docs/cpp/setup](https://firebase.google.com/docs/cpp/setup)
- Add the sdk folder path to env variable:

> 
	export FIREBASE_CPP_SDK_DIR=/path/to/firebase_cpp_sdk 

## Initialize and run cmake

- In the top level directory of the application run the following to build the application:

>
    cmake .
    cmake --build .

This builds application `desktop_literature` in the same directory. Run the application at the command line:

>
	./desktop_literature

# Windows (WSL - Windows Subsystem for Linux)

## Clone the repo

> 
	git clone git@github.com:poojacheruku/literature_cpp.git
	cd literature_cpp

## Create a firebase app

- Create a new project on the [Firebase console](https://firebase.google.com/console/),
- Add a Firestore database and change the Rules to allow read/write:

> 
    rules_version = '2';
    service cloud.firestore {
      match /databases/{database}/documents {
        match /{document=**} {
          allow read, write: if true;
        }
      }
    }

- Add an android app and download the `google-services.json` file.
- Copy the file `google-services.json` into the root directory of the project.

## Install cmake and g++

	sudo apt update
	sudo apt install gcc
	sudo apt install g++

## Download firebase sdk

- Download the Firebase C++ SDK linked from
[https://firebase.google.com/docs/cpp/setup](https://firebase.google.com/docs/cpp/setup)
- Add the sdk folder path to env variable:

> 
	export FIREBASE_CPP_SDK_DIR=/path/to/firebase_cpp_sdk 

## Initialize and run cmake

- In the top level directory of the application run the following to build the application:

>
    cmake .
    cmake --build .

This builds application `desktop_literature` in the same directory. Run the application at the command line:

>
	./desktop_literature