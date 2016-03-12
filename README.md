Experiment `playground' for conducting side-channel research on
the RaspberryPi 3 platform.

# Build

First things first, get all the stuff running on the Pi. Clone the repo:

```bash
git clone https://github.com/jakelongo/sca-pi3.git
```

Pull in OpenSSL and WiringPi:

```bash
git submodule update --init
```

Build and install WiringPi:

```bash
cd WiringPi
./build
cd ..
```

Patch OpenSSL:

```bash
patch -p0 < openssl.patch
```

Build OpenSSL and optionally set flags from the build table below:
```bash
cd openssl
./build [lib] [defs]
make depend
make
sudo make install_sw
```

#### OpenSSL linker and pre-processor flags:

|  lib       |  defs       |                     Notes                    |
|:----------:|:-----------:|:--------------------------------------------:|
| -lwiringPi | -DAES_TRIG  | Trigger placed around each AES enc/dec call. |


---

Contents of this repo are as follows:

* generatePatch.sh - generates the openssl.patch file

* /openssl - OpenSSL 1.0.1s @57ac73f from
  https://github.com/openssl/openssl.git

* /wiringPi - WiringPi from https://github.com/WiringPi/WiringPi.git the
  authors personal git was timing out

* /featureTest - tests for implemented ARMv8 SoC features on both aarch32|64

* openssl.patch - changes to the openssl source to aid in the SCA signal
  exploration phase
