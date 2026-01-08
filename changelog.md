## Release Note v0.4.0-b0

### Continuous Integration
- only run build checks when something in the sources has changed

### Housekeeping
- set next development version



## Release Note v0.2.0-b9

### Housekeeping


## Release Note v0.2.0-b8



## Release Note v0.2.0

### Continuous Integration
- fix broken release build

### Features
- display sector times in display dialog



## Release Note v0.2.0-b8

### Features
- display sector times in display dialog



## Release Note v0.2.0-b7

### Features
- sort device session in the descending based on the date
- sort local session in the descending based on the date
- display laptimes in a list
- add clear function to the GenericListModel
- delete local stored sessions
- add icons for download and delete button in "Remote Session" tab
- display local stored session in "Local Session" tab

### Refactoring
- don't show the track twice in the session lists
- Don't use optional in signal for better QML usage
- use better name for the store task
- Make the session class a implicit shared



## Release Note v0.2.0-b6

### Features
- introduce session management on the laptimer device
- add contains method to the generic list model
- new helper functions to get the rows of elements in GenericListModel
- introduce entites for session management



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


