
# Untitled Adventure


## Description

Untitled Adventure is a source-available GBA game that takes inspiration from classic adventure games. Embark on a thrilling journey through a vast and immersive world filled with puzzles, enemies, and secrets waiting to be discovered.


## (Planned) Features

- Challenging puzzles and dungeons to explore

- Exciting combat system with a variety of weapons and abilities

- Beautifully designed pixel art graphics

- Memorable soundtrack that enhances the gameplay experience


## Getting Started

To get started with Untitled Adventure, follow these steps:

1. Follow the [Getting Started Guide](https://gvaliente.github.io/butano/getting_started.html) for Butano, Devkitpro, and DevkitARM (`gba-dev`)

1. Download [Tiled](https://thorbjorn.itch.io/tiled/purchase), our choosen map editor. Paying is optional, but it's nice if you do as it is very useful for these types of projects.

1. Clone the repository: `git clone https://github.com/breadbored/untitled-adventure-gba.git` or via Github Desktop

1. Download Python 3.12 and the Pip package manager

1. Navigate to the project directory: `cd untitled-adventure-gba`

1. Install Python and set up your environment

    - Linux / MacOS

        1. Run `python3 -m venv .py_venv`, or use `python` instead of `python3` if 3.12 is your default

        1. Run `source .py_venv/bin/activate`

        1. Install the required packages `pip install pillow scikit-learn`

    - Windows

        1. If you have a preferred virtual environment, use that to set up a new one

        1. Install the required packages `pip install pillow scikit-learn`

1. Compile the map assets: `make assets`

1. Build the GBA ROM: `make`

1. Run the game on a Gameboy Advance emulator or flash it to a physical cartridge.

## License

[![CC BY-NC-SA 4.0][cc-by-nc-sa-shield]][cc-by-nc-sa]

This work is licensed under a
[Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License][cc-by-nc-sa].

[![CC BY-NC-SA 4.0][cc-by-nc-sa-image]][cc-by-nc-sa]

[cc-by-nc-sa]: http://creativecommons.org/licenses/by-nc-sa/4.0/
[cc-by-nc-sa-image]: https://licensebuttons.net/l/by-nc-sa/4.0/88x31.png
[cc-by-nc-sa-shield]: https://img.shields.io/badge/License-CC%20BY--NC--SA%204.0-lightgrey.svg

## License Notes

We are currently **not** in a state where we should figure out our distribution and commercial terms. We will most likely re-license the code in the future regardless of how it turns out.

Our current license is primarily to stop paid distributions on cheap 5000-in-1 cartridges and marketplace sellers who sell incomplete ROMs, as well as require credit to the original authors.

## Credits

Contributors to this project include:


- [Jason C](https://github.com/breadbored)
	- Development
	- Game Co-Director

- [Nick B](https://github.com/NickBaxter)
	- Development
	- Game Co-Director

- Jenna C
	- Art
	- Character Design


## Contact

If you have any questions or suggestions, feel free to reach out to us at [gba@bread.codes](mailto:gba@bread.codes).
