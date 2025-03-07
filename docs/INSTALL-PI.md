# Raspberry Pi

Smaller radio systems can be covered using a Raspberry Pi. If you are interested in doing this, you should really get a Pi 4. It maybe possible to get things running on an older Pi, but you often get unexpect behavior and errors. A Pi 4 can handle 3-4 simulatanious recordings. Make sure you have a good power supply. Also pay attention to heat. If the Pi gets too hot, it will slow down. A good case or fan can help keep it going full tilt.

## RaspberryOS (aka Raspbian)

### Setup Raspbian
This is a [good guide](https://desertbot.io/blog/headless-raspberry-pi-4-ssh-wifi-setup) on how to setup a Raspberry Pi in headless mode. This means that you do not have to attach a monitor, keyboard or mouse to it to get it working. The steps below are pulled from this guide.

#### Download and burn the image

The first step is to put the Raspberry Pi OS onto a MicroSD card. You will need to have a USB MicroSD card adaptor, so you can connect it to your computer. Either of these approaches should work:

- Download the [Raspbery OS Desktop](https://www.raspberrypi.org/software/operating-systems/#raspberry-pi-os-32-bit)
- Use [Etcher](https://www.balena.io/etcher/) to burn it to a MicroSD card

OR

- get the [Pi Imager](https://www.raspberrypi.org/software/) which can download and burn an image.


#### Setup for headless boot

After the OS has been written to MicroSD card, we need to change a few files so that the Pi can get on Wifi and also allow for SSH connections. See the [guide](https://desertbot.io/blog/headless-raspberry-pi-4-ssh-wifi-setup) for how to do it using Windows.

- **On a Mac** `touch /Volumes/boot/ssh`
- Next, add the WiFi info
    - `nano /Volumes/boot/wpa_supplicant.conf`

```
country=US
ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev
update_config=1

network={
    ssid="NETWORK-NAME"
    psk="NETWORK-PASSWORD"
}
```
- Eject the MicroSD card (first in the OS and then physhically from the reader)
- Put the MicroSD card in the Pi and power it on.

#### Remote Access

This is a [good guide](https://www.raspberrypi.org/documentation/computers/remote-access.html) for how to find and connect to a Pi on your network. 

*These steps should work on a Mac and assume you only have one Pi on the Network*
- Check to see if it is up: `ping raspberrypi.local`
- Clear old known hosts: `ssh-keygen -R raspberrypi.local`
- See if you can connect: `ssh pi@raspberrypi.local`  *default password is: **raspberry** *
- Exit: `exit`
- Create your SSH keys if you don't have them yet: `ssh-keygen`
- Send over you keys: `ssh-copy-id pi@raspberrypi.local`

### Setup

The following steps setup all of the libraries needed to build Trunk Recorder.

- Add the Debian Multimedia source and include non-free libraries, like **fdkaac**. Edit the sources.list file: 
```bash
sudo nano /etc/apt/sources.list
```
- and add this line to the end:
```
deb http://www.deb-multimedia.org/ buster main non-free
```
- Download the keys for the apt source and install them:
```bash
wget https://www.deb-multimedia.org/pool/main/d/deb-multimedia-keyring/deb-multimedia-keyring_2016.8.1_all.deb
sudo dpkg -i deb-multimedia-keyring_2016.8.1_all.deb
```
- Update the OS:
```
sudo apt update
sudo apt upgrade
```
- Add all of the libraries needed:
```bash
sudo apt -y install libssl-dev openssl curl fdkaac sox libcurl3-gnutls libcurl4 libcurl4-openssl-dev gnuradio gnuradio-dev gr-osmosdr libhackrf-dev libuhd-dev cmake make build-essential libboost-all-dev libusb-1.0-0.dev 
```

Configure RTL-SDRs to load correctly:

```bash
sudo wget https://raw.githubusercontent.com/osmocom/rtl-sdr/master/rtl-sdr.rules  /etc/udev/rules.d/20.rtlsdr.rules
```



Now go [Build](#build-trunk-recorder) Trunk Recorder!


***
## Ubuntu 20.04 Server (64-bit support!)

Ubuntu has a very good [guide](https://ubuntu.com/tutorials/how-to-install-ubuntu-on-your-raspberry-pi#1-overview) on setting up Ubuntu Server to run on a Raspberry Pi. Follow this to get started.


```bash
sudo apt update
sudo apt upgrade
```

```bash
sudo   apt-get install -y apt-transport-https build-essential ca-certificates fdkaac git gnupg gnuradio gnuradio-dev gr-osmosdr libboost-all-dev libcurl4-openssl-dev libgmp-dev libhackrf-dev liborc-0.4-dev libpthread-stubs0-dev libssl-dev libuhd-dev libusb-dev pkg-config software-properties-common cmake sox
```

If you are using a HackRF:

```bash
sudo apt install -y hackrf libhackrf-dev libhackrf0
```

Configure RTL-SDRs to load correctly:

```bash
sudo wget https://raw.githubusercontent.com/osmocom/rtl-sdr/master/rtl-sdr.rules  /etc/udev/rules.d/20.rtlsdr.rules
```

***

# Building Trunk Recorder

In order to keep your copy of the Trunk Recorder source code free of build artifacts created by the build process, it is suggested to create a separate "out-of-tree" build directory. We will use `trunk-build` as our build directory.

Assuming you are in the desired directory to place both trunk-recorder and trunk-build folders to, perform the following...

```bash
mkdir trunk-build
git clone https://github.com/robotastic/trunk-recorder.git
cd trunk-build
cmake ../trunk-recorder
make -j `nproc`
sudo make install
```
Note:  If the Pi hangs during the `make -j 'nproc'` command, try `make` instead (it may take longer but may also prevent locking up the Pi due to all processor cores being 100% in use)

## Profile
(It takes about 15 minutes for this section.)

Run the command `volk_profile` to ensure that [VOLK (Vector-Optimized Library of Kernels)](https://wiki.gnuradio.org/index.php/Volk) uses the best [SIMD (Single instruction, multiple data)](https://en.wikipedia.org/wiki/SIMD) architecture for your processor.

## Configuring Trunk Recorder

The next step is to [configure Trunk Recorder](CONFIGURE.md) for the system you are trying to capture.

## Running trunk recorder. 

If all goes well you should now have the executable named `trunk-recorder`, and created the `config.json` configuration file as described in the [Wiki](https://github.com/robotastic/trunk-recorder/wiki/Configuring-a-System) and [README](https://github.com/robotastic/trunk-recorder/blob/master/README.md#configure).

From your build directory (e.g. `trunk-build`) you can now run
`./trunk-recorder`

### Runtime options

Trunk Recorder will look for a *config.json* file in the same directory as it is being run in. You can point it to a different config file by using the *--config* argument on the command line, for example: `./trunk-recorder --config=examples/config-wmata-rtl.json`.
