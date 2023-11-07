#pragma once

#include <type_traits>

template <typename T, typename Arg1, typename Arg2>
concept BinaryPredicate = requires(T t, Arg1 a1, Arg2 a2)
{
    {
        t(a1, a2)
        } -> std::convertible_to<bool>;
};
