# Design

The detailed design of the solution, including the architecture, used tech stack for development, production and testing, etc.

## Technological Choices

### Choice 3: Programming SDK
- **Option A: Raspberry Pi Pico SDK (Selected)** - Native SDK for RP2040, providing low-level peripheral access and high performance. Used extensively in the reference `Renode_RP2040` repository.

### Choice 4: Testing Framework
- **Option A: Robot Framework (Selected)** - Integration and system testing framework used for Renode simulations. Provides clear test reports and is used in the reference `Renode_RP2040` repository.

### Choice 5: CI/CD Platform
- **Option A: GitHub Actions (Selected)** - Integrated CI/CD platform for GitHub repositories, allowing for automated builds and simulation runs. Explicitly mentioned in the `ROADMAP.md`.

## Detailed Architecture
- (To be defined)

## Technical Interfaces
- (To be defined)

## Implementation Choices
- (To be defined)

## Discarded Alternatives
### Choice 3: Programming SDK
- **Option B: Arduino Framework** - Higher level abstraction, but may hide details needed for precise peripheral simulation.
- **Option C: Bare Metal (C/C++ without SDK)** - Maximum control but requires significant effort to implement basic functionality.

### Choice 4: Testing Framework
- **Option B: Unity** - Lightweight C unit testing framework, but less suitable for high-level simulation orchestration compared to Robot Framework.
- **Option C: Pytest** - Python testing framework, powerful but lacks native integration with Renode's built-in testing capabilities compared to Robot Framework.

### Choice 5: CI/CD Platform
- **Option B: GitLab CI/CD** - Requires external hosting or mirroring for this repository.
- **Option C: Local Bash Scripts** - Not suitable for automated, cloud-based continuous integration and reporting.
