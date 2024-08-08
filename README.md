# MiniFetch
A W.I.P ripoff of fastfetch.

Unfortunately theres currently a few errors regarding installation.

## Features

- Displays the hostname
- Detects the operating system
- Shows Linux distribution information
- Displays CPU and GPU information
- Shows total RAM
- Lists network interface addresses

## Installation

### Prerequisites

- GCC (GNU Compiler Collection)
- Make
- Standard C libraries
- dos2unix

### Steps

1. **Clone the repository:**

    ```bash
    git clone https://github.com/AllSeeingBird-Tux/MiniFetch
    cd MiniFetch
    ```

2. **Compile and Install:**

    Run the `install.sh` script to compile and install the `minifetch` command:

    ```bash
    dos2unix install.sh
    chmod +x install.sh
    ./install.sh
    ```

    This script will compile the code using `make` and move the binary to `/usr/local/bin`.

## Usage

After installation, you can run the `minifetch` command in your terminal:

```bash
minifetch
```

## Current Contributors
AllSeeingBird-Tux

astroLazybones
