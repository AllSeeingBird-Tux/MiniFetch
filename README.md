# MiniFetch
A W.I.P ripoff of fastfetch.
![MiniFetch](https://github.com/user-attachments/assets/8e42a4d0-773a-403c-9990-cf6baabccb06)

## Features

- Displays the hostname
- Detects the operating system
- Shows Linux distribution information
- Displays CPU information
- Displays GPU information if applicable, only works with some AMD cards currently, W.I.P
- Shows total RAM
- Lists network interface addresses

## Installation

### Prerequisites

- GCC (GNU Compiler Collection)
- Make
- Standard C libraries

### Steps

1. **Clone the repository:**

    ```bash
    git clone https://github.com/AllSeeingBird-Tux/MiniFetch
    cd MiniFetch
    ```

2. **Compile and Install:**

    Run the `install.sh` script to compile and install the `minifetch` command:

    ```bash
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
