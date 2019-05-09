Feature: Compare files
  As an embedded software developer
  I want to compare binary files
  So that I can see if the result is the same

  Scenario: Empty files
    Given file1 is empty
      And file2 is empty
     When I compare file1 and file2
     Then result is that files are identical
