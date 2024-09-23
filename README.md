<h1 align="center">[🐱] CCAT :: 'C' Cat [🐱]</h1>

> CCAT is a simple C program that downloads a specified number of cat images from the internet.  
> It's free to use or modify, intended to be a simple example of using `libcurl` and `libjson-c` in a C program. Fork it, optimize it, and share the love of cats!

<h2 align="center">🐈‍⬛ Install</h2>

> To compile CCAT from source, you'll need the following packages preinstalled:

### 🧶 Debian
-  libcurl4-openssl-dev
-  libjson-c-dev
```bash
sudo apt update && sudo apt-get install libcurl4-openssl-dev libjson-c-dev
```
### 🧶 OpenBSD
-  curl
-  json-c
```bash
doas pkg_add curl json-c
```
### 🧶 Gentoo
-  curl
-  json-c
```bash
doas emerge net-misc/curl dev-libs/json-c
```
### 🧶 Fedora
-  libcurl-devel
-  json-c-devel
```bash
sudo dnf install libcurl-devel json-c-devel
```
### 🧶 MacOS
-  curl
-  json-c
```bash
brew install curl json-c
```

<h2 align="center">🐾 CCAT install:</h2>

```bash
git clone https://github.com/plastic-bottleneck/ccat && cd ccat
```
```bash
make
```
<h2 align="center">🙀 Usage</h2>

> Make sure you have created a directory for storing your downloaded cat images. To run CCAT, specify the number of images to download and the output folder. Example:

```bash
./ccat 5 ./cat_images
```

<p align="center">‧˚₊•┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈୨୧┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈•‧₊˚⊹ </p>
<p align="center">୨⎯ Made by Paul ❤️ Plastic Bottleneck <a href="https://github.com/plastic-bottleneck">GitHub</a> ⎯୧</p>
<p align="center">୨⎯ Check out my custom <a href="https://github.com/plastic-bottleneck/bashrc">Bash config</a> ⎯୧</p>
