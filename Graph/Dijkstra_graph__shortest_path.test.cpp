// library-checker-judge test case
// problem: graph/shortest_path
// library: Graph/Dijkstra.hpp



#include <bits/stdc++.h>
#include <atcoder/all>


// Define LOCAL from the compiler command line (for example: -DLOCAL).
// In a non-LOCAL build only the no-op macros at the bottom of this file exist;
// the implementation itself is deliberately unavailable.
#ifdef LOCAL

#include <chrono>
#include <cctype>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <memory>
#include <mutex>
#include <optional>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <typeinfo>
#include <utility>
#include <variant>
#include <vector>

namespace fib {
namespace local_debug {

struct FormatOptions {
    std::size_t max_depth = 16;
    std::size_t max_elements = 64;
};

namespace detail {

template <class T>
class RuntimeArrayView {
public:
    RuntimeArrayView(const T* data, std::size_t size)
        : data_(data), size_(size) {}

    const T* begin() const noexcept { return data_; }
    const T* end() const noexcept { return data_ + size_; }

private:
    const T* data_;
    std::size_t size_;
};

template <class T>
const T& make_debug_argument(const T& value,
                             std::size_t,
                             std::true_type) {
    return value;
}

// GCC/Clang VLA types cannot be template arguments. Array-to-pointer decay
// removes only the runtime-sized outer dimension; its length is restored here.
template <class T>
RuntimeArrayView<T> make_debug_argument(T* data,
                                        std::size_t total_bytes,
                                        std::false_type) {
    return RuntimeArrayView<T>(data, total_bytes / sizeof(T));
}

template <class T>
using remove_cvref_t =
    typename std::remove_cv<typename std::remove_reference<T>::type>::type;

template <class T, class = void>
struct is_stream_insertable : std::false_type {};

template <class T>
struct is_stream_insertable<
    T,
    std::void_t<decltype(std::declval<std::ostream&>()
                         << std::declval<const T&>())>> : std::true_type {};

template <class T, class = void>
struct has_val_method : std::false_type {};

template <class T>
struct has_val_method<
    T,
    std::void_t<decltype(std::declval<const T&>().val())>>
    : std::integral_constant<
          bool,
          !std::is_void<decltype(std::declval<const T&>().val())>::value> {};

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

// Poison pill: a matching function can only be found through ADL.
void fib_debug_fields();

template <class T, class = void>
struct has_debug_fields : std::false_type {};

template <class T>
struct has_debug_fields<
    T,
    std::void_t<decltype(fib_debug_fields(std::declval<const T&>()))>>
    : std::true_type {};

template <class Tuple>
struct NamedDebugFields {
    const char* names;
    Tuple values;
};

template <class T>
struct is_named_debug_fields : std::false_type {};

template <class Tuple>
struct is_named_debug_fields<NamedDebugFields<Tuple>> : std::true_type {};

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

inline std::string trim_field_name(std::string value) {
    std::size_t first = 0;
    while (first < value.size() &&
           std::isspace(static_cast<unsigned char>(value[first]))) {
        ++first;
    }
    std::size_t last = value.size();
    while (last > first &&
           std::isspace(static_cast<unsigned char>(value[last - 1]))) {
        --last;
    }
    value = value.substr(first, last - first);
    constexpr const char prefix[] = "value.";
    if (value.compare(0, sizeof(prefix) - 1, prefix) == 0) {
        value.erase(0, sizeof(prefix) - 1);
    }
    return value;
}

inline std::vector<std::string> split_field_names(const char* names) {
    std::vector<std::string> result;
    if (names == nullptr) return result;

    std::string current;
    int nesting = 0;
    char quote = '\0';
    bool escaped = false;
    for (const char ch : std::string(names)) {
        if (quote != '\0') {
            current.push_back(ch);
            if (escaped) escaped = false;
            else if (ch == '\\') escaped = true;
            else if (ch == quote) quote = '\0';
            continue;
        }
        if (ch == '\"' || ch == '\'') {
            quote = ch;
            current.push_back(ch);
        } else if (ch == '(' || ch == '[' || ch == '{') {
            ++nesting;
            current.push_back(ch);
        } else if (ch == ')' || ch == ']' || ch == '}') {
            --nesting;
            current.push_back(ch);
        } else if (ch == ',' && nesting == 0) {
            result.push_back(trim_field_name(current));
            current.clear();
        } else {
            current.push_back(ch);
        }
    }
    if (!current.empty()) result.push_back(trim_field_name(current));
    return result;
}

inline std::mutex& output_mutex() {
    static std::mutex mutex;
    return mutex;
}

inline void write_escaped(std::ostream& out, std::string_view value, char quote) {
    out.put(quote);
    for (const unsigned char ch : value) {
        switch (ch) {
            case '\\': out << "\\\\"; break;
            case '\n': out << "\\n"; break;
            case '\r': out << "\\r"; break;
            case '\t': out << "\\t"; break;
            case '\0': out << "\\0"; break;
            case '\"':
                if (quote == '\"') out << "\\\"";
                else out.put(static_cast<char>(ch));
                break;
            case '\'':
                if (quote == '\'') out << "\\'";
                else out.put(static_cast<char>(ch));
                break;
            default:
                if (ch >= 0x20 && ch < 0x7f) {
                    out.put(static_cast<char>(ch));
                } else {
                    const auto flags = out.flags();
                    const auto fill = out.fill();
                    out << "\\x" << std::hex << std::setw(2)
                        << std::setfill('0') << static_cast<unsigned int>(ch);
                    out.flags(flags);
                    out.fill(fill);
                }
        }
    }
    out.put(quote);
}

class Formatter {
public:
    Formatter(std::ostream& out, FormatOptions options)
        : out_(out), options_(options) {}

