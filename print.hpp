#pragma once

#include <cstddef>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <memory>
#include <mutex>
#include <optional>
#include <ostream>
#include <sstream>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

namespace fib {
namespace output {
namespace detail {

template <class T>
using remove_cvref_t =
    typename std::remove_cv<typename std::remove_reference<T>::type>::type;

template <class>
struct dependent_false : std::false_type {};

template <class T>
struct is_basic_string : std::false_type {};

template <class Traits, class Allocator>
struct is_basic_string<std::basic_string<char, Traits, Allocator>>
    : std::true_type {};

template <class T>
struct is_basic_string_view : std::false_type {};

template <class Traits>
struct is_basic_string_view<std::basic_string_view<char, Traits>>
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
    std::void_t<decltype(std::begin(std::declval<const T&>())),
                decltype(std::end(std::declval<const T&>()))>> : std::true_type {
};

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
struct is_reference_wrapper : std::false_type {};

template <class T>
struct is_reference_wrapper<std::reference_wrapper<T>> : std::true_type {};

template <class T>
struct is_unique_ptr : std::false_type {};

template <class T, class Deleter>
struct is_unique_ptr<std::unique_ptr<T, Deleter>> : std::true_type {};

template <class T>
struct is_shared_ptr : std::false_type {};

template <class T>
struct is_shared_ptr<std::shared_ptr<T>> : std::true_type {};

template <class T, class = void>
struct has_val_method : std::false_type {};

template <class T>
struct has_val_method<
    T,
    std::void_t<decltype(std::declval<const T&>().val())>>
    : std::integral_constant<
          bool,
          !std::is_void<decltype(std::declval<const T&>().val())>::value> {};

template <class T, class = void>
struct is_stream_insertable : std::false_type {};

template <class T>
struct is_stream_insertable<
    T,
    std::void_t<decltype(std::declval<std::ostream&>()
                         << std::declval<const T&>())>> : std::true_type {};

// Poison pill: an actual customization can only be found through ADL.
void fib_print_fields();

template <class T, class = void>
struct has_print_fields : std::false_type {};

template <class T>
struct has_print_fields<
    T,
    std::void_t<decltype(fib_print_fields(std::declval<const T&>()))>>
    : std::true_type {};

struct Block {
    std::vector<std::string> lines;
    bool multiline = false;
};

inline Block one_line(std::string value) {
    return Block{{std::move(value)}, false};
}

inline Block raw_text_block(const std::string& value) {
    Block result;
    std::size_t start = 0;
    while (true) {
        const std::size_t newline = value.find('\n', start);
        if (newline == std::string::npos) {
            result.lines.push_back(value.substr(start));
            result.multiline = result.lines.size() != 1;
            return result;
        }
        result.lines.push_back(value.substr(start, newline - start));
        start = newline + 1;
    }
}

inline std::string join_cells(const std::vector<std::string>& cells) {
    std::string result;
    for (const std::string& cell : cells) {
        if (cell.empty()) continue;
        if (!result.empty()) result.push_back(' ');
        result += cell;
    }
    return result;
}

inline Block compose_blocks(const std::vector<Block>& blocks) {
    if (blocks.empty()) return one_line("");

    bool has_multiline_block = false;
    for (const Block& block : blocks) {
        if (block.multiline || block.lines.size() != 1) {
            has_multiline_block = true;
            break;
        }
    }

    if (!has_multiline_block) {
        std::vector<std::string> cells;
        cells.reserve(blocks.size());
        for (const Block& block : blocks) cells.push_back(block.lines.front());
        return one_line(join_cells(cells));
    }

    Block result;
    result.multiline = true;
    for (const Block& block : blocks) {
        result.lines.insert(
            result.lines.end(), block.lines.begin(), block.lines.end());
    }
    if (result.lines.empty()) result.lines.emplace_back();
    return result;
}

template <class T>
std::string stream_to_string(const T& value) {
    std::ostringstream out;
    out.flags(std::cout.flags());
    out.precision(std::cout.precision());
    out.imbue(std::cout.getloc());
    out << value;
    return out.str();
}

template <class T>
Block render(const T& value);

template <class Tuple, std::size_t... Index>
Block render_tuple(const Tuple& value, std::index_sequence<Index...>) {
    std::vector<Block> fields;
    fields.reserve(sizeof...(Index));
    (fields.push_back(render(std::get<Index>(value))), ...);
    return compose_blocks(fields);
}

template <class Range>
Block render_range(const Range& value) {
    using Item = remove_cvref_t<decltype(*std::begin(value))>;

    if constexpr (std::is_same<Item, char>::value) {
        std::string text;
        for (const char ch : value) text.push_back(ch);
        return raw_text_block(text);
    } else {
        constexpr bool items_are_rows =
            is_basic_string<Item>::value || is_basic_string_view<Item>::value ||
            is_char_array<Item>::value || is_char_pointer<Item>::value ||
            is_range<Item>::value || is_tuple_like<Item>::value ||
            has_print_fields<Item>::value;

        std::vector<Block> items;
        for (const auto& item : value) items.push_back(render(item));
        if (items.empty()) return Block{{""}, items_are_rows};

        bool needs_multiple_rows = items_are_rows;
        for (const Block& item : items) {
            if (item.lines.size() != 1) {
                needs_multiple_rows = true;
                break;
            }
        }

        if (!needs_multiple_rows) return compose_blocks(items);

        Block result;
        result.multiline = true;
        for (const Block& item : items) {
            result.lines.insert(
                result.lines.end(), item.lines.begin(), item.lines.end());
        }
        if (result.lines.empty()) result.lines.emplace_back();
        return result;
    }
}

template <class T>
Block render(const T& value) {
    using U = remove_cvref_t<T>;

    if constexpr (has_print_fields<U>::value) {
        const auto fields = fib_print_fields(value);
        using Fields = remove_cvref_t<decltype(fields)>;
        static_assert(is_tuple_like<Fields>::value,
                      "fib_print_fields must return a tuple-like value");
        return render_tuple(
            fields,
            std::make_index_sequence<std::tuple_size<Fields>::value>{});
    } else if constexpr (std::is_same<U, std::nullptr_t>::value) {
        return one_line("null");
    } else if constexpr (is_char_array<T>::value) {
        constexpr std::size_t size =
            std::extent<typename std::remove_reference<T>::type>::value;
        std::size_t length = 0;
        while (length < size && value[length] != '\0') ++length;
        return raw_text_block(std::string(value, length));
    } else if constexpr (is_char_pointer<T>::value) {
        return value == nullptr ? one_line("null")
                                : raw_text_block(std::string(value));
    } else if constexpr (is_basic_string<U>::value ||
                         is_basic_string_view<U>::value) {
        return value.empty()
                   ? one_line("")
                   : raw_text_block(std::string(value.data(), value.size()));
    } else if constexpr (std::is_same<U, char>::value) {
        return raw_text_block(std::string(1, value));
    } else if constexpr (std::is_same<U, bool>::value) {
        return one_line(value ? "1" : "0");
    } else if constexpr (std::is_arithmetic<U>::value) {
        return raw_text_block(stream_to_string(value));
    } else if constexpr (std::is_enum<U>::value) {
        using Underlying = typename std::underlying_type<U>::type;
        return raw_text_block(stream_to_string(static_cast<Underlying>(value)));
    } else if constexpr (is_optional<U>::value) {
        return value ? render(*value) : one_line("null");
    } else if constexpr (is_variant<U>::value) {
        if (value.valueless_by_exception()) return one_line("null");
        return std::visit([](const auto& item) { return render(item); }, value);
    } else if constexpr (std::is_same<U, std::monostate>::value) {
        return one_line("null");
    } else if constexpr (is_reference_wrapper<U>::value) {
        return render(value.get());
    } else if constexpr (is_range<U>::value) {
        return render_range(value);
    } else if constexpr (is_tuple_like<U>::value) {
        return render_tuple(
            value, std::make_index_sequence<std::tuple_size<U>::value>{});
    } else if constexpr (std::is_pointer<U>::value || is_unique_ptr<U>::value ||
                         is_shared_ptr<U>::value) {
        static_assert(dependent_false<U>::value,
                      "print does not accept pointers except C strings");
    } else if constexpr (has_val_method<U>::value) {
        return render(value.val());
    } else if constexpr (is_stream_insertable<U>::value) {
        return raw_text_block(stream_to_string(value));
    } else {
        static_assert(
            dependent_false<U>::value,
            "print cannot output this type; define operator<< or "
            "FIB_PRINT_FIELDS(Type, value.member, ...)");
    }
}

inline std::mutex& output_mutex() {
    static std::mutex mutex;
    return mutex;
}

inline std::string serialize(const Block& block) {
    std::string result;
    for (std::size_t i = 0; i < block.lines.size(); ++i) {
        if (i != 0) result.push_back('\n');
        result += block.lines[i];
    }
    return result;
}

template <class... Args>
void print_impl(const Args&... args) {
    std::vector<Block> blocks;
    blocks.reserve(sizeof...(Args));
    (blocks.push_back(render(args)), ...);
    const std::string text = serialize(compose_blocks(blocks));

    std::lock_guard<std::mutex> lock(output_mutex());
    std::cout.write(text.data(), static_cast<std::streamsize>(text.size()));
    std::cout.put('\n');
}

}  // namespace detail
}  // namespace output
}  // namespace fib

template <class... Args>
void print(const Args&... args) {
    ::fib::output::detail::print_impl(args...);
}

template <class T>
void print(std::initializer_list<T> value) {
    ::fib::output::detail::print_impl(value);
}

#define FIB_PRINT_FIELDS(Type, ...)                                           \
    inline auto fib_print_fields(const Type& value) {                         \
        return std::tie(__VA_ARGS__);                                         \
    }
