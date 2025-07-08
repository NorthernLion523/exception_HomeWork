# Makefile для nmake (VS Developer Command Prompt)

# Путь к поддиректории googletest, куда вы клонировали репо:
# там есть subfolders include\gtest и src\*.cc
GTEST_BASE = C:\tools\googletest\googletest

# Заголовки gtest
GTEST_INC  = $(GTEST_BASE)\include

# Исходники gtest
GTEST_SRC  = $(GTEST_BASE)\src

# Компилятор и флаги
CXX        = cl
CXXFLAGS   = /EHsc /std:c++20 /I"." /I"$(GTEST_INC)" /I"$(GTEST_BASE)"

# Цель по умолчанию
all: format_tests.exe

# Собираем ваш тест + gtest-all и gtest_main из исходников
format_tests.exe: test_format.cpp format.h
	$(CXX) $(CXXFLAGS) test_format.cpp "$(GTEST_SRC)\gtest-all.cc" "$(GTEST_SRC)\gtest_main.cc" /Fe:format_tests.exe

# Очистка
clean:
	del format_tests.exe
	del *.obj
