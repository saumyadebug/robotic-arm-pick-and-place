# Contributing Guidelines

Thank you for your interest in contributing to the **Robotic Arm Pick and Place** project!

## How to Contribute

1. **Reporting Bugs**:
   - Check existing issues before opening a new one.
   - Provide hardware specs, wiring configuration, Arduino IDE version, and exact error logs.

2. **Proposing Features**:
   - Describe the motivation, kinematic equations or algorithms, and required hardware changes.

3. **Submitting Pull Requests**:
   - Fork the repository and create a feature branch (`git checkout -b feature/smooth-motion`).
   - Keep Arduino sketches well-commented with pin definitions, units (deg/ms/ticks), and baud rates.
   - Test sketches on hardware before submitting PRs.
   - Update documentation under `docs/` and test logs under `tests/` if your change affects calibration or pinouts.

## Code Conventions
- Follow Arduino/C++ style guidelines.
- Use explicit constants for pin numbers and servo channels (e.g., `#define SERVO_BASE_CHANNEL 0`).
- Include safe bounds checking for servo angles to prevent mechanical stalls.
