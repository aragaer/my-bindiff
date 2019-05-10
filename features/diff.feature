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

  Scenario: Different printable data
    Given file1 contains the following:
        """
        y
        """
      And file2 contains the following:
        """
        x
        """
     When I compare file1 and file2
     Then result is the following:
       """
       Files file1 and file2 are different.
       Difference starting at offset 0 (size 1):
       >>> str y
       <<< str x
       """

  Scenario: Different binary data
    Given file1 contains the following binary data:
        """
        01
        """
      And file2 contains the following binary data:
        """
        02
        """
     When I compare file1 and file2
     Then result is the following:
       """
       Files file1 and file2 are different.
       Difference starting at offset 0 (size 1):
       >>> hex 01
       <<< hex 02
       """

  Scenario: Different mixed data
    Given file1 contains the following binary data:
        """
        01
        """
      And file2 contains the following:
        """
        0
        """
     When I compare file1 and file2
     Then result is the following:
       """
       Files file1 and file2 are different.
       Difference starting at offset 0 (size 1):
       >>> hex 01
       <<< hex 30
       """
     When I compare file2 and file1
     Then result is the following:
       """
       Files file2 and file1 are different.
       Difference starting at offset 0 (size 1):
       >>> hex 30
       <<< hex 01
       """
