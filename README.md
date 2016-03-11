Experiment `playground' for conducting side-channel research on
the RaspberryPi 3 platform.

First things first, get all the stuff running on the Pi. Clone the repo:

```bash
git clone https://bitbucket.org/jakelongo/multicore.git
```

Pull in and install WiringPi submodule:

```bash
git submodule init
git submodule update
cd WiringPi
./build
cd ..
```

Prep and build the OpenSSL version included:

```bash
cd openssl
./build
make
```


---

Contents of this repo are as follows:

* /openssl - OpenSSL from https://github.com/openssl/openssl.git

* /wiringPi - WiringPi from https://github.com/WiringPi/WiringPi.git the authors
personal git was timing out.

* /featureTest - Quick test for the implemented features on both aarch32|64


  ** wiringPi setup and pin mode   | apps/openssl.c:239
  ** set and clear trigger signals | crypto/aes/asm/aes-armv4.pl:194,292
