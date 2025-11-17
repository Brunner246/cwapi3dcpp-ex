# Contributing to cwapi3dcpp.ex

Thank you for considering contributing to cwapi3dcpp.ex! This document provides guidelines for contributing to the project.

## Code of Conduct

- Be respectful and inclusive
- Provide constructive feedback
- Focus on what is best for the community

## How to Contribute

### Reporting Bugs

#TODO

### Suggesting Features

#TODO

### Pull Requests

1. Fork the repository
2. Create a feature branch: `git checkout -b feature/your-feature-name`
3. Make your changes following the code style guidelines
4. Add tests for new functionality
5. Ensure all tests pass
6. Commit with clear, descriptive messages
7. Push to your fork
8. Create a pull request

## Code Style Guidelines

### C++ Style

- Follow Modern C++ (C++20) best practices
- Use RAII and smart pointers
- Prefer `const` and `noexcept` where applicable
- Use meaningful variable and function names
- Keep functions focused and small

### Formatting

- Use the provided `.clang-format` configuration
- Run `clang-format` before committing

### Documentation

- Document public APIs with comments
- Use Doxygen-style documentation
- Include usage examples for complex features
- Update README.md if adding new features

### Testing

- Write unit tests for new functionality
- Ensure all existing tests pass
- Aim for high code coverage
- Test edge cases and error conditions

## Building and Testing

```bash
# Configure
cmake -B build -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=ON

# Build
cmake --build build

# Test
cd build
ctest --output-on-failure
```

## Commit Messages

- Use present tense ("Add feature" not "Added feature")
- Keep first line under 72 characters
- Reference issues and pull requests
- Example:
  ```
  Add KDTree k-nearest neighbor search

  Implements efficient k-NN search using priority queue.
  Fixes #123
  ```

## Design Principles

Follow these principles when contributing:

- **SOLID Principles**
- **Clean Code** - Self-documenting, readable code
- **DRY** - Don't Repeat Yourself
- **YAGNI** - You Aren't Gonna Need It
- **Separation of Concerns**

## License

By contributing, you agree that your contributions will be licensed under the MIT License.
