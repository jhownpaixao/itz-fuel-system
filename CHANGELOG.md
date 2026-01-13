# Changelog - iTz Fuel System

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/).

---

## [1.0.1] - 2026-01-13

### Fixed
- **NULL Pointer in LogFuelStation**: Fixed NULL pointer error in `LogFuelStation` function that caused server crash during initialization
  - The error occurred when trying to access `station.m_Variables` without checking if it was NULL
  - Generated multiple identical entries in crash.log (one per loaded station)
  - Stack trace: `fs_module.c:876 -> LoadStationProfiles:371 -> OnMissionStart:61`
  - Affected file: [FS_Module.c](Scripts/4_World/Modules/FS_Module.c:876)

---

## [1.0.0] - 2026-01-13

### Added
- Initial release of iTz Fuel System
- Complete fuel management system for DayZ
- **FS_PowerGenerator**: Custom power generator
  - Fuel tank capacity: 5000ml
  - Custom 3D model with textures
  - Integrated energy system
- **Fuel Station System**
  - Support for multiple stations on the map
  - Fuel profile system
  - Range configuration and energy requirements
  - Fuel respawn system
- **Refueling Actions**
  - Refuel vehicles (cars and boats)
  - Refuel generators
  - Fill containers with fuel
  - Measure available fuel
- **Sound System**
  - Fuel pump sounds (2 variations)
  - Pump lever sounds
  - Realistic ambient sounds
- **Vehicle Support**
  - CarScript integration
  - BoatScript integration
  - RF mod support (RFWC_base)
- **Custom Fuels**
  - KeroseneLiquid (Aviation Fuel)
  - Toxicity and flammability system
- **Compatible Containers**
  - Barrels, gasoline canisters
  - Pots, cauldrons, canteens
  - Water bottles and glass bottles
  - Water pouches
- **Profile System**
  - Fuel station profiles (JSON)
  - Vehicle profiles (JSON)
  - Automatic example profile generator
  - Legacy version migration system
- **Development Tools**
  - Logging system with levels (0=Error, 1=Info, 2=Trace)
  - XML file manipulation
  - Client-server synchronization system
- **Localization**
  - Stringtable for multi-language support

### Technical
- Dependencies: DZ_Data, DZ_Scripts, JM_CF_Scripts
- Modules: Core, Game, World, Mission
- Continuous action component system
- Configurable damage system
- EnergyManager for generator power management

### Credits
- **Author**: iTzChronuZ
- **Name**: Jhonnata Paixão
- **SteamID**: 76561199441861559
- **Version**: 1.0.0
