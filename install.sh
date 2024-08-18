#!/bin/bash


if [ $? -eq 0 ]; then
	gcc main.c other.c gpufetch.c -o minifetch
    sudo mv minifetch /usr/local/bin/

    if [ $? -eq 0 ]; then
        echo "minifetch installed successfully!"
    else
        echo "Failed to move minifetch to /usr/local/bin/"
    fi
else
    echo "Compilation failed!"
fi
