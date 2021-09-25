add_library(spacepi-libdtc
    src/diagnostics/Diagnostic.cpp
    src/diagnostics/DiagnosticReporter.cpp
    src/diagnostics/DiagnosticReporterImpl.cpp
    src/diagnostics/SourceFile.cpp
    src/diagnostics/SourceLocation.cpp
    src/emitter/Emitter.cpp
    src/emitter/EmitterImpl.cpp
    src/includer/Includer.cpp
    src/includer/IncluderImpl.cpp
    src/lexer/Lexeme.cpp
    src/lexer/Lexer.cpp
    src/lexer/LexerImpl.cpp
    src/main/Main.cpp
    src/main/MainImpl.cpp
    src/parser/Parser.cpp
    src/parser/ParserImpl.cpp
    src/parser/SyntaxCell.cpp
    src/parser/SyntaxProperty.cpp
    src/parser/SyntaxTree.cpp
    src/tokenizer/Token.cpp
    src/tokenizer/Tokenizer.cpp
    src/tokenizer/TokenizerImpl.cpp
    src/validator/Validator.cpp
    src/validator/ValidatorImpl.cpp
)

target_include_directories(spacepi-libdtc PUBLIC
    include
)

add_executable(spacepi-dtc
    src/main/Exe.cpp
)

target_link_libraries(spacepi-dtc
    spacepi-libdtc
)