    template <class T>
    void write(const T& value, std::size_t depth = 0) {
        using U = remove_cvref_t<T>;

        if (depth > options_.max_depth) {
            out_ << "...";
        } else if constexpr (has_debug_fields<U>::value) {
            write_custom_object(fib_debug_fields(value), depth);
        } else if constexpr (std::is_same<U, std::nullptr_t>::value) {
            out_ << "nullptr";
        } else if constexpr (is_char_array<T>::value) {
            constexpr std::size_t size =
                std::extent<typename std::remove_reference<T>::type>::value;
            std::size_t length = size;
            if (length != 0 && value[length - 1] == '\0') --length;
            write_escaped(out_, std::string_view(value, length), '\"');
        } else if constexpr (is_char_pointer<T>::value) {
            if (value == nullptr) out_ << "nullptr";
            else write_escaped(out_, std::string_view(value), '\"');
        } else if constexpr (is_basic_string<U>::value ||
                             is_basic_string_view<U>::value) {
            write_escaped(out_, std::string_view(value.data(), value.size()), '\"');
        } else if constexpr (std::is_same<U, char>::value) {
            const char data[] = {value};
            write_escaped(out_, std::string_view(data, 1), '\'');
        } else if constexpr (std::is_same<U, bool>::value) {
            out_ << (value ? "true" : "false");
        } else if constexpr (is_optional<U>::value) {
            if (value) {
                out_ << "optional(";
                write(*value, depth + 1);
                out_ << ')';
            } else {
                out_ << "nullopt";
            }
        } else if constexpr (is_variant<U>::value) {
            if (value.valueless_by_exception()) {
                out_ << "variant[valueless]";
            } else {
                out_ << "variant[" << value.index() << "](";
                std::visit([&](const auto& item) { write(item, depth + 1); }, value);
                out_ << ')';
            }
        } else if constexpr (std::is_same<U, std::monostate>::value) {
            out_ << "monostate";
        } else if constexpr (is_reference_wrapper<U>::value) {
            write(value.get(), depth + 1);
        } else if constexpr (std::is_pointer<U>::value) {
            if (value == nullptr) {
                out_ << "nullptr";
            } else if constexpr (
                std::is_object<typename std::remove_pointer<U>::type>::value) {
                out_ << "<ptr:" << static_cast<const void*>(value) << '>';
            } else {
                out_ << "<function-ptr>";
            }
        } else if constexpr (is_unique_ptr<U>::value || is_shared_ptr<U>::value) {
            if (!value) out_ << "nullptr";
            else out_ << "<ptr:" << static_cast<const void*>(value.get()) << '>';
        } else if constexpr (is_tuple_like<U>::value && !is_range<U>::value) {
            write_tuple(value, depth);
        } else if constexpr (is_range<U>::value) {
            write_range(value, depth);
        } else if constexpr (std::is_enum<U>::value) {
            using Underlying = typename std::underlying_type<U>::type;
            out_ << static_cast<Underlying>(value);
        } else if constexpr (is_stream_insertable<U>::value) {
            out_ << value;
        } else if constexpr (has_val_method<U>::value) {
            write(value.val(), depth + 1);
        } else if constexpr (is_automatic_aggregate<U>::value) {
            write_aggregate(value, depth);
        } else {
            out_ << "<unprintable:" << typeid(U).name() << '>';
        }
    }

private:
    template <class Fields>
    void write_custom_object(const Fields& fields, std::size_t depth) {
        using U = remove_cvref_t<Fields>;
        if constexpr (is_named_debug_fields<U>::value) {
            write_object_tuple(
                fields.values, depth, split_field_names(fields.names));
        } else if constexpr (is_tuple_like<U>::value) {
            write_object_tuple(fields, depth, {});
        } else {
            out_ << '{';
            write(fields, depth + 1);
            out_ << '}';
        }
    }

