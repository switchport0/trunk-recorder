# Configuring Trunk Recorder

It takes a little bit of work to correctly configure Trunk Recorder, but once you get everything working you will not have to touch it again.

## Research

Before you can start entering values, you will need to do a little research about the radio system you are trying to capture and the correct parameters for receiving it. [Radio Reference](http://www.radioreference.com/apps/db/?coid=1) is a great place to learn about a radio system. Search for your location and then select the system you are trying to record. Take note of the frequencies that the system uses. You will want to make sure you can cover the range of frequencies used with one or more SDRs. Also look at the System Type, which tells you if it is a Trunked system and what type it is. For Trunked systems, you will need to write down the control channels, and alternate control channels.

### Frequency

![](media/gqrx.png)

The next step is to try and receive the control channel for the trunked system, using [http://gqrx.dk/](GQRX). GQRX visualizes what your SDR is receiving and makes it easy to fine-tune the system and associated spectrum. While the system you are trying to tune in may have a lot of control channels, it is generally only transmitting on one. Type in the different frequencies to look for the active control channel. Control channels are always broadcasting, and show up as a persistent line on the waterfall graph.

There is a good chance that when you tune to the active control channel, it will actually be a few thousand Hz above or below the frequency you tuned to. This is because the tuners on most SDRs are not super accurate and have frequency drifting. Click on the transmission to get the frequency that your SDR thinks it is at.

Trunk Recorder needs to know the amount of tuning error for your SDR in order to successfully tune-in to transmissions. To calculate this, take frequency that the SDR was tuned to... for example 854.548MHz, and subtract the actual frequency for the channel, 854.5625MHz.

`854.548 - 854.5625 = -0.0145 MHz`

You then have to convert that from MHz to Hz, so multiply your answer by 1,000,000:

`-0.0145 * 1000000 = -14500 Hz`

The amount of tuning error is -14500Hz, so that would go under **error:** for this *source* in the `config.json` file.

**NOTE:** In some instances, an alternative is to use `ppm` correction rather than the `error` configuration option.

Alternatively, you can use this tool here: http://garvas.org/trunk-recorder/ to obtain RTL-SDR dongle/array configurations. 

### Gain

After you have figured out the amount of tuning error, you now need to find the optimal amount of receiver gain to use. 

Gain is a measure of how much amplification is required for the received signal, and on some SDRs, there are multiple places along the receive path where a signal can be amplified. 

If there is not enough gain, the signal will not be strong enough to decode, and Trunk-Recorder will fail to lock to the control channel. If there is too much gain, it can distort the signal, there is also the chance you might be causing harm to your SDR reception device. Setting the gain too high will result in amplification of the background RF and create noise. 

Generally, you can mess around with the gain slider in GQRX until the signal looks well defined and there isn't too much noise. If it is impossible to get a well-defined signal, it could be a sign that you have one or more issues: a better antenna that is tuned to the needed frequency range, moving the antenna to a new location, or using a different SDR device. There could also be some strong interference nearby, which can introduce a lot of background noise making it tough to distinguish the signal. Various computer hardware, poorly grounded hardware, and cheaply made USB hubs can be notorious for producing RF noise across the entire spectrum. 

Once you find the correct gain settings, use them for this source in the `config.json` file.

### Center Frequency
When you set the center frequency for a source, **you are picking the frequency that will be in the _middle_ of the block of spectrum that you are recording**. Half of the bandwidth for the device will be above that frequency and the other half below. 

For example, if you are using a HackRF, with 8MHz of bandwidth, and you tune the center frequency to 854MHz, it would cover from 850.0MHz to 858.0MHz. 

To find your ideal center frequency, look at what the lowest frequency you want to cover is and what the highest is. You need to need to be able cover slightly beyond the frequncy of a channel. This is because the frequency is for the center of the channel and the actual channel is wider and a bit of filtering is done to receive it. The sample rate should be higher than the difference between the low and high frequency. Most SDRs do not perform as well right at the beginnging and end of the frequency range they are set to. It is best to set a slightly higher sample rate than needed, to avoid those spots. Also, some SDRs have some artifacts right at there center frequency, so ensure that center frequency doesn't land on the frequency of a channel you are trying to record.

### Multiple Sources
If the low frequency and high frequency of the system you are trying to capture is greater than the amount of bandwidth your SDR can capture, you need to use multiple SDRs. 

In addition to being able to use a cheaper SDR, it also helps with general performance of the devices. When a single SDR is used, each of the Recorders gets fed all of the sampled signals. Each Recorder needs to cut down the multi-mega samples per second into a small 12.5Khz or even 6.25Khz(!) slivers. 

When you use multiple SDRs, each SDR is capturing only a partial slice of the system so the recorders have to cut down a much smaller amount of sample to get to the sliver they are interested in. This ultimately denotes that you can have a lot more recorders running!

To use multiple SDRs, simply define additional Sources in the Source array. The config-multi-rtl.json.sample has an example of how to do this. In order to tell the different SDRs apart and make sure they get the right error correction value, give them a serial number using the `rtl_eeprom -s` command and then specifying that number in the device setting for that Source, `rtl=2`.

---

## The config.json file

Trunk Recorder is configured using a JSON formatted file. It defines the SDRs that are available and the trunk system that will be recorded. Trunk Recorder will look for a *config.json* file in the same directory as it is being run in. You can point it to a different config file by using the *--config* argument on the command line, for example: `./trunk-recorder --config=examples/config-wmata-rtl.json`. The following is an example for my local system in DC, using an Ettus B200:

```json
{
    "ver": 2,
    "sources": [{
        "center": 857000000.0,
        "rate": 8000000.0,
        "error": 0,
        "gain": 40,
        "antenna": "TX/RX",
        "digitalRecorders": 2,
        "driver": "usrp",
        "device": ""
    }],
    "systems": [{
        "control_channels": [855462500],
        "type": "p25",
        "talkgroupsFile": "ChanList.csv",
        "unitTagsFile": "UnitTags.csv",
        "squelch": -50,
        "modulation": "qpsk"
    }]
}
```


Here is a map of the different sections of the *config.json* file:

```json
{
	Global Configs
	
	"sources": [{ Source Object }, { Source Object }],
	"systems": [{ System Object }, { System Object }],
	"plugins": [{ Plugin Object }]
}
```



### Global Configs

| Key                     | Required | Default Value     | Type                                                        | Description                                                  |
| ----------------------- | :------: | ----------------- | ----------------------------------------------------------- | ------------------------------------------------------------ |
| ver                     |    ✓     |                   | number                                                      | the version of formatting for the config file. **This should be set to 2**. Trunk Recorder will not start without this set. |
| sources                 |    ✓     |                   | array of JSON objects<br />[{}]                             | An array of JSON formatted [Source Objects](#source-object) that define the different SDRs available. Source Objects are described below. |
| systems                 |    ✓     |                   | array of JSON objects<br />[{}]                             | An array of JSON formatted [System Objects](#system-object) that define the trunking systems that will be recorded. System Objects are described below. |
| plugins                 |          |                   | array of JSON objects<br />[{}]                             | An array of JSON formatted [Plugin Objects](#plugin-object) that define the different plugins to use. Refer to the [Plugin System](notes/PLUGIN-SYSTEM.md) documentation for more details. |
| defaultMode             |          | "digital"         | **"analog"** or **"digital"**                               | Default mode to use when a talkgroups is not listed in the **talkgroupsFile**. The options are *digital* or *analog*. The default is *digital*. This argument is global and not system-specific, and only affects `smartnet` trunking systems which can have both analog and digital talkpaths. |
| captureDir              |          | current directory | string                                                      | The complete path to the directory where recordings should be saved. |
| callTimeout             |          | 3                 | number                                                      | A Call will stop recording and save if it has not received anything on the control channel, after this many seconds. |
| uploadServer            |          |                   | string                                                      | The URL for uploading to OpenMHz. The default is an empty string. See the Config tab for your system in OpenMHz to find what the value should be. |
| broadcastifyCallsServer |          |                   | string                                                      | The URL for uploading to Broadcastify Calls. The default is an empty string. Refer to [Broadcastify's wiki](https://wiki.radioreference.com/index.php/Broadcastify-Calls-API) for the upload URL. |
| logFile                 |          | false             | **true** / **false**                                        | Save the console output to a file.                           |
| frequencyFormat         |          | "exp"             | **"exp" "mhz"** or **"hz"**                                 | the display format for frequencies to display in the console and log file. |
| controlWarnRate         |          | 10                | number                                                      | Log the control channel decode rate when it falls bellow this threshold. The value of *-1* will always log the decode rate. |
| statusAsString          |          | true              | **true** / **false**                                        | Show status as strings instead of numeric values             |
| statusServer            |          |                   | string                                                      | The URL for a WebSocket connect. Trunk Recorder will send JSON formatted update message to this address. HTTPS is currently not supported, but will be in the future. OpenMHz does not support this currently. [JSON format of messages](STATUS-JSON.md) |
| broadcastSignals        |          | true              | **true** / **false**                                        | Broadcast decoded signals to the status server.              |
| logLevel                |          | "info"            | **"trace" "debug" "info" "warning" "error"** or **"fatal"** | the logging level to display in the console and log file. The options are *trace*, *debug*, *info*, *warning*, *error* & *fatal*. The default is *info*. |
| debugRecorder           |          | true              | **true** / **false**                                        | Will attach a debug recorder to each Source. The debug recorder will allow you to examine the channel of a call be recorded. There is a single Recorder per Source. It will monitor a recording and when it is done, it will monitor the next recording started. The information is sent over a network connection and can be viewed using the `udp-debug.grc` graph in GnuRadio Companion |
| debugRecorderPort       |          | 1234              | number                                                      | The network port that the Debug Recorders will start on. For each Source an additional Debug Recorder will be added and the port used will be one higher than the last one. For example the ports for a system with 3 Sources would be: 1234, 12345, 1236. |
| debugRecorderAddress    |          | "127.0.0.1"       | string                                                      | The network address of the computer that will be monitoring the Debug Recorders. UDP packets will be sent from Trunk Recorder to this computer. The default is *"127.0.0.1"* which is the address used for monitoring on the same computer as Trunk Recorder. |



#### Source Object

| Key              | Required | Default Value | Type                                                         | Description                                                  |
| :--------------- | :------: | :-----------: | ------------------------------------------------------------ | ------------------------------------------------------------ |
| center           |    ✓     |               | number                                                       | the center frequency in Hz to tune the SDR to                |
| rate             |    ✓     |               | number                                                       | the sampling rate to set the SDR to, in samples / second     |
| error            |          |       0       | number                                                       | the tuning error for the SDR, in Hz. This is the difference between the target value and the actual value. So if you wanted to recv 856MHz but you had to tune your SDR to 855MHz (when set to 0ppm)  to actually receive it, you would set this to -1000000. You should also probably get a new SDR if it is off by this much. |
| gain             |    ✓     |               | number                                                       | The RF gain setting for the SDR. Use a program like GQRX to find a good value. |
| digitalRecorders |          |               | number                                                       | The number of Digital Recorders to have attached to this source. This is essentially the number of simultaneous calls you can record at the same time in the frequency range that this Source will be tuned to. It is limited by the CPU power of the machine. Some experimentation might be needed to find the appropriate number. |
| analogRecorders  |          |               | number                                                       | the number of Analog Recorder to have attached to this source. This is the same as Digital Recorders except for Analog Voice channels. |
| driver           |    ✓     |               | **"usrp"** or **"osmosdr"**                                  | The GNURadio block you wish to use for the SDR.              |
| device           |          |               | **string**<br /> See the [osmosdr page](http://sdr.osmocom.org/trac/wiki/GrOsmoSDR) for supported devices and parameters. | Osmosdr device name and possibly serial number or index of the device. <br /> You only need to do add this key if there are more than one osmosdr devices being used.<br /> Example: `bladerf=00001` for BladeRF with serial 00001 or `rtl=00923838` for RTL-SDR with serial 00923838, just `airspy` for an airspy.<br />It seems that when you have 5 or more RTLSDRs on one system you need to decrease the buffer size. I think it has something to do with the driver. Try adding buflen: `"device": "rtl=serial_num,buflen=65536"`, there should be no space between buflen and the comma |
| ppm              |          |       0       | number                                                       | the tuning error for the SDR in ppm (parts per million), as an alternative to `error` above. Use a program like GQRX to find an accurate value. |
| agc              |          |     false     | **true** / **false**                                         | whether or not to enable the SDR's automatic gain control (if supported). This is false by default. It is not recommended to set this as it often yields worse performance compared to a manual gain setting. |
|                  |          |               |                                                              |                                                              |
| ifGain           |          |               | number                                                       | [AirSpy/hackrf only] sets the **if** gain.                   |
| bbGain           |          |               | number                                                       | [hackrf only] sets the **bb** gain.                          |
| mixGain          |          |               | number                                                       | [AirSpy only] sets the **mix** gain.                         |
| lnaGain          |          |               | number                                                       | [AirSpy/bladeRF only] sets the **lna** gain.                 |
| vga1Gain         |          |               | number                                                       | [bladeRF only] sets the **vga1** gain.                       |
| vga2Gain         |          |               | number                                                       | [bladeRF only] sets the **vga2** gain.                       |
| antenna          |          |               | string, e.g.: **"TX/RX"**                                    | [usrp] selects which antenna jack to use                     |
| debugRecorders   |          |               | number                                                       | the number of Debug Recorder to have attached to this source. Debug Recorders capture a raw sample that you can examine later using GNURadio Companion. This is helpful if you want to fine tune your the error and gain for this Source. |



#### System Object

| Key                    | Required | Default Value                  | Type                                                         | Description                                                  |
| ---------------------- | :------: | ------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| shortName              |    ✓     |                                | string                                                       | This is a nickname for the system. It is used to help name and organize the recordings from this system. It should be 4-6 letters with no spaces. |
| type                   |    ✓     |                                | **"smartnet"  "p25"   "conventional"** or **"conventionalP25"** | The type of radio system.                                    |
| control_channels       |    ✓     |                                | array of numbers;<br />[496537500, 496437500]                | **For trunked systems** The control channel frequencies for the system, in Hz. The frequencies will automatically be cycled through if the system moves to an alternate channel. |
| channels               |    ✓     |                                | array of numbers;<br />[166725000, 166925000, 167075000, 166850000] | **For conventional systems**  The channel frequencies, in Hz, used for the system. The channels get assigned a virtual talkgroup number based upon their position in the array. Squelch levels need to be specified for the Source(s) being used. |
| modulation             |          | "qpsk"                         | **"qpsk"** or  **"fsk4"**                                    | The type of digital modulation that the system uses          |
| squelch                |          | 0 <br />(which means disabled) | number                                                       | Squelch in DB, this needs to be set for all conventional systems. The squelch setting is also used for analog talkgroups in a SmartNet system. I generally use -60 for my rtl-sdr. |
| talkgroupsFile         |          |                                | string                                                       | The filename for a CSV file that provides information about the talkgroups. It determines whether a talkgroup is analog or digital, and what priority it should have. This file should be located in the same directory as the trunk-recorder executable. |
| apiKey                 |          |                                | string                                                       | [*if uploadServer is set*] System-specific API key for uploading calls to OpenMHz.com. See the Config tab for your system in OpenMHz to find what the value should be. |
| broadcastifyApiKey     |          |                                | string                                                       | [*if broadcastifyCallsServer is set*] System-specific API key for Broadcastify Calls |
| broadcastifySystemId   |          |                                | number                                                       | [*if broadcastifyCallsServer is set*] System ID for Broadcastify Calls <br />(this is an integer, and different from the RadioReference system ID) |
| uploadScript           |          |                                | string                                                       | This is the filename of a script that is called after each recording has finished. Checkout *encode-upload.sh.sample* as an example. The script should be located in the same directory as the trunk-recorder executable. |
| compressWav            |          | true                           | bool                                                         | Convert the recorded .wav file to an .m4a file. **This is required for both OpenMHz and Broadcastify!** The `sox` and `fdkaac` packages need to be installed for this command to work. |
| unitScript             |          |                                | string                                                       | This is the filename of a script that runs when a radio (unit) registers (is turned on), affiliates (joins a talk group), deregisters (is turned off), sends an acknowledgment response or transmits. Passed as parameters:  `shortName radioID on|join|off|ackresp|call`. On joins and transmissions, `talkgroup` is passed as a fourth parameter. See *examples/unit-script.sh* for a logging example. Note that for paths relative to recorder, this should start with `./`( or `../`). |
| audioArchive           |          | true                           | **true** / **false**                                         | Should the recorded audio files be kept after successfully uploading them? |
| transmissionArchive    |          | false                          | **true** / **false**                                         | Should each of the individual transmission be kept? These transmission are combined together with other recent ones to form a single call. |
| callLog                |          | false                          | **true** / **false**                                         | Should a json file with the call details be kept after successful uploads? |
| analogLevels           |          | 8                              | number (1-32)                                                | The amount of amplification that will be applied to the analog audio. |
| maxDev                 |          | 4000                           | number                                                       | Allows you to set the maximum deviation for analog channels. If you analog recordings sound good or if you have a completely digital system, then there is no need to touch this. |
| digitalLevels          |          | 1                              | number (1-16)                                                | The amount of amplification that will be applied to the digital audio. |
| alphatags              |          |                                | array of strings<br />e.g.: ["police", "fire", "ems"]        | [*For conventional systems*] these tags will be displayed in the log files to show what each frequency is used for. They will be applied to the *channels* in the order the values appear in the array. |
| unitTagsFile           |          |                                | string                                                       | This is the filename of a CSV files that provides information about the unit tags. It allows a Unit ID to be assigned a name. This file should be located in the same directory as the trunk-recorder executable. The format is 2 columns, the first being the decimal number of the Unit ID, the second is the Unit Name, |
| recordUnknown          |          | true                           | **true** / **false**                                         | Record talkgroups if they are not listed in the Talkgroups File. |
| hideEncrypted          |          | false                          | **true** / **false**                                         | Hide encrypted talkgroups log entries                        |
| hideUnknownTalkgroups  |          | false                          | **true** / **false**                                         | Hide unknown talkgroups log entries                          |
| minDuration            |          | 0<br />(which is disabled)     | number                                                       | The minimum call (transmission) duration in seconds (decimals allowed), calls below this number will have recordings deleted and will not be uploaded. |
| talkgroupDisplayFormat |          | "id"                           | **"id" "id_tag"** or **"tag_id"**                            | The display format for talkgroups in the console and log file. (*id_tag* and *tag_id* is only valid if **talkgroupsFile** is specified) |
| bandplan               |          | "800_standard"                 | **"800_standard" "800_reband" "800_splinter"** or **"400_custom"** | [SmartNet only] this is the SmartNet bandplan that will be used. |
| bandplanBase           |          |                                | number                                                       | [SmartNet, 400_custom only] this is for the *400_custom* bandplan only. This is the base frequency, specified in Hz. |
| bandplanHigh           |          |                                | number                                                       | [SmartNet, 400_custom only] this is the highest channel in the system, specified in Hz. |
| bandplanSpacing        |          |                                | number                                                       | [SmartNet, 400_custom only] this is the channel spacing, specified in Hz. Typically this is *25000*. |
| bandplanOffset         |          |                                | number                                                       | [SmartNet, 400_custom only] this is the offset used to calculate frequencies. |
| decodeMDC              |          | false                          | **true** / **false**                                         | [ Conventional systems only ] enable the MDC-1200 signaling decoder. |
| decodeFSync            |          | false                          | **true** / **false**                                         | [ Conventional systems only ] enable the Fleet Sync signaling decoder. |
| decodeStar             |          | false                          | **true** / **false**                                         | [ Conventional systems only ] enable the Star signaling decoder. |
| decodeTPS              |          | false                          | **true** / **false**                                         | [ Conventional systems only ] enable the Motorola Tactical Public Safety (aka FDNY Fireground) signaling decoder. |



#### Plugin Object

| Key     | Required | Default Value | Type   | Description                                                  |
| ------- | :------: | ------------- | ------ | ------------------------------------------------------------ |
| library |    ✓     |               | string | the name of the library that contains the plugin.            |
| name    |    ✓     |               | string | the name of the plugin. This name is used to find the `<name>_plugin_new` method that creates a new instance of the plugin. |
|         |          |               |        | *Additional elements can be added, they will be passed into the `parse_config` method of the plugin.* |


## talkgroupsFile

This file provides info on the different talkgroups in a trunking system. A lot of this info can be found on the [Radio Reference](http://www.radioreference.com/) website. You need to be a Radio Reference member to download the table for your system preformatted as a CSV file. If you are not a Radio Reference member, try clicking on the "List All in one table" link, selecting everything in the table and copying it into Excel or a spreadsheet, and then exporting or saving as a CSV file.

**Note** - Fields in preformatted CSV downloads from Radio Reference are now in a different order than Trunk Recorder expects. See below for the correct field order. Additionally, Radio Reference inserts a header line at the tope of the CSV file which should be removed.

You may add an additional column that adds a priority for each talkgroup. The priority field specifies the number of recorders the system must have available to record a new call for the talkgroup. For example, a priority of 1, the highest means as long as at least a single recorder is available, the system will record the new call. If the priority is 2, the system would at least 2 free recorders to record the new call, and so on. If there is no priority set for a talkgroup entry, a prioity of 1 is assumed.

The Trunk Record program really only uses the priority information and the Dec Talkgroup ID. The Website uses the same file though to help display information about each talkgroup.

Here are the column headers and some sample data:

| DEC |	HEX |	Mode |	Alpha Tag	| Description	| Tag |	Group | Priority |
|-----|-----|------|-----------|-------------|-----|-------|----------|
|101	| 065	| D	| DCFD 01 Disp	| 01 Dispatch |	Fire Dispatch |	Fire | 1 |
|2227 |	8b3	| D	| DC StcarYard	| Streetcar Yard |	Transportation |	Services | 3 |

