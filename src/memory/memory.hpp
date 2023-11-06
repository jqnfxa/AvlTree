#include <type_traits>

/**
 * @brief Function to safely delete a pointer.
 * @tparam ptr_t pointer type.
 * @param pointer pointer to delete.
 */
template <typename ptr_t>
requires std::is_pointer_v<ptr_t>

void safe_delete(ptr_t &pointer) noexcept
{
    delete pointer;
    pointer = nullptr;
}