    template <class Tuple, std::size_t... Index>
    void write_object_tuple_items(const Tuple& fields,
                                  std::size_t depth,
                                  const std::vector<std::string>& names,
                                  std::index_sequence<Index...>) {
        (write_object_field<Index>(fields, depth, names), ...);
    }

    template <std::size_t Index, class Tuple>
    void write_object_field(const Tuple& fields,
                            std::size_t depth,
                            const std::vector<std::string>& names) {
        out_ << (Index == 0 ? "\n" : ",\n");
        write_indent(depth + 1);
        const std::string key =
            Index < names.size() ? names[Index] : "_" + std::to_string(Index);
        write_escaped(out_, key, '\"');
        out_ << ": ";
        write(std::get<Index>(fields), depth + 1);
    }

    template <class Tuple>
    void write_object_tuple(const Tuple& fields,
                            std::size_t depth,
                            const std::vector<std::string>& names) {
        constexpr std::size_t size =
            std::tuple_size<remove_cvref_t<Tuple>>::value;
        out_ << '{';
        write_object_tuple_items(
            fields, depth, names, std::make_index_sequence<size>{});
        if constexpr (size != 0) {
            out_ << '\n';
            write_indent(depth);
        }
        out_ << '}';
    }

    template <class... Fields>
    void write_automatic_object(std::size_t depth, const Fields&... fields) {
        write_object_tuple(std::forward_as_tuple(fields...), depth, {});
    }

    template <class T>
    void write_aggregate(const T& value, std::size_t depth) {
        constexpr std::size_t size =
            AggregateArity<T, max_automatic_aggregate_fields>::value;
        if constexpr (size == 0) {
            write_automatic_object(depth);
        } else if constexpr (size == 1) {
            const auto& [f0] = value;
            write_automatic_object(depth, f0);
        } else if constexpr (size == 2) {
            const auto& [f0, f1] = value;
            write_automatic_object(depth, f0, f1);
        } else if constexpr (size == 3) {
            const auto& [f0, f1, f2] = value;
            write_automatic_object(depth, f0, f1, f2);
        } else if constexpr (size == 4) {
            const auto& [f0, f1, f2, f3] = value;
            write_automatic_object(depth, f0, f1, f2, f3);
        } else if constexpr (size == 5) {
            const auto& [f0, f1, f2, f3, f4] = value;
            write_automatic_object(depth, f0, f1, f2, f3, f4);
        } else if constexpr (size == 6) {
            const auto& [f0, f1, f2, f3, f4, f5] = value;
            write_automatic_object(depth, f0, f1, f2, f3, f4, f5);
        } else if constexpr (size == 7) {
            const auto& [f0, f1, f2, f3, f4, f5, f6] = value;
            write_automatic_object(depth, f0, f1, f2, f3, f4, f5, f6);
        } else if constexpr (size == 8) {
            const auto& [f0, f1, f2, f3, f4, f5, f6, f7] = value;
            write_automatic_object(depth, f0, f1, f2, f3, f4, f5, f6, f7);
        } else if constexpr (size == 9) {
            const auto& [f0, f1, f2, f3, f4, f5, f6, f7, f8] = value;
            write_automatic_object(
                depth, f0, f1, f2, f3, f4, f5, f6, f7, f8);
        } else if constexpr (size == 10) {
            const auto& [f0, f1, f2, f3, f4, f5, f6, f7, f8, f9] = value;
            write_automatic_object(
                depth, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9);
        } else if constexpr (size == 11) {
            const auto& [f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10] =
                value;
            write_automatic_object(
                depth, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10);
        } else if constexpr (size == 12) {
            const auto& [f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10,
                         f11] = value;
            write_automatic_object(
                depth, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11);
        }
    }

