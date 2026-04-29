*** Settings ***
Documentation    A dummy test to verify the Robot Framework environment.

*** Test Cases ***
Verify Environment
    [Documentation]    Simple test to check if Robot Framework is running.
    Should Be Equal    1    1
