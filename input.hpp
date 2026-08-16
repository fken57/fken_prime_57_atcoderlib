#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

namespace fib {
namespace input_support {
namespace detail {

template <class T>
using remove_cvref_t =
    typename std::remove_cv<typename std::remove_reference<T>::type>::type;

template <class>
struct dependent_false : std::false_type {};

template <class T, bool = std::is_enum<T>::value>
struct dimension_value_type {
    using type = T;
};

template <class T>
struct dimension_value_type<T, true> {
    using type = typename std::underlying_type<T>::type;
};

template <class T>
struct DimensionReference {
    const T* value;

    const T& get() const noexcept { return *value; }
};

template <class T>
struct DimensionValue {
    T value;

    const T& get() const noexcept { return value; }
};

template <class T>
auto make_dimension(T&& value) {
    if constexpr (std::is_lvalue_reference<T&&>::value) {
        using U = typename std::remove_reference<T>::type;
        return DimensionReference<U>{std::addressof(value)};
    } else {
        using U = typename std::decay<T>::type;
        return DimensionValue<U>{std::forward<T>(value)};
    }
}

template <class Container, class Dimension>
struct SizedInput {
    Container& container;
    Dimension dimension;
};

template <class Container, class... Dimensions>
struct ShapedInput {
    Container& container;
    std::tuple<Dimensions...> dimensions;
};

template <class Container, class Height, class Width>
struct ShapedLinesInput {
    Container& container;
    Height height;
    Width width;
};

template <class T>
struct is_basic_string : std::false_type {};

template <class Traits, class Allocator>
struct is_basic_string<std::basic_string<char, Traits, Allocator>>
    : std::true_type {};

template <class T>
struct is_char_array
    : std::integral_constant<
          bool,
          std::is_array<typename std::remove_reference<T>::type>::value &&
              std::is_same<
                  typename std::remove_cv<typename std::remove_extent<
                      typename std::remove_reference<T>::type>::type>::type,
                  char>::value> {};

template <class T>
struct is_char_pointer
    : std::integral_constant<
          bool,
          std::is_pointer<typename std::decay<T>::type>::value &&
              std::is_same<
                  typename std::remove_cv<typename std::remove_pointer<
                      typename std::decay<T>::type>::type>::type,
                  char>::value> {};

template <class T, class = void>
struct is_range : std::false_type {};

template <class T>
struct is_range<
    T,
    std::void_t<decltype(std::begin(std::declval<T&>())),
                decltype(std::end(std::declval<T&>()))>> : std::true_type {};

template <class T, class = void>
struct is_tuple_like : std::false_type {};

template <class T>
struct is_tuple_like<
    T,
    std::void_t<decltype(std::tuple_size<remove_cvref_t<T>>::value)>>
    : std::true_type {};

template <class T>
struct is_optional : std::false_type {};

template <class T>
struct is_optional<std::optional<T>> : std::true_type {};

template <class T>
struct is_variant : std::false_type {};

template <class... T>
struct is_variant<std::variant<T...>> : std::true_type {};

template <class T>
struct is_vector_bool : std::false_type {};

template <class Allocator>
struct is_vector_bool<std::vector<bool, Allocator>> : std::true_type {};

template <class T, class = void>
struct is_associative_container : std::false_type {};

template <class T>
struct is_associative_container<
    T,
    std::void_t<typename T::key_type>> : std::true_type {};

template <class T, class = void>
struct has_val_method : std::false_type {};

template <class T>
struct has_val_method<
    T,
    std::void_t<decltype(std::declval<const T&>().val())>>
    : std::true_type {};

template <class T, class = void>
struct is_stream_extractable : std::false_type {};

template <class T>
struct is_stream_extractable<
    T,
    std::void_t<decltype(std::declval<std::istream&>()
                         >> std::declval<T&>())>> : std::true_type {};

template <class T, class = void>
struct is_resizable : std::false_type {};

template <class T>
struct is_resizable<
    T,
    std::void_t<decltype(std::declval<T&>().resize(std::size_t{})),
                decltype(std::declval<const T&>().max_size())>>
    : std::true_type {};

// Poison pill: an actual customization can only be found through ADL.
void fib_input_fields();

template <class T, class = void>
struct has_input_fields : std::false_type {};

template <class T>
struct has_input_fields<
    T,
    std::void_t<decltype(fib_input_fields(std::declval<T&>()))>>
    : std::true_type {};

struct AggregateAny {
    template <class T>
    constexpr operator T() const noexcept;
};

template <class T, std::size_t... Index>
auto aggregate_initialization_test(std::index_sequence<Index...>)
    -> decltype(T{(static_cast<void>(Index), AggregateAny{})...},
                std::true_type{});

template <class T>
auto aggregate_initialization_test(...) -> std::false_type;

template <class T, std::size_t Size>
using can_initialize_aggregate = decltype(
    aggregate_initialization_test<T>(std::make_index_sequence<Size>{}));

template <class T, std::size_t Size>
struct AggregateArity {
    static constexpr std::size_t value =
        can_initialize_aggregate<T, Size>::value
            ? Size
            : AggregateArity<T, Size - 1>::value;
};

template <class T>
struct AggregateArity<T, 0> {
    static constexpr std::size_t value = 0;
};

constexpr std::size_t max_automatic_aggregate_fields = 12;

template <class T>
struct is_automatic_aggregate
    : std::integral_constant<
          bool,
          std::is_aggregate<T>::value && std::is_standard_layout<T>::value &&
              !is_range<T>::value && !is_tuple_like<T>::value &&
              !can_initialize_aggregate<
                  T,
                  max_automatic_aggregate_fields + 1>::value> {};

class Reader {
public:
    explicit Reader(std::istream& input) : input_(input) {}