    template <class Tuple, std::size_t... Index>
    void write_tuple_items(const Tuple& value,
                           std::size_t depth,
                           std::index_sequence<Index...>) {
        std::size_t item_index = 0;
        ((out_ << (item_index++ == 0 ? "" : ", "),
          write(std::get<Index>(value), depth + 1)),
         ...);
    }

    template <class Tuple>
    void write_tuple(const Tuple& value, std::size_t depth) {
        out_ << '(';
        write_tuple_items(
            value,
            depth,
            std::make_index_sequence<std::tuple_size<remove_cvref_t<Tuple>>::value>{});
        out_ << ')';
    }

    template <class Range>
    void write_range(const Range& value, std::size_t depth) {
        using Item = remove_cvref_t<decltype(*std::begin(value))>;
        constexpr bool multiline =
            is_range<Item>::value && !is_basic_string<Item>::value &&
            !is_basic_string_view<Item>::value &&
            !is_char_array<Item>::value && !is_char_pointer<Item>::value;
        constexpr bool object_items = has_debug_fields<Item>::value ||
                                      is_automatic_aggregate<Item>::value;
        constexpr bool pretty = multiline || object_items;

        out_ << '[';
        std::size_t count = 0;
        for (const auto& item : value) {
            if (count == options_.max_elements) {
                write_range_separator(count, pretty, depth);
                out_ << "...";
                ++count;
                break;
            }
            write_range_separator(count, pretty, depth);
            write(item, depth + 1);
            ++count;
        }
        if constexpr (pretty) {
            if (count != 0) {
                out_ << '\n';
                write_indent(depth);
            }
        }
        out_ << ']';
    }

    void write_range_separator(std::size_t count,
                               bool multiline,
                               std::size_t depth) {
        if (multiline) {
            out_ << (count == 0 ? "\n" : ",\n");
            write_indent(depth + 1);
        } else if (count != 0) {
            out_ << ", ";
        }
    }

    void write_indent(std::size_t depth) {
        for (std::size_t i = 0; i < depth * 2; ++i) out_.put(' ');
    }

    std::ostream& out_;
    FormatOptions options_;
};

template <class... Args>
std::string format_values(const FormatOptions& options, const Args&... args) {
    std::ostringstream out;
    Formatter formatter(out, options);
    std::size_t index = 0;
    ((out << (index++ == 0 ? "" : " "), formatter.write(args)), ...);
    return out.str();
}

template <class Tuple, std::size_t... Index>
std::string format_tuple_values(const FormatOptions& options,
                                const Tuple& values,
                                std::index_sequence<Index...>) {
    return format_values(options, std::get<Index>(values)...);
}

}  // namespace detail

template <class T>
std::string to_debug_string(const T& value,
                            const FormatOptions& options = FormatOptions{}) {
    return detail::format_values(options, value);
}

// Writes one formatted line to stderr. This function only exists in LOCAL builds.
template <class... Args>
void print(const Args&... args) {
    const std::string message = detail::format_values(FormatOptions{}, args...);
    std::lock_guard<std::mutex> lock(detail::output_mutex());
    std::cerr << message << '\n';
    std::cerr.flush();
}

template <class Tuple>
void print_tuple_at(const char* file,
                    int line,
                    const char* expressions,
                    const Tuple& values) {
    constexpr std::size_t size =
        std::tuple_size<detail::remove_cvref_t<Tuple>>::value;
    const std::string message = detail::format_tuple_values(
        FormatOptions{}, values, std::make_index_sequence<size>{});

    std::lock_guard<std::mutex> lock(detail::output_mutex());
    std::cerr << "[debug " << file << ':' << line << ']';
    if (expressions != nullptr && expressions[0] != '\0') {
        std::cerr << ' ' << expressions << " =";
    }
    if (!message.empty()) std::cerr << ' ' << message;
    std::cerr << '\n';
    std::cerr.flush();
}

template <class T>
void print_value_at(const char* file,
                    int line,
                    const char* expression,
                    const T& value) {
    const std::string message = detail::format_values(FormatOptions{}, value);

    std::lock_guard<std::mutex> lock(detail::output_mutex());
    std::cerr << "[debug " << file << ':' << line << ']';
    if (expression != nullptr && expression[0] != '\0') {
        std::cerr << ' ' << expression << " =";
    }
    if (!message.empty()) std::cerr << ' ' << message;
    std::cerr << '\n';
    std::cerr.flush();
}

class LoopDetected : public std::runtime_error {
public:
    explicit LoopDetected(const std::string& message)
        : std::runtime_error(message) {}
};

class LoopGuard {
public:
    LoopGuard(std::string label,
              const char* file,
              int line,
              std::uint64_t max_iterations,
              std::chrono::milliseconds timeout,
              std::size_t max_same_state = 0,
              FormatOptions format_options = FormatOptions{})
        : label_(std::move(label)),
          file_(file),
          line_(line),
          max_iterations_(max_iterations),
          timeout_(timeout),
          max_same_state_(max_same_state),
          format_options_(format_options),
          started_at_(Clock::now()) {}

