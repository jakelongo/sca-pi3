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
cd openssl
```

Build OpenSSL and optionally use directives from the build table below
```bash
./build [lib] [defs]
make depend
make
sudo make install
```

## OpenSSL pre-processor directives

|  libraries |  defines |                     Notes                    |
|:----------:|:--------:|:--------------------------------------------:|
| -lwiringPi | AES_TRIG | Trigger placed around each AES enc/dec call. |


---

Contents of this repo are as follows:

* generatePatch.sh - this is used to generate the openssl.patch file

* /openssl - OpenSSL 1.0.1s @57ac73f from https://github.com/openssl/openssl.git

* /wiringPi - WiringPi from https://github.com/WiringPi/WiringPi.git the authors
personal git was timing out.

* /featureTest - Quick test for the implemented features on both aarch32|64

* patchOpenssl.patch

** wiringPi setup and pin mode   | apps/openssl.c:239
** set and clear trigger signals | crypto/aes/asm/aes-armv4.pl:194,292
