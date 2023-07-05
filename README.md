# EspRelayMqtt

Control relay devices through MQTT commands using an ESP8266 board

# Circuit connections

| Esp8266 | Relay |
|---------|-------|
| 3.3v    | DC+   |
| GND     | DC-   |
| D2      | IN    |

# Quick start

## Clone this repository

```
$ git clone git@github.com:knaiskes/EspRelayMqtt.git
$ cd EspRelayMqtt/
```

## Create and activate a Python virtual environment

```
$ python -m venv venv
$ source venv/bin/activate
```

## Install project dependencies

```
$ pip install -r requirements.txt
```

## Add your network and MQTT perferences

Edit ``` src/main.ino ``` file in order to add your network (ssid  and password)
and MQTT (mqtt_server and topic) perferences

## Upload code to the ESP8266

```
$ platformio run --target upload
```
