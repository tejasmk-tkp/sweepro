import pygame
import serial
import serial.tools.list_ports
import time
import signal
import sys


def find_esp_port():
    """Automatically detect the ESP's COM port."""
    ports = list(serial.tools.list_ports.comports())
    for port in ports:
        # Look specifically for USB/ACM ports
        if "USB" in port.device or "ACM" in port.device:
            print(f"Found ESP on port: {port.device}")
            return port.device
    print("No ESP device detected! Check your connection.")
    exit()

def graceful_exit(signum, frame):
    """Handle graceful exit on signal."""
    print("\nExiting gracefully...")
    esp_serial.close()  # Close serial connection
    pygame.quit()       # Quit pygame
    sys.exit(0)

# Register the signal handlers
signal.signal(signal.SIGINT, graceful_exit)
signal.signal(signal.SIGTERM, graceful_exit)

# Find the ESP port
esp_port = find_esp_port()

# Initialize serial communication
esp_serial = serial.Serial(esp_port, 115200, timeout=1)
time.sleep(2)  # Wait for ESP to initialize

# Initialize pygame
pygame.init()

# Initialize the joystick module
pygame.joystick.init()

# Check for connected joysticks
if pygame.joystick.get_count() == 0:
    print("No joysticks connected! Plug one in and try again.")
    exit()

# Select the first joystick
joystick = pygame.joystick.Joystick(0)
joystick.init()

print(f"Joystick initialized: {joystick.get_name()}")

try:
    while True:
        # Handle events
        pygame.event.pump()

        # Get axis values
        axis_1 = joystick.get_axis(2)  # Axis 1

        # Get button values
        button_0 = joystick.get_button(4)  # Button 0
        button_1 = joystick.get_button(5)  # Button 1
        right = joystick.get_button(1)
        left = joystick.get_button(2)
        neutral = joystick.get_button(3)

        # Prepare data string
        data = f"{axis_1:.2f},{button_0},{button_1},{right},{left},{neutral}\n"

        # Send data to ESP
        esp_serial.write(data.encode())

        # Print sent data
        print(f"Sent: {data.strip()}")

        # Small delay to avoid spamming
        time.sleep(0.1)

except KeyboardInterrupt:
    # Handle Ctrl+C gracefully
    graceful_exit(None, None)
