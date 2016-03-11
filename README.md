Experiment `playground' for conducting side-channel research on
the RaspberryPi 3 platform.

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

Patch and build OpenSSL:

```bash
cd openssl
./build
make
```

---

Contents of this repo are as follows:

* /openssl - OpenSSL 1.0.2g @902f3f5 from https://github.com/openssl/openssl.git

* /wiringPi - WiringPi from https://github.com/WiringPi/WiringPi.git the authors
personal git was timing out.

* /featureTest - Quick test for the implemented features on both aarch32|64
** wiringPi setup and pin mode   | apps/openssl.c:239
** set and clear trigger signals | crypto/aes/asm/aes-armv4.pl:194,292