    LoopGuard(const LoopGuard&) = delete;
    LoopGuard& operator=(const LoopGuard&) = delete;
    LoopGuard(LoopGuard&&) = default;
    LoopGuard& operator=(LoopGuard&&) = default;

    template <class... State>
    void tick(const State&... state) {
        ++iterations_;
        last_state_ = detail::format_values(format_options_, state...);

        if (max_same_state_ != 0) {
            if (last_state_ == previous_state_) {
                ++same_state_count_;
            } else {
                previous_state_ = last_state_;
                same_state_count_ = 1;
            }
        }

        if (max_iterations_ != 0 && iterations_ > max_iterations_) {
            fail("iteration limit exceeded");
        }

        if (timeout_.count() != 0 && Clock::now() - started_at_ > timeout_) {
            fail("time limit exceeded");
        }

        if (max_same_state_ != 0 && same_state_count_ > max_same_state_) {
            fail("same state repeated");
        }
    }

    std::uint64_t iterations() const noexcept { return iterations_; }

private:
    using Clock = std::chrono::steady_clock;

    [[noreturn]] void fail(const char* reason) const {
        const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                                 Clock::now() - started_at_)
                                 .count();

        std::ostringstream message;
        message << "loop '" << label_ << "' detected at " << file_ << ':' << line_
                << "; reason=" << reason << "; iterations=" << iterations_
                << "; elapsed_ms=" << elapsed;
        if (!last_state_.empty()) message << "; state=" << last_state_;

        const std::string text = message.str();
        {
            std::lock_guard<std::mutex> lock(detail::output_mutex());
            std::cerr << "[loop-detected] " << text << '\n';
            std::cerr.flush();
        }
        throw LoopDetected(text);
    }

    std::string label_;
    const char* file_;
    int line_;
    std::uint64_t max_iterations_;
    std::chrono::milliseconds timeout_;
    std::size_t max_same_state_;
    FormatOptions format_options_;
    Clock::time_point started_at_;
    std::uint64_t iterations_ = 0;
    std::size_t same_state_count_ = 0;
    std::string previous_state_;
    std::string last_state_;
};

inline LoopGuard make_loop_guard(const char* label,
                                 const char* file,
                                 int line,
                                 std::uint64_t max_iterations,
                                 std::int64_t timeout_milliseconds,
                                 std::size_t max_same_state = 0) {
    return LoopGuard(label,
                     file,
                     line,
                     max_iterations,
                     std::chrono::milliseconds(timeout_milliseconds),
                     max_same_state);
}

}  // namespace local_debug
}  // namespace fib

// GCC/Clang report whether sizeof(value) is runtime-sized. Standard C++ types
// take the true_type path; a VLA takes the runtime-array-view path.
#if defined(__GNUC__) || defined(__clang__)
#define FIB_DEBUG_DETAIL_SIZE_TAG(value)                                      \
    std::integral_constant<bool, __builtin_constant_p(sizeof(value))>{}
#else
#define FIB_DEBUG_DETAIL_SIZE_TAG(value) std::true_type{}
#endif

#define FIB_DEBUG_DETAIL_ARGUMENT(value)                                      \
    ::fib::local_debug::detail::make_debug_argument(                          \
        (value), sizeof(value), FIB_DEBUG_DETAIL_SIZE_TAG(value))

#define FIB_DEBUG_DETAIL_ONE(expressions, value)                              \
    ::fib::local_debug::print_value_at(                                       \
        __FILE__, __LINE__, expressions, FIB_DEBUG_DETAIL_ARGUMENT(value))

