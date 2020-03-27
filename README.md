# ninjaminer / violetminer

![image](https://upload.wikimedia.org/wikipedia/commons/thumb/8/87/Argon_discharge_tube.jpg/500px-Argon_discharge_tube.jpg)

A CPU miner for Argon2i, Argon2d, and Argon2id.

#### Master Build Status

[![Build Status](https://travis-ci.org/turtlecoin/violetminer.svg?branch=master)](https://travis-ci.org/turtlecoin/violetminer)

#### Development Build Status

[![Build Status](https://travis-ci.org/turtlecoin/violetminer.svg?branch=development)](https://travis-ci.org/turtlecoin/violetminer)

## Download

[Go here to download the latest release.](https://github.com/NinjaCoin-Master/violetminer/releases/) 

If you prefer to compile yourself, read on. This can result in increased hashrates in some cases.

## Algorithms Supported
* NinjaCoin - choose `ninjacoin` or `ninja`
* TurtleCoin - choose `turtlecoin` or `chukwa`
* WrkzCoin - choose `wrkzcoin` or `chukwa_wrkz`

## Notes

* Supports AVX-512, AVX-2, SSE4.1, SSSE3, SSE2 and NEON optimizations.
* Pools are tried top to bottom, the lower a pool, the lower it's priority. If we are not connected to the highest priority pool, we will continuously retry connecting to higher priority pools.
* Dev fee is 1%.
* Supports [xmrig-proxy](https://github.com/xmrig/xmrig-proxy) - Make sure to enable `"niceHash": true` in your pool config.

## Configuring

There are a couple of ways to configure the miner.

* Just start it, and walk throught the guided setup. Upon completion, the config will be written to `config.json` for modification.
* Use command line options. Use `violetminer --help` to list them all. It is not recommended to use command line options, as they are less configurable than the config.
* Copy the below config to `config.json` and modify to your purposes.

```json
{
    "optimizationMethod": "Auto",
    "pools": [
        {
            "agent": "",
            "algorithm": "turtlecoin",
            "host": "publicnode.ydns.eu",
            "niceHash": false,
            "password": "",
            "port": 4666,
            "rigID": "",
            "username": "TRTLv2Fyavy8CXG8BPEbNeCHFZ1fuDCYCZ3vW5H5LXN4K2M2MHUpTENip9bbavpHvvPwb4NDkBWrNgURAd5DB38FHXWZyoBh4wW"
        },
        {
            "agent": "violetminer-v0.0.3",
            "algorithm": "wrkzcoin",
            "host": "139.162.29.140",
            "niceHash": false,
            "password": "x",
            "port": 4444,
            "rigID": "rig1",
            "username": "WrkzjJMM8h9F8kDU59KUdTN8PvZmzu2HchyBG15R4SjLD4EcMg6qVWo3Qeqp4nNhgh1CPL7ixCL1P4MNwNPr5nTw11ma1MMXr7"
        }

    ],
    "threadCount": 12
}
```

### Optimization method

By default, the program will automatically choose the optimization method to use.

In some cases, you may find you get better performance by manually specifying the optimization method to use.

You can, if desired, use a different optimization method, or disable optimizations altogether.

Note that you can only use optimizations that your hardware has support for - these are printed at startup.

Simply set the desired value in the `optimizationMethod` config field.

Available optimizations for each platform are as follows:

#### x86_64 (64 bit Intel/AMD Windows/Mac/Linux)

* `AVX-512`
* `AVX-2`
* `SSE4.1`
* `SSSE3`
* `SSE2`
* `None`
* `Auto`

#### ARMv8

Note: On ARMv8, `Auto` uses no optimizations. From my testing, the NEON implementation actually performs worse than the reference implementation. You may want to experiment with toggling between `NEON` and `None` if you are on an ARM machine.

* `NEON`
* `None`
* `Auto`

#### Anything else

* `None`
* `Auto`

## Compiling

### Windows

- Download the [Build Tools for Visual Studio 2019](https://visualstudio.microsoft.com/thank-you-downloading-visual-studio/?sku=BuildTools&rel=16) Installer
- When it opens up select **C++ build tools**, it automatically selects the needed parts
<!---
- Install the latest full version of OpenSSL if you want to compile with SSL support. (currently OpenSSL 1.1.1c). Select the appropriate version for your system:
  - [OpenSSL 64-bit](https://slproweb.com/download/Win64OpenSSL-1_1_1c.exe)
  - [OpenSSL 32-bit](https://slproweb.com/download/Win32OpenSSL-1_1_1c.exe)
  -->

For 64-bit:
- From the start menu, open 'x64 Native Tools Command Prompt for VS 2019'.
- `git clone https://github.com/turtlecoin/violetminer`
- `cd violetminer`
- `git submodule update --init --recursive`
- `mkdir build`
- `cd build`
- `cmake -G "Visual Studio 16 2019" -A x64`
- `MSBuild violetminer.sln /p:Configuration=Release /m`

For 32-bit:
- From the start menu, open 'x86 Native Tools Command Prompt for VS 2019'.
- `git clone https://github.com/turtlecoin/violetminer`
- `cd violetminer`
- `git submodule update --init --recursive`
- `mkdir build`
- `cd build`
- `cmake -G "Visual Studio 16 2019" -A Win32 ..`
- `MSBuild violetminer.sln /p:Configuration=Release /p:Platform=Win32 /m` 

### Linux

**If you are on x86-64 (If you're not sure, you almost certainly are) It's highly recommended to use Clang to compile. It gets better hashrate for many people.**

For example, I get 7300h/s with GCC, and 10200h/s with Clang on a Ryzen 1600.

If you're on ARM however, GCC gets slightly better hashrate.

#### Ubuntu, using Clang

- `sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y`
- `wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | sudo apt-key add -`

You need to modify the below command for your version of ubuntu - see https://apt.llvm.org/

* Ubuntu 14.04 (Trusty)
- `sudo add-apt-repository "deb https://apt.llvm.org/trusty/ llvm-toolchain-trusty 6.0 main"`

* Ubuntu 16.04 (Xenial)
- `sudo add-apt-repository "deb https://apt.llvm.org/xenial/ llvm-toolchain-xenial 6.0 main"`

* Ubuntu 18.04 (Bionic)
- `sudo add-apt-repository "deb https://apt.llvm.org/bionic/ llvm-toolchain-bionic 6.0 main"`

- `sudo apt-get update`
- `sudo apt-get install aptitude -y`
- `sudo aptitude install -y -o Aptitude::ProblemResolver::SolutionCost='100*canceled-actions,200*removals' build-essential clang-6.0 libstdc++-7-dev git python-pip libssl-dev`
- `sudo pip install cmake`
- `export CC=clang-6.0`
- `export CXX=clang++-6.0`
- `git clone https://github.com/turtlecoin/violetminer`
- `cd violetminer`
- `git submodule update --init --recursive`
- `mkdir build`
- `cd build`
- `cmake ..`
- `make`

#### Ubuntu, using GCC

- `sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y`
- `sudo apt-get update`
- `sudo apt-get install aptitude -y`
- `sudo aptitude install -y build-essential g++-8 gcc-8 git python-pip libssl-dev`
- `sudo pip install cmake`
- `export CC=gcc-8`
- `export CXX=g++-8`
- `git clone https://github.com/turtlecoin/violetminer`
- `cd violetminer`
- `git submodule update --init --recursive`
- `mkdir build`
- `cd build`
- `cmake ..`
- `make`

#### Generic Linux

Reminder to use clang if possible. Make sure to set `CC` and `CXX` to point to `clang` and `clang++` as seen in the Ubuntu instructions.

- `git clone https://github.com/turtlecoin/violetminer`
- `cd violetminer`
- `git submodule update --init --recursive`
- `mkdir build`
- `cd build`
- `cmake ..`
- `make`

### Android Cross Compile

Using [this](https://android.googlesource.com/platform/ndk/+/ndk-release-r20/build/cmake/android.toolchain.cmake) toolchain

ANDROID_ABI can be 

* armeabi-v7a
* arm64-v8a
* x86
* x86_64

Set this depending on the architecture of the phone you want to run it on.

- `git clone https://github.com/turtlecoin/violetminer`
- `cd violetminer`
- `git submodule update --init --recursive`
- `mkdir build`
- `cd build`
- `cmake -DCMAKE_TOOLCHAIN_FILE="${HOME}/Android/sdk/android-ndk-r20/build/cmake/android.toolchain.cmake" -DANDROID_ABI=arm64-v8a -DANDROID_CROSS_COMPILE=ON ..`
- `make`

## Developing

* Update submodules to latest commit: `git submodule foreach git pull origin master`
