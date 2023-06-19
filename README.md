<h1 align="center">vpr-bin2fmt</h1>
<p align="center">
  <img src="https://img.shields.io/badge/Windows-supported-44CC11"/>
  <img src="https://img.shields.io/badge/Mac-supported-44CC11"/>
  <img src="https://img.shields.io/badge/Linux-supported-44CC11"/>
  <a href="https://mit-license.org/"/>
    <img src="https://img.shields.io/badge/License-MIT-44CC11"/>
  </a>
  <img src="https://img.shields.io/github/actions/workflow/status/0xvpr/scct-trainer/docker_build.yml?label=Build"/>
</p>
<br>
This project aims to provide a tool for the conversion of a file to  
formatted double quoted string or a c-style array.

## Build instructions
```bash
git clone https://github.com/0xvpr/vpr-bin2fmt vpr-bin2fmt
cd vpr-bin2fmt
make
# sudo make install # uncomment if you want it available globally (only recommended for LINUX/WSL/MSYS environments)
```

## Example Usage
```bash
Usage:
  vpr-bin2fmt <path/to/executable> [ optional switches ]

positional arguments:
  path/to/executable

optional switches:
  -c,-C         output as c-style char array
  -s,-S         output as double quoted string
example :
  vpr-bin2fmt path/to/file-1 path/to/file2 -s
```