#define FIB_DEBUG_DETAIL_MANY(expressions, ...)                               \
    ::fib::local_debug::print_tuple_at(                                       \
        __FILE__, __LINE__, expressions, std::forward_as_tuple(__VA_ARGS__))

#define FIB_DEBUG_DETAIL_SELECT(                                              \
    _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15,      \
    _16, NAME, ...) NAME

// One argument gets transparent VLA support. For a VLA among multiple values,
// wrap it in FIB_VLA: FIB_DEBUG(FIB_VLA(dp), another_value).
#define FIB_DEBUG_DETAIL_DISPATCH(expressions, ...)                           \
    FIB_DEBUG_DETAIL_SELECT(                                                  \
        __VA_ARGS__,                                                          \
        FIB_DEBUG_DETAIL_MANY, FIB_DEBUG_DETAIL_MANY,                         \
        FIB_DEBUG_DETAIL_MANY, FIB_DEBUG_DETAIL_MANY,                         \
        FIB_DEBUG_DETAIL_MANY, FIB_DEBUG_DETAIL_MANY,                         \
        FIB_DEBUG_DETAIL_MANY, FIB_DEBUG_DETAIL_MANY,                         \
        FIB_DEBUG_DETAIL_MANY, FIB_DEBUG_DETAIL_MANY,                         \
        FIB_DEBUG_DETAIL_MANY, FIB_DEBUG_DETAIL_MANY,                         \
        FIB_DEBUG_DETAIL_MANY, FIB_DEBUG_DETAIL_MANY,                         \
        FIB_DEBUG_DETAIL_MANY, FIB_DEBUG_DETAIL_ONE,                          \
        FIB_DEBUG_DETAIL_UNUSED)(expressions, __VA_ARGS__)

#define FIB_DEBUG(...) FIB_DEBUG_DETAIL_DISPATCH(#__VA_ARGS__, __VA_ARGS__)

#define FIB_VLA(value) FIB_DEBUG_DETAIL_ARGUMENT(value)

#define FIB_DEBUG_FIELDS(Type, ...)                                           \
    inline auto fib_debug_fields(const Type& value) {                         \
        return ::fib::local_debug::detail::NamedDebugFields<                  \
            decltype(std::tie(__VA_ARGS__))>{#__VA_ARGS__,                    \
                                               std::tie(__VA_ARGS__)};         \
    }

