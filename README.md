# ModOS

ModOS is a modular operating system designed to be customizable, lightweight, and optimized, particularly suited for IoT environments, embedded systems, and video game development. It allows granular control over resources and supports loading/unloading of modules in real-time without requiring a reboot.

**Currently**, we have a 64-bit OS that displays pixels on the screen using a framebuffer. The next steps are to handle memory management and the filesystem.

## Getting Started

### Installation

Install dependencies

For Ubuntu:
```bash
sudo apt update && sudo apt install -y qemu-system-x86 grub-pc-bin mtools xorriso build-essential nasm bison flex libgmp3-dev libmpfr-dev libmpc-dev texinfo
```

For Fedora:
```
sudo dnf update && sudo dnf install -y qemu-system-x86 grub2-tools mtools xorriso make gcc nasm bison flex gmp mpfr mpc texinfo
```

Extract sources
```
mkdir -p ~/cross-compiler && cd ~/cross-compiler
```

Install [binutils](https://www.gnu.org/software/binutils/)

Compile binutils
```
mkdir binutils-build && cd binutils-build
../binutils-X.XX/configure --target=x86_64-elf --prefix=/opt/cross --with-sysroot --disable-nls --disable-werror
make -j$(nproc)
sudo make install
cd ..
```

Compile GCC
```
mkdir gcc-build && cd gcc-build
../gcc-XX.X.X/configure --target=x86_64-elf --prefix=/opt/cross --disable-nls --enable-languages=c,c++ --without-headers
make -j$(nproc) all-gcc
sudo make install-gcc
cd ..
```

```
echo 'export PATH=/opt/cross/bin:$PATH' >> ~/.bashrc
source ~/.bashrc
```

Try
```
x86_64-elf-gcc --version
```

### Quickstart

```
make build-x86_64 && make qemu
```

### Usage

For now, there are no interactions; the OS can only display pixels.

## Get involved

You're invited to join this project ! Check out the [contributing guide](./CONTRIBUTING.md).

If you're interested in how the project is organized at a higher level, please contact the current project manager.

## Our PoC team ❤️


Developers
| [<img src="https://github.com/louonezime.png?size=85" width=85><br><sub>Lou Onézime</sub>](https://github.com/louonezime) | [<img src="https://github.com/lg-epitech.png?size=85" width=85><br><sub>Laurent Gonzalez</sub>](https://github.com/lg-epitech) | [<img src="https://github.com/moonia.png?size=85" width=85><br><sub>Mounia Arjdal</sub>](https://github.com/moonia) | [<img src="https://github.com/SIMLUKE.png?size=85" width=85><br><sub>Luc Simon</sub>](https://github.com/SIMLUKE) | [<img src="https://github.com/sephorah.png?size=85" width=85><br><sub>Séphorah Aniambossou</sub>](https://github.com/sephorah)
| :---: | :---: | :---: | :---: | :---: |

Manager
| [<img src="https://github.com/pierrelissope.png?size=85" width=85><br><sub>Pierre Lissope</sub>](https://github.com/pierrelissope)
| :---:

<h2 align=center>
Organization
</h2>

<p align='center'>
    <a href="https://www.linkedin.com/company/pocinnovation/mycompany/">
        <img src="https://img.shields.io/badge/LinkedIn-0077B5?style=for-the-badge&logo=linkedin&logoColor=white" alt="LinkedIn logo">
    </a>
    <a href="https://www.instagram.com/pocinnovation/">
        <img src="https://img.shields.io/badge/Instagram-E4405F?style=for-the-badge&logo=instagram&logoColor=white" alt="Instagram logo"
>
    </a>
    <a href="https://twitter.com/PoCInnovation">
        <img src="https://img.shields.io/badge/Twitter-1DA1F2?style=for-the-badge&logo=twitter&logoColor=white" alt="Twitter logo">
    </a>
    <a href="https://discord.com/invite/Yqq2ADGDS7">
        <img src="https://img.shields.io/badge/Discord-7289DA?style=for-the-badge&logo=discord&logoColor=white" alt="Discord logo">
    </a>
</p>
<p align=center>
    <a href="https://www.poc-innovation.fr/">
        <img src="https://img.shields.io/badge/WebSite-1a2b6d?style=for-the-badge&logo=GitHub Sponsors&logoColor=white" alt="Website logo">
    </a>
</p>

> 🚀 Don't hesitate to follow us on our different networks, and put a star 🌟 on `PoC's` repositories

> Made with ❤️ by PoC
