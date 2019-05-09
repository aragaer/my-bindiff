Feature: Compare files
  As an embedded software developer
  I want to compare binary files
  So that I can see if the result is the same

  Scenario: Empty files
    Given file1 is empty
      And file2 is empty
     When I compare file1 and file2
     Then result is the following:
       """
       Files file1 and file2 are equal.
       """

  Scenario: Empty vs non-empty
    Given file1 is empty
      And file2 contains the following:
        """
        x
        """
     When I compare file1 and file2
     Then result is the following:
       """
       Files file1 and file2 have different size (0 and 1).
       """

  Scenario: Different data
    Given file1 contains the following:
        """
        y
        """
      And file2 contains the following:
        """
        x
        """
     When I compare file1 and file2
     Then result starts with the following:
       """
       Files file1 and file2 are different.
       """