// Usage: FIB_LOOP_GUARD(name, max_iterations, timeout_ms[, max_same_state]).
// A zero limit disables that particular check. Call FIB_LOOP_TICK in the loop.
#define FIB_LOOP_GUARD(name, ...)                                             \
    auto name = ::fib::local_debug::make_loop_guard(                          \
        #name, __FILE__, __LINE__, __VA_ARGS__)

#define FIB_LOOP_TICK(name, ...) (name).tick(__VA_ARGS__)

#define FIB_DEBUG_ENABLED 1

#else

#define FIB_DEBUG(...) ((void)0)
#define FIB_VLA(value) (value)
#define FIB_DEBUG_FIELDS(Type, ...)
#define FIB_LOOP_GUARD(name, ...) ((void)0)
#define FIB_LOOP_TICK(name, ...) ((void)0)
#define FIB_DEBUG_ENABLED 0

#endif
// Injecting debug.hpp <- _fib/template.hpp

using namespace std;
using namespace atcoder;

/* frequently used types */
using ll = long long;
using vll = vector<ll>;
using vvll = vector<vll>;
using vvvll = vector<vvll>;

using ull = unsigned long long;
using vull = vector<ull>;
using vvull = vector<vull>;
using vvvull = vector<vvull>;

using Graph = vvll;

using vch = vector<char>;
using vvch = vector<vector<char>>;
using vs = vector<string>;

using Pll = pair<ll, ll>;
using vPll = vector<Pll>;
using pll = pair<ll, ll>;
using pii = pair<int, int>;

using tup = tuple<ll, ll, ll>;

using bl = bool;
using vbl = vector<bool>;
using vvbl = vector<vbl>;
using vvvbl = vector<vvbl>;

using ld = long double;
using vld = vector<ld>;
using vvld = vector<vld>;
using vvvld = vector<vvld>;

/* modint aliases: mint remains compatible with the old template. */
using mint998 = atcoder::modint998244353;
using mint107 = atcoder::modint1000000007;
using mint = mint998;
using vmint = vector<mint>;
using vvmint = vector<vmint>;
using vvvmint = vector<vvmint>;
template <int Mod>
using static_mint = atcoder::static_modint<Mod>;
template <int Id = 0>
using dynamic_mint = atcoder::dynamic_modint<Id>;

constexpr int infint = (1 << 30) - 1;
constexpr ll inf = 1LL << 60;

template <class Cost>
struct WeightedEdge {
    int to;
    Cost weight;
    int id = -1;
};

template <class Cost>
using WeightedGraphT = vector<vector<WeightedEdge<Cost>>>;

using WeightedGraphEdge = WeightedEdge<ll>;
using WeightedGraph = WeightedGraphT<ll>;

template <class T>
inline bool chmax(T& a, const T& b) {
    if (a < b) {
        a = b;
        return true;
    }
    return false;
}

template <class T>
inline bool chmin(T& a, const T& b) {
    if (b < a) {
        a = b;
        return true;
    }
    return false;
}

#define rep(i, x, lim) for (ll i = (x); i < static_cast<ll>(lim); ++i)
#define all(x) std::begin(x), std::end(x)
#define rall(x) std::rbegin(x), std::rend(x)

template <class Container>
constexpr int sz(const Container& container) {
    return static_cast<int>(container.size());
}

namespace fib {
namespace utility {

template <class Range>
auto array_max(const Range& range) {
    using Value = std::decay_t<decltype(*std::begin(range))>;
    const auto first = std::begin(range);
    const auto last = std::end(range);
    assert(first != last && "arr_max requires a non-empty range");
    return Value(*std::max_element(first, last));
}

template <class Range>
auto array_min(const Range& range) {
    using Value = std::decay_t<decltype(*std::begin(range))>;
    const auto first = std::begin(range);
    const auto last = std::end(range);
    assert(first != last && "arr_min requires a non-empty range");
    return Value(*std::min_element(first, last));
}

template <class Range>
auto array_sum(const Range& range) {
    using Value = std::decay_t<decltype(*std::begin(range))>;
    Value result{};
    for (const auto& value : range) result += value;
    return result;
}

}  // namespace utility
}  // namespace fib

// Function-backed macros evaluate the range expression exactly once.
#define arr_max(range) (::fib::utility::array_max((range)))
#define arr_min(range) (::fib::utility::array_min((range)))
#define arr_sum(range) (::fib::utility::array_sum((range)))

constexpr ll mod1000000007 = 1000000007LL;
constexpr ll mod998244353 = 998244353LL;

inline constexpr array<ll, 8> dx{1, -1, 0, 0, 1, 1, -1, -1};
inline constexpr array<ll, 8> dy{0, 0, 1, -1, 1, -1, 1, -1};

inline constexpr array<ull, 64> pow2ll{
    1ULL, 2ULL, 4ULL, 8ULL, 16ULL, 32ULL, 64ULL, 128ULL,
    256ULL, 512ULL, 1024ULL, 2048ULL, 4096ULL, 8192ULL,
    16384ULL, 32768ULL, 65536ULL, 131072ULL, 262144ULL,
    524288ULL, 1048576ULL, 2097152ULL, 4194304ULL, 8388608ULL,
    16777216ULL, 33554432ULL, 67108864ULL, 134217728ULL,
    268435456ULL, 536870912ULL, 1073741824ULL, 2147483648ULL,
    4294967296ULL, 8589934592ULL, 17179869184ULL, 34359738368ULL,
    68719476736ULL, 137438953472ULL, 274877906944ULL,
    549755813888ULL, 1099511627776ULL, 2199023255552ULL,
    4398046511104ULL, 8796093022208ULL, 17592186044416ULL,
    35184372088832ULL, 70368744177664ULL, 140737488355328ULL,
    281474976710656ULL, 562949953421312ULL, 1125899906842624ULL,
    2251799813685248ULL, 4503599627370496ULL, 9007199254740992ULL,
    18014398509481984ULL, 36028797018963968ULL, 72057594037927936ULL,
    144115188075855872ULL, 288230376151711744ULL,
    576460752303423488ULL, 1152921504606846976ULL,
    2305843009213693952ULL, 4611686018427387904ULL,
    9223372036854775808ULL,
};

inline constexpr array<ull, 20> pow10ll{
    1ULL, 10ULL, 100ULL, 1000ULL, 10000ULL, 100000ULL, 1000000ULL,
    10000000ULL, 100000000ULL, 1000000000ULL, 10000000000ULL,
    100000000000ULL, 1000000000000ULL, 10000000000000ULL,
    100000000000000ULL, 1000000000000000ULL, 10000000000000000ULL,
    100000000000000000ULL, 1000000000000000000ULL,
    10000000000000000000ULL,
};

#ifndef FIB_NO_MAIN

#endif
// Injecting ../template.hpp <- _fib/Graph/Dijkstra.hpp

template <class Cost>
struct DijkstraResult {
    Cost infinity{};
    vector<Cost> dist;
    vector<int> parent_vertex;
    vector<int> parent_edge;

    bool reachable(int vertex) const { return dist[vertex] != infinity; }

    vector<int> restore_vertices(int target) const {
        if (!reachable(target)) return {};
        vector<int> path;
        for (int vertex = target; vertex != -1;
             vertex = parent_vertex[vertex]) {
            path.push_back(vertex);
        }
        reverse(path.begin(), path.end());
        return path;
    }

    vector<int> restore_edges(int target) const {
        if (!reachable(target)) return {};
        vector<int> path;
        while (parent_vertex[target] != -1) {
            path.push_back(parent_edge[target]);
            target = parent_vertex[target];
        }
        reverse(path.begin(), path.end());
        return path;
    }
};

template <class Cost>
DijkstraResult<Cost> dijkstra(
    const WeightedGraphT<Cost>& graph,
    const vector<int>& sources,
    Cost infinity = std::numeric_limits<Cost>::max() / Cost(4)) {
    const int n = static_cast<int>(graph.size());
    DijkstraResult<Cost> result{
        infinity,
        vector<Cost>(n, infinity),
        vector<int>(n, -1),
        vector<int>(n, -1),
    };

    using QueueEntry = pair<Cost, int>;
    priority_queue<QueueEntry, vector<QueueEntry>, greater<QueueEntry>> queue;
    for (const int source : sources) {
        assert(0 <= source && source < n);
        if (result.dist[source] == Cost{}) continue;
        result.dist[source] = Cost{};
        queue.emplace(Cost{}, source);
    }

    while (!queue.empty()) {
        const auto [distance, vertex] = queue.top();
        queue.pop();
        if (result.dist[vertex] != distance) continue;

        for (int index = 0; index < static_cast<int>(graph[vertex].size());
             ++index) {
            const auto& edge = graph[vertex][index];
            assert(!(edge.weight < Cost{}) &&
                   "dijkstra requires non-negative edge costs");
            if (edge.weight > infinity ||
                result.dist[vertex] > infinity - edge.weight) {
                continue;
            }
            const Cost next_distance = result.dist[vertex] + edge.weight;
            if (next_distance >= result.dist[edge.to]) continue;
            result.dist[edge.to] = next_distance;
            result.parent_vertex[edge.to] = vertex;
            result.parent_edge[edge.to] = edge.id == -1 ? index : edge.id;
            queue.emplace(next_distance, edge.to);
        }
    }
    return result;
}

template <class Cost>
DijkstraResult<Cost> dijkstra(
    const WeightedGraphT<Cost>& graph,
    int source,
    Cost infinity = std::numeric_limits<Cost>::max() / Cost(4)) {
    return dijkstra(graph, vector<int>{source}, infinity);
}
// Injecting Graph/Dijkstra.hpp <- _fib/Graph/Dijkstra_graph__shortest_path.test.cpp

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, source, target;
    cin >> n >> m >> source >> target;
    WeightedGraph graph(n);
    vector<pair<int, int>> endpoints(m);
    for (int id = 0; id < m; ++id) {
        int from, to;
        ll cost;
        cin >> from >> to >> cost;
        endpoints[id] = {from, to};
        graph[from].push_back({to, cost, id});
    }
    const auto result = dijkstra(graph, source, numeric_limits<ll>::max() / 4);
    if (!result.reachable(target)) {
        cout << -1 << '\n';
        return 0;
    }
    const auto path = result.restore_edges(target);
    cout << result.dist[target] << ' ' << path.size() << '\n';
    for (const int edge : path) {
        cout << endpoints[edge].first << ' ' << endpoints[edge].second << '\n';
    }
}
