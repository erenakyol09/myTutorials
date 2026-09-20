Apply the project's Python Language Standards to the Python files in the current task. Review style, documentation, and code practices, and fix any violations.

## Python Language Standards

### Style Guidelines
- Follow the PEP 8 style guide
- Use snake_case for:
  - Function names
  - Variable names
  - File names
- Use CamelCase (PascalCase) for:
  - Class names
- Use 4 spaces per indentation level

### Documentation Requirements
- Use PEP 257-compliant docstrings for:
  - Public functions
  - Classes
  - Modules
- Each docstring must include:
  - A brief summary of the purpose
  - "Args:" section listing parameter names and types
  - "Returns:" section describing return type and value
  - "Raises:" section describing exceptions raised (if any)

### Code Practices
- Prefer explicit logic over implicit
- Validate all inputs and handle errors appropriately
- Maintain modular code with separation of:
  - Logic
  - Data handling
  - I/O
- Write testable code
- Avoid OS-specific or non-portable code patterns