    template <class... Args>
    bool read_many(Args&&... args) {
        return (read_one(std::forward<Args>(args)) && ...);
    }

    template <class T>
    bool read_one(T& value) {
        using U = remove_cvref_t<T>;
        static_assert(!std::is_const<typename std::remove_reference<T>::type>::value,
                      "input requires writable values");

        if constexpr (has_input_fields<U>::value) {
            auto fields = fib_input_fields(value);
            using Fields = remove_cvref_t<decltype(fields)>;
            static_assert(is_tuple_like<Fields>::value,
                          "fib_input_fields must return a tuple-like value");
            return read_tuple(
                fields,
                std::make_index_sequence<std::tuple_size<Fields>::value>{});
        } else if constexpr (std::is_same<U, bool>::value) {
            int raw = 0;
            if (!(input_ >> raw)) return false;
            if (raw != 0 && raw != 1) return fail();
            value = raw != 0;
            return true;
        } else if constexpr (std::is_arithmetic<U>::value) {
            return static_cast<bool>(input_ >> value);
        } else if constexpr (std::is_enum<U>::value) {
            using Underlying = typename std::underlying_type<U>::type;
            Underlying raw{};
            if (!(input_ >> raw)) return false;
            value = static_cast<U>(raw);
            return true;
        } else if constexpr (is_basic_string<U>::value) {
            return static_cast<bool>(input_ >> value);
        } else if constexpr (is_char_array<T>::value) {
            constexpr std::size_t size =
                std::extent<typename std::remove_reference<T>::type>::value;
            for (std::size_t i = 0; i < size; ++i) {
                if (!(input_ >> value[i])) return false;
            }
            return true;
        } else if constexpr (is_char_pointer<T>::value ||
                             std::is_pointer<U>::value) {
            static_assert(dependent_false<U>::value,
                          "input does not accept pointers");
        } else if constexpr (is_optional<U>::value || is_variant<U>::value) {
            static_assert(
                dependent_false<U>::value,
                "input cannot infer optional/variant state; use an explicit parser");
        } else if constexpr (is_vector_bool<U>::value) {
            for (std::size_t i = 0; i < value.size(); ++i) {
                bool item = false;
                if (!read_one(item)) return false;
                value[i] = item;
            }
            return true;
        } else if constexpr (is_associative_container<U>::value) {
            static_assert(
                dependent_false<U>::value,
                "input does not fill associative containers; use input_n later");
        } else if constexpr (is_range<U>::value) {
            for (auto&& item : value) {
                if (!read_one(item)) return false;
            }
            return true;
        } else if constexpr (is_tuple_like<U>::value) {
            return read_tuple(
                value, std::make_index_sequence<std::tuple_size<U>::value>{});
        } else if constexpr (has_val_method<U>::value &&
                             std::is_assignable<U&, long long>::value) {
            long long raw = 0;
            if (!(input_ >> raw)) return false;
            value = raw;
            return true;
        } else if constexpr (is_stream_extractable<U>::value) {
            return static_cast<bool>(input_ >> value);
        } else if constexpr (is_automatic_aggregate<U>::value) {
            return read_aggregate(value);
        } else {
            static_assert(
                dependent_false<U>::value,
                "input cannot load this type; define operator>> or "
                "FIB_INPUT_FIELDS(Type, value.member, ...)");
        }
    }

