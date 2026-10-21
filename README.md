# swkotor1-reconstruction

An unofficial reverse-engineered executable reconstruction project for **Star Wars: Knights of the Old Republic**, targeting the **GOG Windows release, game version 1.03**.

This public repository currently contains documentation and a rights notice only. It does not provide a playable release or implementation source code.

## Target and verification

The following facts were checked on 2026-10-21 by reading the local original installation's executable and metadata, without executing the game or installer:

| Item | Verified value and evidence |
| --- | --- |
| Distribution | GOG Windows, English; installed GOG metadata identifies game ID `1207666283` |
| Game patch version | `1.03`, reported by GOG's installed registry metadata and corroborated by the installed `SWKotORv103.txt` patch notes |
| Executable | `swkotor.exe`, Windows x86, 4,042,752 bytes |
| Executable file and product version | Both version resources report `1, 0, 3, 0` (`1.0.3.0`) |
| Executable SHA-256 | `9c10e0450a6eeca417e036e3cde7474fed1f0a92aab018446d156944dea91435` |
| Installed GOG build ID | `52316137315118251`, reported by `goggame-1207666283.info` and GOG's installed registry metadata |
| GOG offline installer version/build | Unknown; an original offline installer package was not verified |

The game patch version, executable version resources, and GOG build ID are distinct identifiers. The installed build ID is not a verified offline installer version. The hash identifies the specific original executable examined; it does not establish compatibility with every GOG download, other releases, or modified installations. Compatibility with this target remains subject to testing.

## Original game required

You must separately purchase and lawfully possess the original game and supply its assets locally. This repository distributes no original game resources, proprietary binaries, middleware, raw decompiler exports, or implementation code. No original game files are uploaded here.

## Project status

The maintainer reports that most gameplay has been reconstructed, with minigames remaining, and describes other remaining functions as including unused Neverwinter multiplayer remnants, debugging, and cheats. These are reported progress and classification statements, not an independently audited inventory or proof of complete gameplay. No complete-playthrough or full-compatibility claim is made.

Planned documentation includes reconstruction provenance, compatibility evidence and limitations, and intentional omissions with their rationale. Those documents and their supporting audits are not yet published here.

## Rights and affiliation

This project is unofficial and is not affiliated with or endorsed by the game's creators, publishers, GOG, or other rights holders.

The original game, its trademarks, assets, and other third-party materials remain the property of their respective rights holders. All rights reserved by the respective rights holders. This project does not claim ownership of those rights or grant permission to redistribute proprietary game materials.

See [LICENSE](LICENSE) for the rights and reservation notice. No implementation source-code license has been chosen; any future source publication and license require a separate determination of the relevant rights and permissions.
