<code_standards>

<python_coding_standards>

<language_format>
  - All code and comments must be written in English.
  - Source files must use UTF-8 encoding.
  - Code must be cross-platform (Windows, Linux, macOS).
</language_format>

<style_guidelines>
  - Follow the PEP 8 style guide.
  - Use snake_case for:
    - Function names
    - Variable names
    - File names
  - Use CamelCase (PascalCase) for:
    - Class names
  - Use 4 spaces per indentation level.
  - Do not use emojis or special Unicode characters (e.g. check marks, arrows, symbols).
  - Use only standard ASCII characters in both code and comments.
</style_guidelines>

<documentation>
  - Use PEP 257-compliant docstrings for:
    - Public functions
    - Classes
    - Modules
  - Each docstring must include:
    - A brief summary of the purpose
    - "Args:" section listing parameter names and types
    - "Returns:" section describing return type and value
    - "Raises:" section describing exceptions raised (if any)
</documentation>

<code_practices>
  - Prefer explicit logic over implicit.
  - Validate all inputs and handle errors appropriately.
  - Maintain modular code with separation of:
    - Logic
    - Data handling
    - I/O
  - Write testable code.
  - Avoid OS-specific or non-portable code patterns.
</code_practices>
</python_coding_standards>

<c_coding_standards>

<language_format>
  - All code and comments must be written in English.
  - Use only standard ASCII characters.
  - Code must compile with common compilers (GCC, Clang, MSVC).
</language_format>

<style_guidelines>
  - Use camelCase for:
    - Function names
    - Variable names
  - Use UPPER_CASE for:
    - Constants
    - Macro definitions
  - Indentation must be 4 spaces (no tabs).
  - Use a consistent brace style (e.g. K&R or Allman, based on project rules).
</style_guidelines>

<documentation>
  - Use Doxygen-style comments for all public functions.
  - Each function comment block must include:
    - @brief: Short description of the function
    - @param: Description of each parameter
    - @return: Description of the return value
    - @note or @warning: Notes about side effects or important usage considerations
</documentation>

<code_practices>
  - Use header guards (#ifndef/#define/#endif) in all header files.
  - Avoid global variables when possible.
  - Use static for private (file-local) variables and functions.
  - Keep functions short and focused.
  - Initialize all variables before use.
</code_practices>
</c_coding_standards>

</code_standards>