    template <class Container, class Dimension>
    bool read_one(const SizedInput<Container, Dimension>& request) {
        std::size_t size = 0;
        if (!resolve_dimension(request.dimension, size)) return false;
        const std::array<std::size_t, 1> dimensions{size};
        if (!resize_shape<0>(request.container, dimensions)) return false;
        return read_one(request.container);
    }

    template <class Container, class... Dimensions>
    bool read_one(const ShapedInput<Container, Dimensions...>& request) {
        static_assert(sizeof...(Dimensions) != 0,
                      "shaped requires at least one dimension");
        std::array<std::size_t, sizeof...(Dimensions)> dimensions{};
        if (!resolve_dimensions(request.dimensions, dimensions)) return false;
        if (!resize_shape<0>(request.container, dimensions)) return false;
        return read_one(request.container);
    }

    template <class Container, class Height, class Width>
    bool read_one(const ShapedLinesInput<Container, Height, Width>& request) {
        using ContainerType = remove_cvref_t<Container>;
        static_assert(is_resizable<ContainerType>::value,
                      "shaped_lines requires a resizable string container");
        using Row = remove_cvref_t<decltype(*std::begin(request.container))>;
        static_assert(is_basic_string<Row>::value,
                      "shaped_lines requires a container of std::string");

        std::size_t height = 0;
        std::size_t width = 0;
        if (!resolve_dimension(request.height, height) ||
            !resolve_dimension(request.width, width)) {
            return false;
        }
        if (height > request.container.max_size()) return fail();
        request.container.resize(height);

        for (std::size_t row = 0; row < height; ++row) {
            if (width == 0) {
                request.container[row].clear();
                continue;
            }
            std::string value;
            if (!(input_ >> value)) return false;
            if (value.size() != width) return fail();
            request.container[row] = std::move(value);
        }
        return true;
    }

private:
    bool fail() {
        input_.setstate(std::ios::failbit);
        return false;
    }

    template <class Dimension>
    bool resolve_dimension(const Dimension& dimension, std::size_t& result) {
        using Raw = remove_cvref_t<decltype(dimension.get())>;
        static_assert(std::is_integral<Raw>::value || std::is_enum<Raw>::value,
                      "container dimensions must be integral values");

        using Value = typename dimension_value_type<Raw>::type;
        const Value value = static_cast<Value>(dimension.get());

        if constexpr (std::is_same<Value, bool>::value) {
            return fail();
        } else {
            if constexpr (std::is_signed<Value>::value) {
                if (value < 0) return fail();
            }
            using Unsigned = typename std::make_unsigned<Value>::type;
            const std::uintmax_t converted =
                static_cast<std::uintmax_t>(static_cast<Unsigned>(value));
            if (converted > std::numeric_limits<std::size_t>::max()) {
                return fail();
            }
            result = static_cast<std::size_t>(converted);
            return true;
        }
    }

    template <class Tuple, std::size_t Size, std::size_t... Index>
    bool resolve_dimensions_impl(
        const Tuple& source,
        std::array<std::size_t, Size>& destination,
        std::index_sequence<Index...>) {
        return (resolve_dimension(std::get<Index>(source), destination[Index]) &&
                ...);
    }

    template <class... Dimensions>
    bool resolve_dimensions(
        const std::tuple<Dimensions...>& source,
        std::array<std::size_t, sizeof...(Dimensions)>& destination) {
        return resolve_dimensions_impl(
            source,
            destination,
            std::make_index_sequence<sizeof...(Dimensions)>{});
    }

    template <std::size_t Index, class Container, std::size_t Size>
    bool resize_shape(Container& container,
                      const std::array<std::size_t, Size>& dimensions) {
        using U = remove_cvref_t<Container>;
        static_assert(is_resizable<U>::value,
                      "sized/shaped requires resizable containers at every "
                      "specified dimension");
        if (dimensions[Index] > container.max_size()) return fail();
        container.resize(dimensions[Index]);

        if constexpr (Index + 1 < Size) {
            for (auto&& item : container) {
                if (!resize_shape<Index + 1>(item, dimensions)) return false;
            }
        }
        return true;
    }

