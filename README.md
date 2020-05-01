# Zag Bridge

This is an open-source bridge between the 802.15.4 radio interface on a Texas Instruments CC2531 USB dongle and a host computer. A host program is intended to handle all communications, especially [zag-coordinator](https://github.com/srhg/zag-coordinator).

This project is based heavily on Frans-Willem's [ZigbeeRadioBridge](https://github.com/frans-willem/ZigbeeRadioBridge) project.  It uses a more condensed and incompatible serial protocol.

### Libraries and tools used

Zag Bridge uses the legacy [Contiki OS](http://www.contiki-os.org/) for it's radio driver and infrastructure. Furthermore it relies on [Small Device C Compiler (SDCC)](http://sdcc.sourceforge.net/) for its compiler.  SDCC is a moiving target, and contiki is quite picky which version it will work with. Inluded are scripts to setup SDCC and Contiki. Subversion is required to check out SDCC sources.

### Building
To build, execute the following commands:
```
git clone --recursive https://github.com/srhg/zag-bridge-firmware
./setup.sh
source export.sh
make
```

You should then have a file named "zag\_bridge.hex" that you can flash to the dongle using [cc-tool](https://github.com/dashesy/cc-tool/).

```
cc-tool -e -w zag_bridge.hex
```

## License
Zag Bridge is licensed under the GNU General Public License, version v3.0. See LICENSE-gpl-3.0.txt or the [online version](https://www.gnu.org/licenses/gpl-3.0.txt) for more information.

Contiki is licensed under the [3-clause BSD license](https://github.com/contiki-os/contiki/blob/master/LICENSE).
