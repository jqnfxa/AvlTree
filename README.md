# AVL Tree Implementation

This project provides a working implementation of an AVL tree, a self-balancing binary search tree. The AVL tree maintains a balanced structure, which allows for efficient insertion, deletion, and search operations.

## Requirements

This project is built with C++ std 20 and uses CMake for building and managing the project. Ensure you have the following installed on your system:

- C++ Compiler (GCC, Clang, MSVC, etc.)
- CMake

## Usage
Usage of avl_tree is similar to usage of std::set.

To use this AVL tree implementation in your project, follow these steps:

1. Clone this repository to your local machine.
2. Include the necessary header files in your project.
3. Use the AVL tree data structure as needed in your code.

Here's a basic example of how to use the AVL tree:

```cpp
#include "avl_tree.hpp"

int main()
{
    avl_tree<int> tree;
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    // Perform other operations as needed...
    return 0;
}
```

## Author
This AVL tree implementation was created by jqnfxa.

## License
This project is in the public domain. There is no need to ask for permission to use or modify this project.