    template <class Tuple, std::size_t... Index>
    bool read_tuple(Tuple& value, std::index_sequence<Index...>) {
        return (read_one(std::get<Index>(value)) && ...);
    }

    template <class... Fields>
    bool read_fields(Fields&... fields) {
        return (read_one(fields) && ...);
    }

    template <class T>
    bool read_aggregate(T& value) {
        constexpr std::size_t size =
            AggregateArity<T, max_automatic_aggregate_fields>::value;
        if constexpr (size == 0) {
            return true;
        } else if constexpr (size == 1) {
            auto& [f0] = value;
            return read_fields(f0);
        } else if constexpr (size == 2) {
            auto& [f0, f1] = value;
            return read_fields(f0, f1);
        } else if constexpr (size == 3) {
            auto& [f0, f1, f2] = value;
            return read_fields(f0, f1, f2);
        } else if constexpr (size == 4) {
            auto& [f0, f1, f2, f3] = value;
            return read_fields(f0, f1, f2, f3);
        } else if constexpr (size == 5) {
            auto& [f0, f1, f2, f3, f4] = value;
            return read_fields(f0, f1, f2, f3, f4);
        } else if constexpr (size == 6) {
            auto& [f0, f1, f2, f3, f4, f5] = value;
            return read_fields(f0, f1, f2, f3, f4, f5);
        } else if constexpr (size == 7) {
            auto& [f0, f1, f2, f3, f4, f5, f6] = value;
            return read_fields(f0, f1, f2, f3, f4, f5, f6);
        } else if constexpr (size == 8) {
            auto& [f0, f1, f2, f3, f4, f5, f6, f7] = value;
            return read_fields(f0, f1, f2, f3, f4, f5, f6, f7);
        } else if constexpr (size == 9) {
            auto& [f0, f1, f2, f3, f4, f5, f6, f7, f8] = value;
            return read_fields(f0, f1, f2, f3, f4, f5, f6, f7, f8);
        } else if constexpr (size == 10) {
            auto& [f0, f1, f2, f3, f4, f5, f6, f7, f8, f9] = value;
            return read_fields(f0, f1, f2, f3, f4, f5, f6, f7, f8, f9);
        } else if constexpr (size == 11) {
            auto& [f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10] = value;
            return read_fields(f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10);
        } else if constexpr (size == 12) {
            auto& [f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11] =
                value;
            return read_fields(
                f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11);
        }
    }

    std::istream& input_;
};

}  // namespace detail
}  // namespace input_support
}  // namespace fib

template <class Container, class Size>
auto sized(Container& container, Size&& size) {
    auto dimension =
        ::fib::input_support::detail::make_dimension(std::forward<Size>(size));
    return ::fib::input_support::detail::SizedInput<Container,
                                                     decltype(dimension)>{
        container, std::move(dimension)};
}

template <class Container, class... Sizes>
auto shaped(Container& container, Sizes&&... sizes) {
    auto dimensions = std::make_tuple(
        ::fib::input_support::detail::make_dimension(
            std::forward<Sizes>(sizes))...);
    return ::fib::input_support::detail::ShapedInput<
        Container,
        decltype(::fib::input_support::detail::make_dimension(
            std::forward<Sizes>(sizes)))...>{container, std::move(dimensions)};
}

template <class Container, class Height, class Width>
auto shaped_lines(Container& container, Height&& height, Width&& width) {
    auto height_dimension = ::fib::input_support::detail::make_dimension(
        std::forward<Height>(height));
    auto width_dimension = ::fib::input_support::detail::make_dimension(
        std::forward<Width>(width));
    return ::fib::input_support::detail::ShapedLinesInput<
        Container,
        decltype(height_dimension),
        decltype(width_dimension)>{container,
                                   std::move(height_dimension),
                                   std::move(width_dimension)};
}

template <class... Args>
bool input(Args&&... args) {
    ::fib::input_support::detail::Reader reader(std::cin);
    return reader.read_many(std::forward<Args>(args)...);
}

inline bool input_line(std::string& value) {
    return static_cast<bool>(std::getline(std::cin, value));
}

inline bool input_line_ws(std::string& value) {
    return static_cast<bool>(std::getline(std::cin >> std::ws, value));
}

#define FIB_INPUT_FIELDS(Type, ...)                                           \
    inline auto fib_input_fields(Type& value) {                               \
        return std::tie(__VA_ARGS__);                                         \
    }
