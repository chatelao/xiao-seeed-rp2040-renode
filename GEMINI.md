# Goal
Create a setup for the XIAO Seed RP2040 able to run the UART, the ADC and the PWM on Renode over PlatformIO.

# Structure
- `CONCEPT.md`: The overall structure of the product, including Business & Use Cases as well as the High-Level Architecture.
- `DESIGN.md`: The detailed design of the solution, including the architecture, used tech stack for development, production and testing, etc.
- `ROADMAP.md`: The list of accomplished and planned steps of the project.
- `TECHNICAL_DEBTS.md`: If you find technical debts, like outdate components, security flaws, old patterns, etc. log them here, but don’t fix them until asked to do so.
- `/specification/`: External Know-How as datasheet, standards, etc. Should be converted to Markdown if PDF, etc.
- `/src/`: The source code of the project
- `/test/`: All tools, configurations & test cases
- `/build/`: Only temporary place for compilation, may be cached by Github

# `CONCEPT.md` handling
- The `CONCEPT.md` add the business and use cases to the top Goal this file.
- It contains an architecture with top-level functional components and their business interfaces.
- It does not contain all precise implementation choices.
- Every major choice is first drawn out as three alternatives, the best one is chosen and the ohter, discarded ones kept in summary in the last chapter of the concept.

# `DESIGN.md`: 
- The `DESIGN.md` derives all necessary technological choices from `CONCEPT.md`.
- It contains a detailed architecture of all components and their technical interfaces.
- It does contain precise implementation choices.
- Every major choice is first drawn out as three alternatives, the best one is chosen and the ohter, discarded ones kept in summary in the last chapter of the concept.

# `ROADMAP.md` handling
- The `ROADMAP.md` is the final plan to implement the `CONCEPT.md` and `DESIGN.md`to achive the top goal.
- The `ROADMAP.md` file structured into Phases as chapters.
- The Tasks, and Subtasks if necessary, have checkboxes to show the progress.
- Every task to be implemented has to be modest, feasible and reasonable.
  - If no such task is available, then break down a bigger steps to modest ones without implementing anything, just changing the `ROADMAP.md`.
- The progress is updated with  with every increment.
- The finished tasks are linked to the corresponding issue and timestamped at the end of the line.

# HOWTO
- Keep `src/install.sh` to install all tools to build the application (test only tools, see below)
- Extract all necessary part from `https://github.com/chatelao/Renode_RP2040`

# Testing Locally & with Github Action Workflow
- Setup the empty CI/CD pipeline before coding anything
- Write CI/CD test independent as `test` script of the Github action workflows
- Create screenshots of each UI step tested and store it as asset of the Action Workflow for review
- Use `test/install.sh` to install test tools.
- Use the Github action workflows to run the tests after commits.
- Before committing fetch all changes from the remote repository and merge the changes
- Run the CI/CD on every commit on every branch
- Add as much caching as possible to the Github action workflows
