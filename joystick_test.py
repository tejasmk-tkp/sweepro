import pygame
import time

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
        # Process pygame events
        pygame.event.pump()

        # Print axes data
        for i in range(joystick.get_numaxes()):
            print(f"Axis {i}: {joystick.get_axis(i):.2f}")

        # Print button data
        for i in range(joystick.get_numbuttons()):
            print(f"Button {i}: {joystick.get_button(i)}")

        # Print hat data (e.g., D-pad)
        for i in range(joystick.get_numhats()):
            print(f"Hat {i}: {joystick.get_hat(i)}")

        print("-" * 50)  # Separator for readability

        time.sleep(0.2)

except KeyboardInterrupt:
    print("\nExiting...")
finally:
    # Cleanup
    joystick.quit()
    pygame.quit()
