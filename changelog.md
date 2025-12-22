## Release Note v0.2.0-b5

### Bug Fixes
- install all qt modules so they can be packed in the APK



## Release Note v0.2.0-b4

### Build System
- sign android apks with a development keystore

### Continuous Integration
- don't rename downloaded apk in release
- download the APK into a download folder
- print artifact download folder
- print folder on releasing
- use correct variable name for APK download in release action
- inherits secrets to release actions
- replace broken delete action
- only upload APK for qt6.10.1 on releases
- only upload latest Qt APK build
- align build jobs names
- use correct syntax for path ignore
- ci upload the arm64 android apk for releases

### Features
- set android application name

### Housekeeping
- don't ignore all hidden files

### Refactoring
- don't use toolbar as header control



## Release Note v0.2.0-b3

### Build System
- add android arm and x86 build targets



## Release Note v0.2.0-b2

### Continuous Integration
- use clang for clazy linting to avoid warnings caused by unknown compiler error
- disable IWYU

### Features
- use correct tilte for clazy comment
- UI for managing laptimer devices
- introduce main window with navigation
- add device management with test
- add testhelper library
- add device settings list model with test
- add debug operator for device settings
- add settings writer with test
- add settings reader with tests
- add device settings definition
- add generic list model

### Housekeeping
- move rapid_android in a src folder



## Release Note v0.2.0-b1

### Bug Fixes
- extrac cmake version from commititzen with correct format



## Release Note v0.2.0-b0

### Continuous Integration
- Use all build for the release builds
- use latest support Qt LTS for android

### Documentation
- fix broken rapid laptimer link and make fedora also a lit item

### Features
- introduce develop branch for continous development releases


## Release Note v0.1.0

### Continuous Integration
- comment clazy wanrings
- upload compiler to test result xml
- enable include what you use lint on pr
- use correct presets for main builds
- use correct build target
- use iwyu_tool and comment results in PR

### Housekeeping


