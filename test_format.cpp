#include "format.h"
#include <gtest/gtest.h>

// 1) Простая подстановка одного и двух аргументов
TEST(FormatTests, SimpleTwoArgs) {
    EXPECT_EQ(format("{1}+{1} = {0}", 2, "one"), "one+one = 2");
}

// 2) Повторное использование одного аргумента несколько раз
TEST(FormatTests, RepeatArgument) {
    EXPECT_EQ(format("{0}{0}{0}{0}", "X"), "XXXX");
}

// 3) Несколько типов: int, double, строка
TEST(FormatTests, MixedTypes) {
    EXPECT_EQ(format("{2}:{1}:{0}", 10, 20.5, "time"), "time:20.5:10");
}

// 4) Строка без скобок возвращается как есть
TEST(FormatTests, NoBraces) {
    std::string s = "no placeholders here";
    EXPECT_EQ(format(s), s);
}

// 5) Индекс аргумента вне диапазона
TEST(FormatTests, IndexOutOfRange) {
    // у нас только один аргумент, но во фразе просят {1}
    ASSERT_THROW(format("{0} {1}", 42), FormatException);
    ASSERT_THROW(format("{5}", 1,2,3), FormatException);
}

// 6) Отсутствует закрывающая '}' для открывающей '{'
TEST(FormatTests, MissingClosingBrace) {
    ASSERT_THROW(format("Value {0", 1), FormatException);
    ASSERT_THROW(format("{0 some text", 1), FormatException);
}

// 7) Лишняя закрывающая '}' без соответствующей '{'
TEST(FormatTests, UnmatchedClosingBrace) {
    ASSERT_THROW(format("oops}", 1), FormatException);
    ASSERT_THROW(format("}oops{0}", 1), FormatException);
}

// 8) Пустые скобки `{}` запрещены
TEST(FormatTests, EmptyBraces) {
    ASSERT_THROW(format("Empty {} here", 1), FormatException);
    ASSERT_THROW(format("{}", 1), FormatException);
}

// 9) Нецифровой символ внутри скобок
TEST(FormatTests, NonDigitInBraces) {
    ASSERT_THROW(format("{a}", 1), FormatException);
    ASSERT_THROW(format("{-1}", 1), FormatException);
    ASSERT_THROW(format("{1.5}", 1.5), FormatException);
}

// 10) Слишком большое число в скобках (число вне диапазона size_t)
TEST(FormatTests, LargeNumberInBraces) {
    // Индекс 999 явно больше числа аргументов
    ASSERT_THROW(format("{999}", "x", "y"), FormatException);
}

// 11) Проверка граничных случаев: одиночные символы
TEST(FormatTests, SingleBraces) {
    ASSERT_THROW(format("{", 1), FormatException);
    ASSERT_THROW(format("}", 1), FormatException);
}

// 12) Проверяем, что наша функция работает на строках со множеством подряд идущих плейсхолдеров
TEST(FormatTests, ManyPlaceholders) {
    std::string tmpl;
    for (int i = 0; i < 100; ++i) {
        tmpl += "{" + std::to_string(i % 3) + "}";
    }
    // даём 3 аргумента
    auto result = format(tmpl, "A", "B", "C");
    // результат — повторяющаяся последовательность "ABCABC..."
    std::string expected;
    for (int i = 0; i < 100; ++i) {
        expected += (i % 3 == 0 ? "A" : i % 3 == 1 ? "B" : "C");
    }
    EXPECT_EQ(result, expected);
}


/*
int main(int argc, char** argv) {
     ::testing::InitGoogleTest(&argc, argv);
     return RUN_ALL_TESTS();
}
*/
