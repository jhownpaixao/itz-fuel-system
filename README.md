# ITZ Fuel System

A comprehensive fuel system mod for DayZ that adds realistic fuel management for vehicles and fuel stations.

## Features

- ✅ Individual fuel station configuration
- ✅ Custom vehicle fuel consumption rates
- ✅ Multiple fuel types (Gasoline, Diesel, Kerosene)
- ✅ Power generator system with overload protection
- ✅ Automatic fuel respawn system
- ✅ Persistent fuel station states
- ✅ XML-based configuration for easy management

---

## Configuration Files

All configuration files are located in: `<ServerProfile>/iTzMods/FuelSystem/`

### 📋 stations.xml

Defines fuel stations, their positions, fuel types, and behavior.

#### Default Settings

The `<defaults>` tag defines global settings for all fuel stations:

- **respawntime** - Time in seconds before fuel respawns
- **range** - Distance around the station where refueling can occur (should be equal to or greater than the furthest pump)
- **flags** - Additional station configuration:
  - **require_energy** - Whether the station requires power to operate
  - **respawn_fuel** - If TRUE, fuel respawns at intervals; if FALSE, only respawns on server restart
  - **use_destroyed** - Whether the station can refuel even when destroyed
  - **can_measure** - Whether players can measure fuel quantity at pumps

#### Station Configuration

The `<station>` tag defines each fuel station:

- **name** - Station name
- **pos** - Station position on the map (X Y Z coordinates)
- **fuel** - Fuel type definition:
  - **type** - Fuel type (`GASOLINE`, `DIESEL`, `KEROSENE`)
  - **capacity** - Maximum tank capacity in liters
  - **min/max** - Minimum and maximum fuel amounts in liters
    - If **respawn_fuel = TRUE**: Amount that respawns at each `<respawntime>` interval
    - If **respawn_fuel = FALSE**: Initial amount on server start

#### Example

```xml
<stations>
    <defaults>
        <respawntime value="60" />
        <range value="50" />
        <flags require_energy="1" respawn_fuel="1" use_destroyed="0" can_measure="1" />
    </defaults>

    <station name="Main Gas Station" pos="1234.5 67.8 91011.12">
        <fuel type="GASOLINE" capacity="50.0" min="5.0" max="15.0" />
        <fuel type="DIESEL" capacity="60.0" min="10.0" max="20.0" />
        
        <!-- Override defaults for this station -->
        <respawntime value="90" />
        <range value="100" />
        <flags require_energy="0" respawn_fuel="1" use_destroyed="1" can_measure="1" />
    </station>
</stations>
```

---

### 📋 vehicles.xml

Defines vehicle fuel types and consumption rates.

- **type** - Vehicle classname (e.g., `CarScript`, `CivilianSedan`, `Truck_01_Base`)
- **fuel** - Fuel type required (`GASOLINE`, `DIESEL`, `KEROSENE`)
- **consumption** - Fuel consumption rate in liters per kilometer

#### Example

```xml
<vehicles>
    <vehicle type="CarScript" fuel="GASOLINE" consumption="30.15" />
    <vehicle type="BoatScript" fuel="GASOLINE" consumption="30.15" />
    <vehicle type="CivilianSedan" fuel="GASOLINE" consumption="30.22" />
    <vehicle type="Hatchback_02" fuel="GASOLINE" consumption="30.11" />
    <vehicle type="Sedan_02" fuel="GASOLINE" consumption="30.13" />
    <vehicle type="OffroadHatchback" fuel="DIESEL" consumption="30.21" />
    <vehicle type="Offroad_02" fuel="DIESEL" consumption="30.24" />
    <vehicle type="Truck_01_Base" fuel="DIESEL" consumption="30.4" />
    <vehicle type="ExpansionHelicopterScript" fuel="KEROSENE" consumption="30.3" />
</vehicles>
```

---

### 📋 persistente.json

**⚠️ WARNING: This file is automatically managed by the mod. Do not edit while the server is running!**

This file stores the current state of each fuel station, including:
- Current fuel amounts for each fuel type
- Station status and configuration

**To manually edit this file, the server must be offline.**

The mod automatically saves and loads this file to persist fuel station states across server restarts.

#### Example Structure

```json
[
    {
        "m_Classname": "CarScript",
        "m_FuelType": "GASOLINE",
        "m_FuelConsumptionPerKm": 30.15
    }
]
```

---

### 📋 Settings.json

Server-wide configuration for the fuel system.

#### Configuration Options

| Setting | Type | Description | Default |
|---------|------|-------------|---------|
| `m_Version` | int | Configuration version (auto-managed) | - |
| `m_ClientSyncCheckInterval` | int | Interval in seconds to sync fuel stations with clients | 1 |
| `m_DataSaveInterval` | int | Interval in seconds to save station states | 3 |
| `m_UseGeneratorOverloadSystem` | bool | Enable generator overload protection system | false |
| `m_GeneratorOverloadActiveTime` | int | Time in seconds before generator overloads | 240 |
| `m_GeneratorOverloadCooldown` | int | Cooldown time in seconds after overload warning | 30 |
| `m_RefillSelectionMaxRange` | int | Maximum distance in meters to select a fuel pump for refueling | 6 |
| `m_VehicleRefuelDirectly` | bool | Allow vehicles to refuel directly at fuel pumps | true |
| `m_LogLevel` | int | Log level (0=None, 1=Error, 2=Warning, 3=Info, 4=Debug) | 4 |

#### Deprecated Settings

The following settings are deprecated and moved to individual station configuration in `stations.xml`:
- `m_FuelRespawnInterval`
- `m_FuelStationRange`
- `m_FuelPumpRequiresPower`
- `m_FuelPumpPowerConsumptionPerSec`
- `m_UseDestroyedPump`
- `m_CanMeasureFuel`

#### Example

```json
{
    "m_Version": 102,
    "m_ClientSyncCheckInterval": 1,
    "m_DataSaveInterval": 3,
    "m_UseGeneratorOverloadSystem": false,
    "m_GeneratorOverloadActiveTime": 240,
    "m_GeneratorOverloadCooldown": 30,
    "m_RefillSelectionMaxRange": 6,
    "m_VehicleRefuelDirectly": true,
    "m_LogLevel": 4
}
```

---

## Installation

1. Download the mod from Steam Workshop or GitHub
2. Add `itz_fuelsystem` to your server's mod list
3. Configure the XML files in `<ServerProfile>/iTzMods/FuelSystem/`
4. Restart the server

---

## Requirements

- **DayZ Server**
- **Community Framework (CF)** - Required dependency

---

## Credits

**Author:** iTzChronuZ (Jhonnata Paixão)  
**Steam ID:** 76561199441861559  
**Version:** 1.0.0

---

## Support

For bug reports, feature requests, or questions:
- Steam Workshop comments
- GitHub Issues

---

## License

MIT License

Copyright (c) 2026 Jhonnata Paixão (iTzChronuZ)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
