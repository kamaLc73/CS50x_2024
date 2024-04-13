def count_letters(text):
    return sum(c.isalpha() for c in text)


def count_words(text):
    return len(text.split())


def count_sentences(text):
    return text.count('.') + text.count('!') + text.count('?')


def calculate_index(text):
    words = count_words(text)
    letters = count_letters(text)
    sentences = count_sentences(text)

    L = (letters / words) * 100
    S = (sentences / words) * 100

    return 0.0588 * L - 0.296 * S - 15.8


def grade_level(index):
    grade = round(index)
    if grade < 1:
        print("\nBefore Grade 1")
    elif grade >= 16:
        print("\nGrade 16+")
    else:
        print("\nGrade", grade)


def main():
    text = input("Text: \n")

    print("letters :", count_letters(text))
    print("words :", count_words(text))
    print("sentences :", count_sentences(text))

    index = calculate_index(text)
    grade_level(index)


if __name__ == "__main__":
    main()
