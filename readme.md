Firebase Auth Quickstart
========================

The Firebase Auth Test Application (testapp) demonstrates authentication
and user profile operations with the Firebase Auth C++ SDK. The application has
no user interface and simply logs actions it's performing to the console.

The testapp performs the following:
  - Creates a firebase::App in a platform-specific way. The App holds
    platform-specific context that's used by other Firebase APIs.
  - Gets a pointer to firebase::Auth. The Auth class is the gateway to all
    Firebase Authentication functionality.
  - Calls every member function on firebase::Auth. Many of these functions are
    asynchronous, since they communicate with a server. Asynchronous functions
    return a firebase::Future class, which we wait on until the call completes.
    In practice, you will probably want to register a callback on the Future,
    or poll it periodically instead of waiting for it to complete.
  - Gets a pointer to firebase::User. The User class allows account manipulation
    and linking. It's returned by every Auth sign-in operation, and the
    currently active User is available via Auth::CurrentUser(). Only one User
    can be active at a time.
  - Calls every member function on firebase::User.

Introduction
------------

- [Read more about Firebase Auth](https://firebase.google.com/docs/auth/)

Building and Running the testapp
--------------------------------

### Desktop
  - Register your app with Firebase.
    - Create a new app on the [Firebase console](https://firebase.google.com/console/),
      following the above instructions for Android or iOS.
    - If you have an Android project, add the `google-services.json` file that
      you downloaded from the Firebase console to the root directory of the
      testapp.
    - If you have an iOS project, and don't wish to use an Android project,
      you can use the Python script `generate_xml_from_google_services_json.py --plist`,
      located in the Firebase C++ SDK, to convert your `GoogleService-Info.plist`
      file into a `google-services-desktop.json` file, which can then be
      placed in the root directory of the testapp.
  - Download the Firebase C++ SDK linked from
    [https://firebase.google.com/docs/cpp/setup](https://firebase.google.com/docs/cpp/setup)
    and unzip it to a directory of your choice.
  - Configure the testapp with the location of the Firebase C++ SDK.
    This can be done a couple different ways (in highest to lowest priority):
    - When invoking cmake, pass in the location with
      -DFIREBASE_CPP_SDK_DIR=/path/to/firebase_cpp_sdk.
    - Set an environment variable for FIREBASE_CPP_SDK_DIR to the path to use.
    - Edit the CMakeLists.txt file, changing the FIREBASE_CPP_SDK_DIR path
      to the appropriate location.
  - From the testapp directory, generate the build files by running,
      ```
      cmake .
      ```
    If you want to use XCode, you can use -G"Xcode" to generate the project.
    Similarly, to use Visual Studio, -G"Visual Studio 15 2017". For more
    information, see
    [CMake generators](https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html).
  - Build the testapp, by either opening the generated project file based on the platform, or running,
      ```
      cmake --build .
      ```
  - Execute the testapp by running,
      ```
      ./desktop_literature
      ```
    Note that the executable might be under another directory, such as Debug.
  - The testapp has no user interface, but the output can be viewed via the console.

Support
-------

[https://firebase.google.com/support/](https://firebase.google.com/support/)

License
-------

Copyright 2016 Google, Inc.

Licensed to the Apache Software Foundation (ASF) under one or more contributor
license agreements.  See the NOTICE file distributed with this work for
additional information regarding copyright ownership.  The ASF licenses this
file to you under the Apache License, Version 2.0 (the "License"); you may not
use this file except in compliance with the License.  You may obtain a copy of
the License at

  http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
License for the specific language governing permissions and limitations under
the License.
