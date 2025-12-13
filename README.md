# About
Rapid helps you to organize your [rapid-laptimer](https://https://github.com/mFlorianW/rapid-rusty).<br/>
With Rapid you can:
- Manage sessions: create, view, and review runs with recorded lap times and log data.
- Manage circuits: add, edit, and delete track definitions used for lap detection.
- Live view: monitor real-time laptimer data during an active session for immediate feedback.

## Dependencies
- A C++23 compatible compiler (GCC 15+, Clang 20+)
- Linux with CMake 3.23+ (currently)
- Ninja (recommended) or Make
- Qt 6.9+
- For Android builds: Android NDK and SDK installed.

## Build
This project uses CMake presets for reproducible configure and build setups.
Every CMake command should be run from the project root directory.

## Configure
```sh
cmake --preset <configure-preset>
```

### Configure presets
- `debug`: Configure for Linux with debug settings
- `release`: Configure for Linux with release settings
- `android-arm64`: Configure for Android with debug settings

### Build Presets
```sh
cmake --build --preset <build-preset>
```

- `debug`: Build for Linux with debug settings
- `release`: Build for Linux with release settings
- `android-arm64`: Build for Android with debug settings

### Test Presets
```sh
ctest --preset <test-preset>
```
- `debug`: Test for Linux with debug settings
- `release`: Test for Linux with release settings


## Conventional Commits
The Rapid project uses [Conventional Commits](https://www.conventionalcommits.org/en/v1.0.0/) to standardize commit messages.
This helps in maintaining a clear and consistent commit history, making it easier to understand the changes made over time.

## Pre-commit
The pre-commits feature needs a Python and pip environment.
The pip environment is only needed for the automatically install of the pre-commit Python module.

To install the pre-commit module, run the following command:
```bash
pip install pre-commit
```

To install the git hooks defined in the `.pre-commit-config.yaml` file, run:
```bash
pre-commit install
```

## Include-What-You-Use
This project uses [Include-What-You-Use (IWYU)](https://include-what-you-use.org/) to help manage C++ header file dependencies.
The tool is automatically enabled in CMake builds if it is found in the PATH.

### Installation on Linux
- Arch Linux:
```bash
yay -S include-what-you-use
```

- Ubuntu:
```bash
sudo apt install include-what-you-use
```

Fedora:
```bash
sudo dnf install include-what-you-use
```
