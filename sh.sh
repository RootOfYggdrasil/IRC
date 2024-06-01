#!/bin/bash

# Function to check if the Escape key is pressed
check_escape_key() {
    while true; do
        # Capture the key press event
        read -rsn1 key
        if [[ "$key" == $'\e' ]]; then
            # If Escape key is pressed, exit the script
            exit 0
        fi
    done
}

# Start the mouse-click loop
while true; do
    xdotool click 1 &
    sleep 0.5
done 

# Call the function to check for Escape key press
