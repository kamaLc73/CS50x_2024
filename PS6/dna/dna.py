import csv
import sys


def main():
    # Check for command-line usage
    if len(sys.argv) != 3:
        print("Usage: python dna.py database.csv sequence.txt")
        sys.exit(1)

    # Read database file into a variable
    db_file = sys.argv[1]
    with open(db_file, 'r', encoding="utf-8") as f1:
        reader = csv.DictReader(f1)
        rows = {row['name']: row for row in reader}

    # Read DNA sequence file into a variable
    seq_file = sys.argv[2]
    with open(seq_file, 'r', encoding="utf-8") as f2:
        sequence = f2.read().strip()

    # Find longest match of each s in DNA sequence
    result_longmatch = []
    for i in range(1, len(reader.fieldnames)):
        result_longmatch.append(longest_match(sequence, reader.fieldnames[i]))

    # Check database for matching profiles
    for name, profile in rows.items():
        matches = 0
        for j in range(1, len(reader.fieldnames)):
            if int(profile[reader.fieldnames[j]]) == result_longmatch[j - 1]:
                matches += 1
        if matches == len(reader.fieldnames) - 1:
            print(name)
            return

    # If no match found
    print("No match")


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
